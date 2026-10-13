#include <windows.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
static char test_exe[MAX_PATH];
static DWORD test_module_name(HMODULE module, LPSTR path, DWORD size)
{
    (void)module;
    strcpy_s(path, size, test_exe);
    return (DWORD)strlen(path);
}
#define GetModuleFileNameA test_module_name
#include "../../src/game/video_settings.c"
#undef GetModuleFileNameA
HWND d3d8_GetHWND(void) { return NULL; }
ptrdiff_t g_xbox_mem_offset;
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); result=1; goto cleanup; } } while(0)
#define WORD_AT(va) (*(uint32_t *)(memory + (va)))
int main(void)
{
    static const unsigned widths[] = {640,640,854,1120,1707};
    static const uint32_t bits[] = {0x3f400000,0x3f100000,0x3f100000,0x3edb6db7,0x3e900000};
    static const float display[] = {4.f/3.f,4.f/3.f,16.f/9.f,21.f/9.f,32.f/9.f};
    char temporary[MAX_PATH], directory[MAX_PATH];
    const char *ini = NULL;
    uint8_t *memory = NULL;
    int result = 0;
    unsigned w,h;
    if (!GetTempPathA(sizeof temporary, temporary) ||
        !GetTempFileNameA(temporary,"dar",0,directory)) return 1;
    DeleteFileA(directory);
    if (!CreateDirectoryA(directory,NULL)) return 1;
    snprintf(test_exe,sizeof test_exe,"%s\\DOA3.exe",directory);
    ini = doa3_settings_ini_path();
    CHECK(!video_settings_load() && video_get_aspect()==VIDEO_ASPECT_16_9);
    video_sync_guest_widescreen(); /* Safe before the Xbox image is mapped. */
    memory = calloc(1,0xA49000);
    CHECK(memory);
    g_xbox_mem_offset = (ptrdiff_t)memory;
    /* Startup patches the constant but does not manufacture uninitialized flags. */
    video_set_aspect(VIDEO_ASPECT_21_9);
    video_sync_guest_widescreen();
    CHECK(WORD_AT(0x251620)==bits[3]);
    CHECK(WORD_AT(0xA48CDC)==0 && WORD_AT(0xA48C58)==0);
    WORD_AT(0xA48C30)=720;
    WORD_AT(0x25161C)=0x12345678;
    WORD_AT(0x251624)=0x87654321;
    CHECK(WritePrivateProfileStringA("Audio","MasterVolume","37",ini));
    for (int aspect=0;aspect<VIDEO_ASPECT_COUNT;++aspect) {
        video_set_aspect(aspect);
        CHECK(fabsf(video_display_aspect(aspect)-display[aspect])<1e-6f);
        for (int scale=1;scale<=3;++scale) {
            video_set_scale(scale);
            video_guest_target_size(aspect,&w,&h);
            CHECK(w==widths[aspect]*scale && h==480*scale);
            float hud_x, hud_w;
            video_hud_region(w,&hud_x,&hud_w);
            CHECK(fabsf(hud_x*2+hud_w-(float)w)<0.001f);
            if (aspect<=VIDEO_ASPECT_16_9) CHECK(hud_x==0 && hud_w==(float)w);
            else CHECK(fabsf(hud_w/(float)w*display[aspect]-16.f/9.f)<1e-6f);
        }
        WORD_AT(0xA48C58)=0x55; WORD_AT(0xA48C78)=0xA5; WORD_AT(0xA48CDC)=0x55;
        video_sync_guest_widescreen();
        CHECK(WORD_AT(0x251620)==bits[aspect]);
        CHECK(WORD_AT(0x25161C)==0x12345678 && WORD_AT(0x251624)==0x87654321);
        CHECK(WORD_AT(0xA48C58)==(aspect==0 ? 0x45 : 0x55));
        CHECK(WORD_AT(0xA48C78)==(aspect==0 ? 0xA4 : 0xA5));
        CHECK(WORD_AT(0xA48CDC)==(aspect==0 ? 0x54 : 0x55));
        CHECK(video_xc_video_value()==(aspect==0 ? 0 : 0x10000));
        CHECK(video_settings_save());
        video_set_aspect(VIDEO_ASPECT_16_9);
        CHECK(video_settings_load() && video_get_aspect()==aspect);
        CHECK(GetPrivateProfileIntA("Audio","MasterVolume",0,ini)==37);
    }
    /* The two 4:3 modes switch projection without a render-target resize. */
    video_set_aspect(VIDEO_ASPECT_4_3_FULL); video_sync_guest_widescreen();
    video_guest_target_size(video_get_aspect(),&w,&h);
    CHECK(w==1920 && h==1440 && WORD_AT(0x251620)==bits[0]);
    video_set_aspect(VIDEO_ASPECT_4_3_ANAMORPHIC); video_sync_guest_widescreen();
    video_guest_target_size(video_get_aspect(),&w,&h);
    CHECK(w==1920 && h==1440 && WORD_AT(0x251620)==bits[1]);
    CHECK(WritePrivateProfileStringA("Video","AspectRatio",NULL,ini));
    CHECK(WritePrivateProfileStringA("Video","Widescreen","0",ini));
    CHECK(video_settings_load() && video_get_aspect()==VIDEO_ASPECT_4_3_ANAMORPHIC);
    CHECK(WritePrivateProfileStringA("Video","Widescreen","1",ini));
    CHECK(video_settings_load() && video_get_aspect()==VIDEO_ASPECT_16_9);
    CHECK(WritePrivateProfileStringA("Video","AspectRatio","999",ini));
    CHECK(video_settings_load() && video_get_aspect()==VIDEO_ASPECT_16_9);
    video_set_aspect(-1); CHECK(video_get_aspect()==VIDEO_ASPECT_16_9);
    video_set_aspect(VIDEO_ASPECT_COUNT); CHECK(video_get_aspect()==VIDEO_ASPECT_16_9);
cleanup:
    g_xbox_mem_offset=0;
    free(memory);
    if (ini) { WritePrivateProfileStringA(NULL,NULL,NULL,ini); DeleteFileA(ini); }
    RemoveDirectoryA(directory);
    if (!result) puts("Five aspect modes, projection bytes, live 4:3 switch, flags, sizes and persistence passed.");
    return result;
}
