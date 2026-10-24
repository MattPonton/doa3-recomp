/* Runs the production event driver against the bundled, unmodified GekkoNet.
 * The small deterministic simulation is a test fixture, NOT DOA3 game state. */
#include "rollback.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <thread>
#include <chrono>

#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #x); std::exit(1); } } while (0)

struct State {
    uint32_t frame = 0, rng = 12345, position[2] = {}, hash = 0, tick = 0;
};
struct Sim {
    State state;
    unsigned loads = 0, sounds = 0, presents = 0;
    bool broken_restore = false, save_fail = false, load_fail = false, step_fail = false;
};
static np_pad input(int frame, int player)
{
    np_pad pad{};
    if (frame < 0 || frame >= 900) return pad; // neutral tail lets predictions settle
    pad.buttons = uint16_t((frame * 17 + player * 41) & 255);
    for (int i = 0; i < 8; ++i) pad.analog[i] = uint8_t((frame + i * 11 + player) % 256);
    pad.lx = int16_t(frame * 13 - 10000);
    pad.ly = int16_t(frame * 7 - 3000);
    pad.rx = int16_t(12000 - frame * 5);
    pad.ry = int16_t(frame * 3 - 9000);
    return pad;
}
static uint32_t checksum(const void *src, size_t len)
{
    auto *bytes = static_cast<const uint8_t *>(src);
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < len; ++i) hash = (hash ^ bytes[i]) * 16777619u;
    return hash;
}
static void step(State &s, const np_pad pads[2])
{
    s.rng = s.rng * 1664525u + 1013904223u;
    for (int p = 0; p < 2; ++p) {
        s.position[p] += uint32_t(pads[p].lx) + pads[p].buttons + (s.rng & 255u);
        s.hash = s.hash * 33u + checksum(&pads[p], sizeof(np_pad));
    }
    ++s.frame;
    s.tick = s.frame * 1000u / 60u;
}
static int save(void *user, void *dst, uint32_t capacity, uint32_t *written, uint32_t *hash)
{
    Sim &sim = *static_cast<Sim *>(user);
    CHECK(capacity == sizeof(State));
    if (sim.save_fail) return 0;
    std::memcpy(dst, &sim.state, sizeof(State));
    *written = sizeof(State);
    *hash = checksum(dst, *written);
    return 1;
}
static int load(void *user, const void *src, uint32_t size)
{
    Sim &sim = *static_cast<Sim *>(user);
    CHECK(size == sizeof(State));
    if (sim.load_fail) return 0;
    uint32_t stale_rng = sim.state.rng;
    std::memcpy(&sim.state, src, size);
    if (sim.broken_restore) sim.state.rng = stale_rng;
    ++sim.loads;
    return 1;
}
static int advance(void *user, int frame, const np_pad pads[2], int replay)
{
    Sim &sim = *static_cast<Sim *>(user);
    CHECK(sim.state.frame == uint32_t(frame));
    if (sim.step_fail) return 0;
    step(sim.state, pads);
    if (!replay) ++sim.sounds; // proves the game receives the replay suppression flag
    return 1;
}
static void present(void *user, int frame)
{
    Sim &sim = *static_cast<Sim *>(user);
    CHECK(sim.state.frame == uint32_t(frame + 1));
    ++sim.presents;
}
static const np_rollback_callbacks callbacks{save, load, advance, present};
static void init(np_rollback &rb, Sim &sim, unsigned window = 8)
{
    CHECK(np_rollback_init(&rb, &callbacks, &sim, sizeof(State), window));
}
static bool consume(np_rollback &rb, GekkoSession *session)
{
    int count = 0, session_count = 0;
    auto events = gekko_update_session(session, &count);
    auto session_events = gekko_session_events(session, &session_count);
    return np_rollback_session_events(&rb, session_events, session_count) &&
           np_rollback_process(&rb, events, count);
}

static void stress(bool broken)
{
    Sim sim;
    sim.broken_restore = broken;
    np_rollback rb;
    init(rb, sim);
    GekkoConfig config;
    CHECK(np_rollback_config(&rb, &config));
    config.check_distance = 8;
    GekkoSession *session = nullptr;
    CHECK(gekko_create(&session, GekkoStressSession));
    gekko_start(session, &config);
    CHECK(gekko_add_actor(session, GekkoLocalPlayer, nullptr) == 0);
    CHECK(gekko_add_actor(session, GekkoLocalPlayer, nullptr) == 1);
    State reference;
    for (int frame = 0; frame < 1100; ++frame) {
        np_pad pads[2] = {input(frame, 0), input(frame, 1)};
        gekko_add_local_input(session, 0, &pads[0]);
        gekko_add_local_input(session, 1, &pads[1]);
        if (!consume(rb, session)) { CHECK(broken); break; }
        step(reference, pads);
        if (!broken) CHECK(std::memcmp(&reference, &sim.state, sizeof(State)) == 0);
    }
    if (broken) {
        CHECK(rb.failed && std::strstr(rb.error, "desync"));
    } else {
        CHECK(rb.forward_frames == 1100 && rb.replayed_frames > 7000);
        CHECK(sim.sounds == 1100 && sim.presents == 1100);
        CHECK(rb.restores > 1000 && sim.loads == rb.restores);
        std::printf("Stress: %llu forward, %llu replayed frames; every frame matched reference.\n",
                    (unsigned long long)rb.forward_frames, (unsigned long long)rb.replayed_frames);
    }
    CHECK(gekko_destroy(&session));
}

