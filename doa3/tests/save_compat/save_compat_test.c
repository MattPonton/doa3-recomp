#include "save_compat.h"
#include "kernel.h"
#include "xbox_memory_layout.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fixtures.h"
ptrdiff_t g_xbox_mem_offset;
UCHAR xbox_SignatureKey[16];
void xbox_log(int level, const char *subsystem, const char *fmt, ...) { (void)level; (void)subsystem; (void)fmt; }
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
static void put32(unsigned char *buffer, size_t offset, unsigned value) { memcpy(buffer+offset,&value,4); }
static unsigned char *read_file(const char *path, size_t *size)
{
    FILE *f = fopen(path,"rb"); CHECK(f);
    CHECK(!fseek(f,0,SEEK_END)); long length=ftell(f); CHECK(length>0 && length<0x1000000);
    rewind(f); *size=(size_t)length;
    unsigned char *data=malloc(*size); CHECK(data && fread(data,1,*size,f)==*size);
    fclose(f); return data;
}
int main(int argc, char **argv)
{
    unsigned char *memory=calloc(1,0xd00000); CHECK(memory);
    g_xbox_mem_offset=(ptrdiff_t)memory;
    if (argc==3) {
        size_t xbe_size,save_size;
        unsigned char *xbe=read_file(argv[1],&xbe_size), *save=read_file(argv[2],&save_size);
        unsigned char *before=malloc(save_size); CHECK(before); memcpy(before,save,save_size);
        CHECK(doa3_save_keys_init(xbe,xbe_size));
        CHECK(doa3_save_accept_compatible(save,save_size));
        CHECK(!memcmp(before,save,save_size));
        /* Round trip through the same HMAC used by the game's save writer. */
        unsigned char digest[20];
        xbox_XcHMAC(xbox_SignatureKey,16,save,0x3c34,NULL,0,digest);
        memcpy(save+0x3c34,digest,20);
        CHECK(doa3_save_accept_compatible(save,save_size));
        puts("Supplied save accepted and re-signed/revalidated in memory; source file untouched.");
        free(before); free(save); free(xbe); free(memory); return 0;
    }
    unsigned char header[0x400]={0}, save[0x3c48], before[sizeof(save)];
    memcpy(header,"XBEH",4); put32(header,0x104,0x10000); put32(header,0x118,0x10200);
    put32(header,0x200,0x1d0); put32(header,0x208,0x54430001);
    for (unsigned i=0;i<16;++i) header[0x2c0+i]=(unsigned char)i;
    CHECK(doa3_save_keys_init(header,sizeof(header)));
    CHECK(!memcmp(xbox_SignatureKey,retail_key,16));
    CHECK(!memcmp(memory+XBOX_KERNEL_DATA_BASE+KDATA_SIGNATURE_KEY,retail_key,16));
    const unsigned char *signatures[]={signature_0,signature_1,signature_2};
    for (int kind=0;kind<3;++kind) {
        CHECK(doa3_save_keys_init(header,sizeof(header)));
        for (unsigned i=0;i<0x3c34;++i) save[i]=(unsigned char)(i*17+3);
        memcpy(save+0x3bfa,"DEAD OR ALIVE 3\0",16);
        memcpy(save+0x3c34,signatures[kind],20); memcpy(before,save,sizeof(save));
        CHECK(doa3_save_accept_compatible(save,sizeof(save)));
        CHECK(!memcmp(save,before,sizeof(save)));
        CHECK(!memcmp(xbox_SignatureKey,kind==1 ? derived_key : retail_key,16));
        CHECK(!doa3_save_accept_compatible(save,sizeof(save)-1));
        save[123]^=1; CHECK(!doa3_save_accept_compatible(save,sizeof(save))); save[123]^=1;
        save[0x3c34]^=1; CHECK(!doa3_save_accept_compatible(save,sizeof(save))); save[0x3c34]^=1;
        save[0x3bfa]^=1; CHECK(!doa3_save_accept_compatible(save,sizeof(save)));
    }
    CHECK(!doa3_save_keys_init(header,0x11b));
    put32(header,0x118,0xffffffff); CHECK(!doa3_save_keys_init(header,sizeof(header)));
    CHECK(!doa3_save_accept_compatible(before,sizeof(before)));
    free(memory);
    puts("Title-key derivation, both imported signatures, legacy migration, corruption rejection and bounds passed.");
    return 0;
}
