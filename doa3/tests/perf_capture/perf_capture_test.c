#include "perf_capture.h"
#include "perf_frame_log.h"
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)

int main(void)
{
    const char *name = "perf_capture_test.csv";
    unsigned accepted[20000], count = 0, line = 0, dropped;
    Doa3PerfFrame frame = {0};
    char row[1024];
    FILE *file;
    LONGLONG start;
    _putenv_s("DOA3_PERF_CAPTURE", "");
    CHECK(!doa3_perf_refresh() && !doa3_perf_begin());
    _putenv_s("DOA3_PERF_CAPTURE", "1");
    CHECK(doa3_perf_refresh());
    start = doa3_perf_begin(); CHECK(start > 0);
    _putenv_s("DOA3_PERF_CAPTURE", "");
    CHECK(!doa3_perf_refresh() && !doa3_perf_begin());
    CHECK(doa3_perf_now() >= start); /* A toggle cannot invalidate an endpoint. */
    CHECK(!doa3_perf_log_push(&frame));
    CHECK(!doa3_perf_log_start(NULL, 1));
    CHECK(doa3_perf_log_start(fopen(name, "w"), 1));
    frame.frame_ms=16.666667; frame.mode=2; frame.foreground=1;
    frame.input_age_ms=-1; frame.input_to_logic_ms=-1; frame.present_ms=0.25;
    frame.logic_input_age_ms=-1; frame.input_reads=3;
    for (unsigned i=0;i<20000;i++) {
        frame.seconds=i;
        if (doa3_perf_log_push(&frame)) accepted[count++]=i;
    }
    dropped=doa3_perf_log_stop();
    CHECK(count > 0 && count+dropped == 20000);
    CHECK(!doa3_perf_log_push(&frame));
    /* Stop drains all accepted rows, then restart appends without a header. */
    CHECK(doa3_perf_log_start(fopen(name,"a"),0));
    frame.seconds=20000;
    while (!doa3_perf_log_push(&frame)) SwitchToThread();
    doa3_perf_log_stop();
    file=fopen(name,"r"); CHECK(file);
    CHECK(fgets(row,sizeof row,file) && !strncmp(row,"seconds,frame_ms,",17));
    while (fgets(row,sizeof row,file)) {
        double seconds, ms, age, logic, present, logic_age;
        unsigned mode, reads; int movie, foreground, camera;
        CHECK(sscanf(row,"%lf,%lf,%u,%d,%d,%d,%lf,%lf,%lf,%lf,%u",
            &seconds,&ms,&mode,&movie,&foreground,&camera,&age,&logic,&present,&logic_age,&reads)==11);
        CHECK(line <= count && seconds == (line<count ? accepted[line] : 20000));
        CHECK(mode==2 && foreground==1 && reads==3 && age==-1 && present==0.25);
        ++line;
    }
    CHECK(line==count+1);
    fclose(file); remove(name);
    printf("Timing toggles and ordered async capture pass (%u accepted, %u dropped under load)\n",count,dropped);
    return 0;
}
