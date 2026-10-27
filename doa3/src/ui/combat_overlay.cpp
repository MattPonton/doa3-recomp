#include "combat_overlay.h"
#include "combat_world.h"
#include "imgui.h"
#include "../game/combat_debug.h"
#include "../game/video_settings.h"
#include "../online/netplay_session.h"
#include <cmath>

void DrawCombatDebugMenu()
{
    if (!ImGui::CollapsingHeader("Fight information")) return;
    unsigned flags = combat_debug_flags();
    bool hit = (flags & COMBAT_HITBOXES) != 0;
    bool hurt = (flags & COMBAT_HURTBOXES) != 0;
    bool frames = (flags & COMBAT_FRAMES) != 0;   /* kept as is; toggled from Sparring */
    bool slippery = (flags & COMBAT_SLIPPERY_GROUND) != 0;
    bool special = (flags & COMBAT_SPECIAL_BOUNDARIES) != 0;
    bool walls = (flags & COMBAT_WALLS) != 0;
    bool objects = (flags & COMBAT_STAGE_OBJECTS) != 0;
    ImGui::BeginDisabled(netplay_active() != 0);
    bool changed = ImGui::Checkbox("Show hitboxes", &hit);
    changed |= ImGui::Checkbox("Show hurtboxes", &hurt);
    if (ImGui::TreeNode("Stage hazards")) {
        changed |= ImGui::Checkbox("Slippery ground (blue)", &slippery);
        changed |= ImGui::Checkbox("Special boundaries (purple)", &special);
        ImGui::TextDisabled("Ledges, breakable barriers, and stage reactions");
        changed |= ImGui::Checkbox("Ordinary walls (orange)", &walls);
        changed |= ImGui::Checkbox("Collidable objects (orange)", &objects);
        ImGui::TreePop();
    }
    if (changed) combat_debug_set_flags((hit ? COMBAT_HITBOXES : 0) |
        (hurt ? COMBAT_HURTBOXES : 0) | (frames ? COMBAT_FRAMES : 0) |
        (slippery ? COMBAT_SLIPPERY_GROUND : 0) |
        (special ? COMBAT_SPECIAL_BOUNDARIES : 0) |
        (walls ? COMBAT_WALLS : 0) | (objects ? COMBAT_STAGE_OBJECTS : 0));
    ImGui::EndDisabled();
    ImGui::TextDisabled("Color legend (?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip)) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 30.0f);
        ImGui::TextUnformatted(
            "Red/pink solid spheres: hitboxes. Green/cyan wireframes: hurtboxes. Amber: an attack already "
            "registered contact. Volumes draw through characters and scenery.\n\n"
            "Hazards: blue ground shows slippery surfaces. Purple marks special boundaries used by ledges, "
            "breakable barriers, and other stage reactions; their behavior depends on the stage and floor "
            "level. Orange shows other boundaries and collidable objects, including ordinary walls. Boundary "
            "panels have a 3m display height; object volumes use the game's collision dimensions. Hazards "
            "respect scene depth, so characters and foreground scenery hide them.\n\n"
            "Session only. Unavailable during online play.");
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
    if (*combat_world_error()) ImGui::TextWrapped("%s", combat_world_error());
}

/* 0.2.18: the frame-data display on its own, for the Sparring tab. */
void DrawMoveInfoToggle()
{
    unsigned flags = combat_debug_flags();
    bool frames = (flags & COMBAT_FRAMES) != 0;
    ImGui::BeginDisabled(netplay_active() != 0);
    if (ImGui::Checkbox("Show Move Info", &frames))
        combat_debug_set_flags(frames ? (flags | COMBAT_FRAMES) : (flags & ~COMBAT_FRAMES));
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip)) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 30.0f);
        ImGui::TextUnformatted(
            "Shows each attack's base startup (first active frame), active duration and recovery alongside "
            "the observed strike frames. The timeline uses blue for startup, red for active, yellow for "
            "recovery, and gray for missing samples. Cancels shorten the observation, not the base timing. "
            "Hit/block advantage is not yet measured. Session only.");
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

