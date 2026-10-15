#include "perf_capture.h"
#include "perf_frame_log.h"
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
#define MEM32(address) 2u
static HWND g_hwnd;
unsigned g_doa3_frames_presented;
static unsigned opens;
static int fail_open;
int free_camera_enabled(void) { return 0; }
int doa3_movie_host_owns_screen(void) { return 0; }
FILE *doa3_diag_fopen(const char *name, const char *mode)
{
    (void)name; ++opens;
    return fail_open ? NULL : fopen("perf_lifecycle_test.csv",mode);
}
#include "frame_impl.h"
static void toggle(int on)
{
    _putenv_s("DOA3_PERF_CAPTURE",on ? "1" : "");
    doa3_perf_refresh(); doa3_perf_frame();
}
int main(void)
{
    FILE *file;
    char row[1024];
    toggle(0); doa3_perf_frame(); CHECK(!s_perf_enabled && !opens);
    toggle(1); CHECK(s_perf_enabled && opens==1);
    doa3_perf_input_sample(1); CHECK(!s_perf_input_reads);
    doa3_perf_input_sample(0); doa3_perf_logic_sample();
    CHECK(s_perf_input_reads==1 && s_perf_logic_input.QuadPart==s_perf_input.QuadPart);
    QueryPerformanceCounter(&s_perf_present); doa3_perf_frame();
    toggle(0); CHECK(!s_perf_enabled && !s_perf_input.QuadPart);
    toggle(1); CHECK(s_perf_enabled && opens==2);
    doa3_perf_frame(); toggle(0);
    file=fopen("perf_lifecycle_test.csv","r"); CHECK(file);
    CHECK(fgets(row,sizeof row,file) && !strncmp(row,"seconds,",8));
    for(int i=0;i<2;i++) {
        double seconds,ms,age,logic,present,logic_age;
        unsigned mode,reads; int movie,foreground,camera;
        CHECK(fgets(row,sizeof row,file));
        CHECK(sscanf(row,"%lf,%lf,%u,%d,%d,%d,%lf,%lf,%lf,%lf,%u",
            &seconds,&ms,&mode,&movie,&foreground,&camera,&age,&logic,&present,&logic_age,&reads)==11);
        CHECK(ms>=0 && mode==2 && reads==(i ? 0u : 1u));
        if(i) CHECK(age==-1 && logic==-1 && present==-1 && logic_age==-1);
        else CHECK(age>=0 && logic>=0 && present>=0 && logic_age>=0);
    }
    CHECK(!fgets(row,sizeof row,file)); fclose(file);
    fail_open=1; toggle(1); CHECK(!s_perf_enabled && opens==3);
    fail_open=0; doa3_perf_frame(); CHECK(!s_perf_enabled && opens==3);
    toggle(0); toggle(1); CHECK(s_perf_enabled && opens==4);
    doa3_perf_shutdown(); CHECK(!s_perf_enabled);
    remove("perf_lifecycle_test.csv");
    puts("Host capture off/on/off/on, append, timestamp reset and open-failure recovery pass");
    return 0;
}
