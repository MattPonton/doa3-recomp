/* Exercise the production digest through its private helper, including
 * simultaneous first use of the shared provider on four threads. */
#include "../../src/d3d/d3d8_shader_cache.c"
#include <stdlib.h>
static DWORD WINAPI check_hash(void *unused)
{
    static const unsigned char expected[32]={
        0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,
        0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};
    unsigned char actual[32];
    (void)unused;
    for(int i=0;i<1000;i++)
        if(!digest("a",1,"b",1,"c",1,NULL,0,actual) || memcmp(actual,expected,32)) return 1;
    return 0;
}
int main(void)
{
    HANDLE threads[4];
    for(int i=0;i<4;i++) {
        threads[i]=CreateThread(NULL,0,check_hash,NULL,0,NULL);
        if(!threads[i]) return 1;
    }
    if(WaitForMultipleObjects(4,threads,TRUE,10000)!=WAIT_OBJECT_0) return 1;
    for(int i=0;i<4;i++) {
        DWORD status=1;
        if(!GetExitCodeThread(threads[i],&status) || status) return 1;
        CloseHandle(threads[i]);
    }
    puts("Shared SHA-256 provider: concurrent initialization and 4000 known digests pass");
    return 0;
}