struct Packet { int destination, due; char sender; std::vector<char> data; };
static std::vector<Packet> wire;
static int network_tick, packet_serial, fault_mode;
template<int Side> static void send_packet(GekkoNetAddress *, const char *data, int length)
{
    ++packet_serial;
    if (fault_mode && (packet_serial % 7 == 0 || (network_tick >= 400 && network_tick < 440))) return;
    int delay = fault_mode ? 2 + packet_serial % 5 : 0;
    Packet packet{1 - Side, network_tick + delay, char('0' + Side), {data, data + length}};
    wire.push_back(packet);
    if (fault_mode && packet_serial % 11 == 0) { packet.due += 2; wire.push_back(packet); }
}
template<int Side> static GekkoNetResult **receive_packets(int *count)
{
    static std::vector<GekkoNetResult *> results;
    results.clear();
    // Reverse delivery intentionally reorders packets that arrive together.
    for (size_t i = wire.size(); i-- > 0;) {
        const Packet &packet = wire[i];
        if (packet.destination != Side || packet.due > network_tick) continue;
        auto *result = static_cast<GekkoNetResult *>(std::malloc(sizeof(GekkoNetResult)));
        CHECK(result);
        result->addr.data = std::malloc(1);
        result->addr.size = 1;
        result->data_len = unsigned(packet.data.size());
        result->data = std::malloc(result->data_len);
        CHECK(result->addr.data && result->data);
        *static_cast<char *>(result->addr.data) = packet.sender;
        std::memcpy(result->data, packet.data.data(), result->data_len);
        results.push_back(result);
        wire.erase(wire.begin() + i);
    }
    *count = int(results.size());
    return results.data();
}
static void free_packet(void *data) { std::free(data); }
static GekkoNetAdapter adapters[2] = {
    {send_packet<0>, receive_packets<0>, free_packet},
    {send_packet<1>, receive_packets<1>, free_packet}
};
static void peers(int local_delay)
{
    wire.clear(); network_tick = packet_serial = fault_mode = 0;
    Sim sims[2]; np_rollback rb[2]; GekkoSession *sessions[2] = {};
    int submitted[2] = {-1, -1}, stalled = 0, outage_start[2] = {};
    for (int p = 0; p < 2; ++p) {
        init(rb[p], sims[p]);
        GekkoConfig config;
        CHECK(np_rollback_config(&rb[p], &config));
        CHECK(gekko_create(&sessions[p], GekkoGameSession));
        gekko_start(sessions[p], &config);
        gekko_net_adapter_set(sessions[p], &adapters[p]);
        char peer = char('0' + 1 - p);
        GekkoNetAddress address{&peer, 1};
        for (int slot = 0; slot < 2; ++slot)
            CHECK(gekko_add_actor(sessions[p], slot == p ? GekkoLocalPlayer : GekkoRemotePlayer,
                                  slot == p ? nullptr : &address) == slot);
        gekko_set_local_delay(sessions[p], p, (unsigned char)local_delay);
    }
    for (network_tick = 0; network_tick < 10000; ++network_tick) {
        if (rb[0].frame > 20 && rb[1].frame > 20) fault_mode = 1;
        for (int p = 0; p < 2; ++p) {
            if (network_tick == 400) outage_start[p] = rb[p].frame;
            if (network_tick == 439) {
                CHECK(rb[p].frame - outage_start[p] <= 16); // in-flight inputs + prediction window
                CHECK(stalled > 0);
            }
            if (rb[p].frame >= 1099) continue;
            int next = rb[p].latest_frame + 1;
            if (submitted[p] != next) {
                np_pad pad = input(next, p);
                gekko_add_local_input(sessions[p], p, &pad);
                submitted[p] = next; // freeze sampled input across stall/retry
            }
            int before = rb[p].frame;
            if (!consume(rb[p], sessions[p])) {
                std::fprintf(stderr, "peer %d tick %d: %s\n", p, network_tick, rb[p].error);
                CHECK(false);
            }
            if (fault_mode && before == rb[p].frame) ++stalled;
        }
        if (rb[0].frame == 1099 && rb[1].frame == 1099) break;
        // Gekko's handshake/retransmission timers use real monotonic time.
        if (!fault_mode || network_tick % 10 == 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    State reference;
    for (int frame = 0; frame < 1100; ++frame) {
        np_pad pads[2] = {input(frame - local_delay, 0), input(frame - local_delay, 1)};
        step(reference, pads);
    }
    for (int p = 0; p < 2; ++p) {
        CHECK(rb[p].frame == 1099 && rb[p].restores > 0 && rb[p].replayed_frames > 0);
        CHECK(rb[p].max_rewind <= 8 && rb[p].forward_frames == 1100);
        CHECK(sims[p].sounds == 1100); // no replayed sounds
        CHECK(sims[p].presents <= unsigned(network_tick + 1)); // never present intermediate replay steps
        CHECK(std::memcmp(&reference, &sims[p].state, sizeof(State)) == 0);
        std::printf("Peer %d, delay %d: %llu restores, %llu replayed, max rewind %u; reference matched.\n",
                    p, local_delay, (unsigned long long)rb[p].restores,
                    (unsigned long long)rb[p].replayed_frames, rb[p].max_rewind);
        CHECK(gekko_destroy(&sessions[p]));
    }
    CHECK(stalled > 0);
    wire.clear();
}

static void validation()
{
    Sim sim; np_rollback rb;
    CHECK(!np_rollback_init(&rb, &callbacks, &sim, 0, 8));
    CHECK(!np_rollback_init(&rb, &callbacks, &sim, sizeof(State), 0));
    CHECK(!np_rollback_init(&rb, &callbacks, &sim, sizeof(State), 17));
    init(rb, sim);
    CHECK(np_rollback_process(&rb, nullptr, 0));
    CHECK(!sim.presents && !sim.sounds);
    uint8_t snapshot[sizeof(State) + NP_ROLLBACK_STATE_HEADER]{};
    unsigned size = 0, hash = 0;
    GekkoGameEvent save_event{};
    save_event.type = GekkoSaveEvent;
    save_event.data.save.frame = -1;
    save_event.data.save.state = snapshot;
    save_event.data.save.state_len = &size;
    save_event.data.save.checksum = &hash;
    GekkoGameEvent *events[] = {&save_event};
    CHECK(np_rollback_process(&rb, events, 1));
    CHECK(size == sizeof(snapshot));
    GekkoGameEvent load_event{};
    load_event.type = GekkoLoadEvent;
    load_event.data.load.frame = -1;
    load_event.data.load.state = snapshot;
    load_event.data.load.state_len = size;
    events[0] = &load_event;
    CHECK(np_rollback_process(&rb, events, 1));
    CHECK(sim.loads == 1);
    // Corrupt every byte in the envelope and payload in isolation.
    for (unsigned byte = 0; byte < size; ++byte) {
        init(rb, sim);
        unsigned before = sim.loads;
        snapshot[byte] ^= 1;
        CHECK(!np_rollback_process(&rb, events, 1));
        CHECK(sim.loads == before); // never handed corrupt data to game
        CHECK(!np_rollback_process(&rb, nullptr, 0)); // terminal failure
        snapshot[byte] ^= 1;
    }
    for (unsigned length = 0; length < size; ++length) {
        init(rb, sim);
        load_event.data.load.state_len = length;
        CHECK(!np_rollback_process(&rb, events, 1));
    }
    load_event.data.load.state_len = size;
    init(rb, sim); sim.load_fail = true;
    CHECK(!np_rollback_process(&rb, events, 1));
    sim.load_fail = false;
    init(rb, sim); sim.save_fail = true; events[0] = &save_event;
    CHECK(!np_rollback_process(&rb, events, 1));
    CHECK(size == 0);
    sim.save_fail = false;
    init(rb, sim); save_event.data.save.portable = true;
    CHECK(!np_rollback_process(&rb, events, 1));
    GekkoGameEvent advance_event{};
    np_pad pads[2]{};
    advance_event.type = GekkoAdvanceEvent;
    advance_event.data.adv.inputs = reinterpret_cast<unsigned char *>(pads);
    advance_event.data.adv.input_len = sizeof(pads);
    events[0] = &advance_event;
    init(rb, sim); advance_event.data.adv.frame = 1;
    CHECK(!np_rollback_process(&rb, events, 1)); // skipped frame zero
    init(rb, sim); advance_event.data.adv.frame = 0; advance_event.data.adv.input_len--;
    CHECK(!np_rollback_process(&rb, events, 1));
    init(rb, sim); advance_event.data.adv.input_len++; advance_event.data.adv.running_ahead = true;
    CHECK(!np_rollback_process(&rb, events, 1));
    init(rb, sim); advance_event.data.adv.running_ahead = false; sim.step_fail = true;
    unsigned presents = sim.presents;
    CHECK(!np_rollback_process(&rb, events, 1));
    CHECK(sim.presents == presents);
    GekkoSessionEvent disconnected{};
    disconnected.type = GekkoPlayerDisconnected;
    GekkoSessionEvent *session_events[] = {&disconnected};
    init(rb, sim);
    CHECK(!np_rollback_session_events(&rb, session_events, 1));
    CHECK(!np_rollback_process(&rb, events, 1));
}

int main()
{
    static_assert(sizeof(np_pad) == 18, "Input wire layout changed");
    static_assert(sizeof(State) == 24, "Test snapshot unexpectedly contains padding");
    validation();
    stress(false);
    stress(true); // intentionally omit RNG restoration; must detect a desync
    peers(0);
    peers(2);
    std::puts("Rollback driver tests passed (fixture simulation, not live DOA3).");
}