namespace {
struct View {
    const float *matrix;
    ImDrawList *draw;
    float x, y, w, h;
    void line(const float a[3], const float b[3], ImU32 color) const {
        float p[4];
        if (!combat_debug_project_line(matrix, a, b, p)) return;
        draw->AddLine(ImVec2(x+(p[0]+1)*w*.5f, y+(1-p[1])*h*.5f),
                      ImVec2(x+(p[2]+1)*w*.5f, y+(1-p[3])*h*.5f), color, 1.5f);
    }
    void triangle(const float a[3], const float b[3], const float c[3], ImU32 color) const {
        float polygon[12][4], clipped[12][4];
        const float *vertices[3] = {a,b,c};
        int count = 3;
        for (int i=0; i<3; ++i) {
            for (int j=0; j<4; ++j) {
                polygon[i][j] = vertices[i][0]*matrix[j] + vertices[i][1]*matrix[4+j]
                    + vertices[i][2]*matrix[8+j] + matrix[12+j];
                if (!std::isfinite(polygon[i][j])) return;
            }
        }
        // Clip surfaces before division so flying through a hitbox cannot
        // produce inverted or enormous triangles at the near plane.
        for (int plane=0; plane<7 && count; ++plane) {
            auto distance = [plane](const float *p) {
                switch (plane) {
                case 0: return p[3]-.0001f;
                case 1: return p[2];
                case 2: return p[3]-p[2];
                case 3: return p[3]+p[0];
                case 4: return p[3]-p[0];
                case 5: return p[3]+p[1];
                default: return p[3]-p[1];
                }
            };
            int next = 0;
            for (int i=0; i<count; ++i) {
                const float *p=polygon[i], *q=polygon[(i+1)%count];
                float dp=distance(p), dq=distance(q);
                if (dp >= 0) {
                    for (int j=0; j<4; ++j) clipped[next][j]=p[j];
                    ++next;
                }
                if ((dp < 0) != (dq < 0)) {
                    float t=dp/(dp-dq);
                    for (int j=0; j<4; ++j) clipped[next][j]=p[j]+t*(q[j]-p[j]);
                    ++next;
                }
            }
            count=next;
            for (int i=0; i<count; ++i)
                for (int j=0; j<4; ++j) polygon[i][j]=clipped[i][j];
        }
        if (count < 3) return;
        ImVec2 points[12];
        for (int i=0; i<count; ++i)
            points[i]=ImVec2(x+(polygon[i][0]/polygon[i][3]+1)*w*.5f,
                            y+(1-polygon[i][1]/polygon[i][3])*h*.5f);
        draw->AddConvexPolyFilled(points,count,color);
    }
};
void solid_sphere(const View &view, const CombatVolume &v, ImU32 color)
{
    constexpr int slices=24, rings=12;
    float points[rings+1][slices][3];
    for (int i=0; i<=rings; ++i) {
        float latitude=i*3.14159265359f/rings;
        for (int j=0; j<slices; ++j) {
            float longitude=j*6.28318530718f/slices;
            points[i][j][0]=v.a[0]+v.radius*std::sin(latitude)*std::cos(longitude);
            points[i][j][1]=v.a[1]+v.radius*std::cos(latitude);
            points[i][j][2]=v.a[2]+v.radius*std::sin(latitude)*std::sin(longitude);
        }
    }
    // Both surfaces contribute a light tint; no mesh-edge antialiasing,
    // which would leave dark seams between the translucent triangles.
    ImDrawListFlags old_flags=view.draw->Flags;
    view.draw->Flags &= ~ImDrawListFlags_AntiAliasedFill;
    color=(color & ~IM_COL32_A_MASK) | (80u << IM_COL32_A_SHIFT);
    for (int i=0; i<rings; ++i) for (int j=0; j<slices; ++j) {
        int next=(j+1)%slices;
        if (i) view.triangle(points[i][j],points[i+1][j],points[i][next],color);
        if (i+1<rings) view.triangle(points[i][next],points[i+1][j],points[i+1][next],color);
    }
    view.draw->Flags=old_flags;
}
void circle(const View &view, const float center[3], const float u[3],
            const float v[3], float radius, ImU32 color)
{
    float previous[3];
    for (int j=0; j<3; ++j) previous[j] = center[j] + radius*u[j];
    for (int i=1; i<=24; ++i) {
        float angle = i*(6.28318530718f/24), next[3];
        for (int j=0; j<3; ++j)
            next[j] = center[j] + radius*(u[j]*std::cos(angle) + v[j]*std::sin(angle));
        view.line(previous, next, color);
        for (int j=0; j<3; ++j) previous[j] = next[j];
    }
}
void shape(const View &view, const CombatVolume &v)
{
    ImU32 color = v.attack ? (v.side ? IM_COL32(255,85,170,240) : IM_COL32(255,70,55,240))
                          : (v.side ? IM_COL32(60,210,255,185) : IM_COL32(60,255,125,185));
    if (v.spent) color = IM_COL32(255,190,55,200);
    if (v.attack && !v.cylinder) {
        solid_sphere(view,v,color);
        return;
    }
    if (!v.cylinder) {
        const float axes[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
        for (int i=0; i<3; ++i) circle(view, v.a, axes[i], axes[(i+1)%3], v.radius, color);
        return;
    }
    float d[3], u[3], w[3];
    float length = 0;
    for (int j=0; j<3; ++j) { d[j]=v.b[j]-v.a[j]; length+=d[j]*d[j]; }
    if (length < 1e-10f) return;
    length = std::sqrt(length);
    for (float &x : d) x /= length;
    // Cross with the least parallel of world up and world right.
    if (std::fabs(d[1]) < .9f) { u[0]=-d[2]; u[1]=0; u[2]=d[0]; }
    else { u[0]=0; u[1]=d[2]; u[2]=-d[1]; }
    length=std::sqrt(u[0]*u[0]+u[1]*u[1]+u[2]*u[2]);
    for (float &x : u) x /= length;
    w[0]=d[1]*u[2]-d[2]*u[1]; w[1]=d[2]*u[0]-d[0]*u[2]; w[2]=d[0]*u[1]-d[1]*u[0];
    circle(view, v.a, u, w, v.radius, color);
    circle(view, v.b, u, w, v.radius, color);
    for (int i=0; i<8; ++i) {
        float a[3], b[3], angle=i*6.28318530718f/8;
        for (int j=0; j<3; ++j) {
            float offset=v.radius*(u[j]*std::cos(angle)+w[j]*std::sin(angle));
            a[j]=v.a[j]+offset; b[j]=v.b[j]+offset;
        }
        view.line(a,b,color);
    }
}
void timing(const char *label, const CombatTiming &t)
{
    if (!t.valid) { ImGui::TextDisabled("%s: waiting for a strike", label); return; }
    ImGui::Text("%s: move %04X%s%s", label, t.move,
        t.interrupted ? " (cancelled/interrupted)" : "", t.partial ? " (partial)" : "");
    if (t.base_valid)
        ImGui::Text("Base S / A / R: %u / %u / %u",
            t.base_startup + 1, t.base_active, t.base_recovery);
    else ImGui::TextDisabled("Base S / A / R: unavailable");
    if (t.first_active) {
        ImGui::Text("Observed first / last active: %u / %u", t.first_active, t.last_active);
        ImGui::Text("Active: %u (%u windows)   Recovery: %u",
            t.active_frames, t.active_windows, t.recovery_frames);
    } else ImGui::Text("Elapsed: %u   No active frames yet", t.frame);
    ImGui::TextDisabled("Observed through frame %u%s", t.frame, t.ended ? "" : " (live)");
    const ImU32 colors[] = {IM_COL32(80,150,245,255), IM_COL32(245,75,80,255),
        IM_COL32(240,200,65,255), IM_COL32(90,90,100,255)};
    ImVec2 p = ImGui::GetCursorScreenPos();
    const float width = 320.f, height = 10.f;
    unsigned count = t.frame < COMBAT_TIMELINE_FRAMES ? t.frame : COMBAT_TIMELINE_FRAMES;
    unsigned extent = count < 60 ? 60 : count;
    float step = width / extent;
    ImDrawList *draw = ImGui::GetWindowDrawList();
    draw->AddRectFilled(p, ImVec2(p.x+width,p.y+height), IM_COL32(35,35,40,255));
    for (unsigned i=0; i<count; ++i) {
        unsigned phase = t.timeline[i] < COMBAT_UNKNOWN ? t.timeline[i] : COMBAT_UNKNOWN;
        draw->AddRectFilled(ImVec2(p.x+i*step,p.y), ImVec2(p.x+(i+1)*step,p.y+height), colors[phase]);
    }
    for (unsigned i=10; i<extent; i+=10)
        draw->AddLine(ImVec2(p.x+i*step,p.y), ImVec2(p.x+i*step,p.y+height), IM_COL32(0,0,0,100));
    ImGui::Dummy(ImVec2(width,height));
    if (t.frame > COMBAT_TIMELINE_FRAMES)
        ImGui::TextDisabled("Timeline: first %u frames", COMBAT_TIMELINE_FRAMES);
}
}

void DrawCombatDebugOverlay()
{
    unsigned flags = combat_debug_flags();
    if (!flags) return;
    CombatSnapshot snapshot;
    if (!combat_debug_snapshot(&snapshot)) return;
    ImVec2 display = ImGui::GetIO().DisplaySize;
    if (display.x <= 0 || display.y <= 0) return;
    float aspect = video_display_aspect(video_get_aspect());
    float w = display.x, h = display.y;
    if (w/h > aspect) w=h*aspect; else h=w/aspect;
    View view = {snapshot.matrix, ImGui::GetBackgroundDrawList(),
        std::floor((display.x-w)*.5f), std::floor((display.y-h)*.5f), w, h};
    view.draw->PushClipRect(ImVec2(view.x,view.y), ImVec2(view.x+w,view.y+h), true);
    if ((flags & COMBAT_HAZARDS) && snapshot.hazards_truncated)
        view.draw->AddText(ImVec2(view.x+12,view.y+12), IM_COL32(255,190,80,255),
            "Stage hazard display limit reached");
    // Fill attacks first so body wireframes remain legible where they overlap.
    for (int attack=1; attack>=0; --attack)
        for (unsigned i=0; i<snapshot.count; ++i) {
            const CombatVolume &v = snapshot.volumes[i];
            if (v.attack == attack && (flags & (attack ? COMBAT_HITBOXES : COMBAT_HURTBOXES)))
                shape(view,v);
        }
    view.draw->PopClipRect();
    if (!(flags & COMBAT_FRAMES)) return;
    ImGui::SetNextWindowPos(ImVec2(view.x+w-12,view.y+h-204), ImGuiCond_Always, ImVec2(1,1));
    ImGui::SetNextWindowBgAlpha(.8f);
    if (ImGui::Begin("Attack frame data", nullptr, ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing)) {
        if (ImGui::BeginTable("Combat sides",2, ImGuiTableFlags_SizingFixedFit)) {
            for (unsigned side=0; side<2; ++side) {
                ImGui::TableNextColumn();
                const CombatFighter &f=snapshot.fighter[side];
                ImGui::Text("P%u   Move %04X   Frame %u",side+1,f.move,f.frame);
                ImGui::Text("%s   HP %u%s", !f.current.valid ? "Neutral / other" :
                    (f.phase == COMBAT_ACTIVE ? "Active" : f.phase == COMBAT_RECOVERY ?
                    "Recovery" : f.phase == COMBAT_STARTUP ? "Startup" : "Unknown phase"),
                    f.health, f.contact ? "   Contact" : "");
                timing("Current",f.current);
                ImGui::Separator();
                timing("Previous",f.previous);
                ImGui::Separator();
                const CombatExchange &e = snapshot.exchange[side];
                if (e.status == COMBAT_EXCHANGE_NONE) {
                    ImGui::TextDisabled("Hit / block: waiting for contact");
                } else {
                    ImGui::Text("Last contact: move %04X", e.move);
                    const char *label = e.kind == COMBAT_CONTACT_BLOCK ?
                        "Block advantage" : "Hit recovery gap";
                    if (e.status == COMBAT_EXCHANGE_COMPLETE) {
                        ImVec4 color = e.advantage > 0 ? ImVec4(.4f,1.f,.55f,1.f) :
                            e.advantage < 0 ? ImVec4(1.f,.5f,.4f,1.f) : ImVec4(1.f,1.f,1.f,1.f);
                        ImGui::TextColored(color, "%s: %+d f", label, e.advantage);
                    } else if (e.status == COMBAT_EXCHANGE_PENDING) {
                        ImGui::TextDisabled("%s: measuring...", label);
                    } else {
                        ImGui::TextDisabled("Recovery: interrupted / unsupported");
                    }
                }
            }
            ImGui::EndTable();
        }
        ImGui::TextDisabled("S = first active frame, A/R = active/recovery duration. Base timing excludes cancels.");
        ImGui::TextDisabled("Timeline: blue startup / red active / yellow recovery / gray missing. Ticks = 10 frames.");
        ImGui::TextDisabled("+ means attacker recovers first. Natural recovery only; holds/cancels/stun escapes excluded.");
    }
    ImGui::End();
}
