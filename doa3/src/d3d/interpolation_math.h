#ifndef DOA3_INTERPOLATION_MATH_H
#define DOA3_INTERPOLATION_MATH_H
#include <math.h>

/* XYZRHW is post-divide. Interpolate homogeneous coordinates so perspective
 * and depth agree with the original vertex shader at either endpoint. */
static int interpolation_position(float *out, const float *a, const float *b, float t)
{
    float wa, wb, w;
    int j;
    if (!(a[3] > 0.000001f && b[3] > 0.000001f)) return 0;
    wa = 1.0f / a[3]; wb = 1.0f / b[3];
    w = wa + (wb - wa) * t;
    if (!isfinite(w) || w <= 0.000001f) return 0;
    for (j = 0; j < 3; ++j) {
        out[j] = (a[j] * wa * (1-t) + b[j] * wb * t) / w;
        if (!isfinite(out[j])) return 0;
    }
    out[3] = 1.0f / w;
    return 1;
}

/* Only presentation is subdivided. The simulation interval remains 1/60. */
static int interpolation_steps(int hz)
{
    return hz == 120 ? 2 : hz == 240 ? 4 : 1;
}

typedef struct InterpolationBudget {
    unsigned warmup;
    double delay_ms;
    int limited;
} InterpolationBudget;
static void interpolation_budget_tick(InterpolationBudget *budget, double interval_ms)
{
    if (budget->limited) return;
    if (budget->warmup<60) { ++budget->warmup; return; }
    /* Use complete tick intervals, including pacing and catch-up. Busy time
     * alone can exceed a tick transiently while gameplay still averages 60 Hz.
     * Fall back when the schedule accumulates more than three ticks of delay. */
    budget->delay_ms += interval_ms-1000.0/60.0;
    if (budget->delay_ms<0) budget->delay_ms=0;
    if (budget->delay_ms>50.0) budget->limited=1;
}
#endif
