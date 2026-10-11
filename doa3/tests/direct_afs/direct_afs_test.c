#include <windows.h>
#define RECOMP_GENERATED_CODE
#include "recomp/recomp_types.h"
#include <stdio.h>
#include <stdlib.h>
#define CHECK(c) do { if (!(c)) { fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #c); exit(1); } } while (0)
ptrdiff_t g_xbox_mem_offset;
uint32_t g_eax, g_ecx, g_edx, g_ebx, g_esi, g_edi, g_esp, g_seh_ebp;
static unsigned mounts, rejected_cache, backend;
void sub_000A4300(void);
void sub_00191F00(void) { esp += 4; }
void sub_00191D20(void) { eax = ++backend; esp += 4; }
void sub_00191D40(void)
{
    unsigned path = MEM32(esp + 8);
    if (path == 0x2D4F78 || path == 0x2D4F88) { esp += 4; return; }
    CHECK(path == 0x2D5000 || path == 0x2D5010 || path == 0x2D501C);
    rejected_cache++;
    esp += 4;
}
void sub_00191F70(void) { esp += 4; }
void sub_00192870(void) { esp += 4; }
void sub_001924E0(void) { esp += 4; }
void sub_00191F80(void) { esp += 4; }
void sub_00190EF0(void)
{
    unsigned partition = MEM32(esp + 4), path = MEM32(esp + 8);
    CHECK(partition == mounts + 1);
    CHECK(path == (partition == 1 ? 0x2D502C : partition == 2 ? 0x2D503C : 0x2D5048));
    CHECK(strcmp((const char *)XBOX_PTR(path), partition == 1 ? "d:\\loadfile.afs" :
                 partition == 2 ? "d:\\bgm.afs" : "d:\\voice.afs") == 0);
    mounts++; esp += 4;
}
void sub_00190B00(void) { eax = 3; esp += 4; }
void sub_001916E0(void) { esp += 4; }
void sub_001C7370(void) { esp += 4; }
void sub_001C73F0(void) { CHECK(0); } /* Must never spawn a cache-copy thread. */
#include "guest.h"

int main(void)
{
    unsigned char *memory = VirtualAlloc((void *)0x20000000, 0xD00000,
                                         MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    CHECK(memory);
    g_xbox_mem_offset = (ptrdiff_t)memory;
    const unsigned old_states[] = {0, 1, 0xFFFFFFFFu};
    for (unsigned i = 0; i < 3; i++) {
        memset(memory, 0, 0xD00000);
        strcpy((char *)XBOX_PTR(0x2D502C), "d:\\loadfile.afs");
        strcpy((char *)XBOX_PTR(0x2D503C), "d:\\bgm.afs");
        strcpy((char *)XBOX_PTR(0x2D5048), "d:\\voice.afs");
        MEM32(0x73C0A4) = old_states[i];
        mounts = rejected_cache = backend = 0;
        esp = 0xD00000; esi = 0x1234;
        PUSH32(esp, 0); /* initializer argument */
        PUSH32(esp, 0);
        sub_001C79F0();
        CHECK(esp == 0xD00000 - 4 && esi == 0x1234);
        CHECK(mounts == 3 && rejected_cache == 3 && backend == 3);
        CHECK(MEM32(0x73C0A4) == 0 && MEM32(0xC59248) == 0);
        ebx = 0; esp -= 0x18;
        start_copy_if_requested();
        CHECK(esp == 0xD00000 - 4);
    }
    VirtualFree(memory, 0, MEM_RELEASE);
    puts("Direct AFS: all three source archives mounted; cache copies excluded; installer skipped; stack preserved");
    return 0;
}
