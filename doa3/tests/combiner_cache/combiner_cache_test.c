/* Exercise the production key and lookup with deterministic shader creation. */
#include <windows.h>
#include <d3d11.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "d3d8_combiners.h"
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "line %d: %s\n", __LINE__, #x); exit(1); } } while (0)
static unsigned compiled, released;
static int fail_compile;
static ID3D11PixelShader *compile_combiner_shader(const NV2ACombinerState *s)
{
    NV2ACombinerState *copy;
    if (fail_compile) return NULL;
    copy = malloc(sizeof *copy);
    CHECK(copy);
    *copy = *s;
    compiled++;
    return (ID3D11PixelShader *)copy;
}
#define ID3D11PixelShader_Release(p) (released++, free(p))
#include "cache_key.h"
#include "cache_lookup.h"
static void clear_cache(void)
{
    int i;
    for (i = 0; i < COMBINER_CACHE_SIZE; i++)
        if (g_cache[i].in_use) ID3D11PixelShader_Release(g_cache[i].shader);
    memset(g_cache, 0, sizeof g_cache);
}
int main(void)
{
    NV2ACombinerState s = {0}, changed, key;
    ID3D11PixelShader *first;
    size_t i;
    unsigned before;
    s.num_stages = 1;
    s.c0[0] = 0xff123456;
    first = d3d8_combiners_get_shader(&s);
    CHECK(first && ((NV2ACombinerState *)first)->c0[0] == s.c0[0]);
    /* Constant animation must reuse the shader; compile receives live state. */
    changed = s;
    memset(changed.c0, 0xab, sizeof changed.c0);
    memset(changed.c1, 0xcd, sizeof changed.c1);
    changed.final_c0 = 123; changed.final_c1 = 456;
    CHECK(d3d8_combiners_get_shader(&changed) == first && compiled == 1);
    /* Every structural byte, including inactive stages, stays in the key. */
    combiner_state_key(&s, &key);
    for (i = 0; i < sizeof s; i++) {
        NV2ACombinerState other_key;
        changed = s;
        ((unsigned char *)&changed)[i] ^= 0x40;
        combiner_state_key(&changed, &other_key);
        if (i >= offsetof(NV2ACombinerState, c0) && i < offsetof(NV2ACombinerState, tex_mode))
            CHECK(memcmp(&key, &other_key, sizeof key) == 0);
        else CHECK(memcmp(&key, &other_key, sizeof key) != 0);
    }
    clear_cache();
    /* Force a hash collision: exact equality must prevent a wrong shader. */
    changed = s; changed.flags = 42;
    combiner_state_key(&changed, &key);
    {
        uint32_t hash = combiner_key_hash(&key, sizeof key);
        CombinerCacheEntry *entry = &g_cache[hash & (COMBINER_CACHE_SIZE - 1)];
        entry->in_use = TRUE; entry->hash = hash;
        combiner_state_key(&s, &entry->state);
        entry->shader = compile_combiner_shader(&s);
        first = entry->shader;
        CHECK(d3d8_combiners_get_shader(&changed) != first);
        CHECK(d3d8_combiners_get_shader(&changed) != first);
    }
    clear_cache();
    /* Fill, fail a compile, then evict. Failed creation preserves live entries. */
    for (i = 0; i < COMBINER_CACHE_SIZE; i++) {
        s.flags = (DWORD)i; g_frame_counter++;
        CHECK(d3d8_combiners_get_shader(&s));
    }
    s.flags = 0; first = d3d8_combiners_get_shader(&s);
    before = released;
    fail_compile = 1; s.flags = 1000;
    CHECK(!d3d8_combiners_get_shader(&s) && released == before);
    s.flags = 0; CHECK(d3d8_combiners_get_shader(&s) == first);
    fail_compile = 0; s.flags = 1000;
    CHECK(d3d8_combiners_get_shader(&s) && released == before + 1);
    clear_cache();
    CHECK(released == compiled);
    puts("Combiner constants, structural keys, collisions, eviction and failed compilation passed.");
    return 0;
}
