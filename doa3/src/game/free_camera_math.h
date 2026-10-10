#ifndef DOA3_FREE_CAMERA_MATH_H
#define DOA3_FREE_CAMERA_MATH_H
#include <math.h>
#include <string.h>

typedef struct FreeCameraPose { float eye[3], yaw, pitch; } FreeCameraPose;

static int free_camera_pose_from_view(FreeCameraPose *p, const float eye[3], const float target[3])
{
    float d[3], length;
    for (int i = 0; i < 3; ++i) {
        if (!isfinite(eye[i]) || !isfinite(target[i]) || fabsf(eye[i]) > 1e7f) return 0;
        d[i] = target[i] - eye[i];
    }
    length = sqrtf(d[0]*d[0] + d[1]*d[1] + d[2]*d[2]);
    if (!isfinite(length) || length < 0.001f) return 0;
    memcpy(p->eye, eye, sizeof(p->eye));
    p->yaw = atan2f(d[0], -d[2]);
    p->pitch = asinf(fmaxf(-1, fminf(1, d[1] / length)));
    return 1;
}

static void free_camera_forward(const FreeCameraPose *p, float f[3])
{
    f[0] = sinf(p->yaw) * cosf(p->pitch);
    f[1] = sinf(p->pitch);
    f[2] = -cosf(p->yaw) * cosf(p->pitch);
}

/* Radial dead zone: no diagonal speed advantage, full travel still reaches 1. */
static void free_camera_stick(short sx, short sy, float dead, float *x, float *y)
{
    float fx = sx / 32768.0f, fy = sy / 32768.0f;
    float len = sqrtf(fx*fx + fy*fy);
    *x = *y = 0;
    if (len > dead) {
        float scale = (fminf(len, 1) - dead) / ((1 - dead) * len);
        *x = fx * scale; *y = fy * scale;
    }
}

static void free_camera_step(FreeCameraPose *p, float dt, float speed,
                             float right, float forward, float up,
                             float yaw_delta, float pitch_delta)
{
    float f[3], v[3], length;
    dt = fmaxf(0, fminf(dt, 0.05f));
    p->yaw = remainderf(p->yaw + yaw_delta, 6.28318530718f);
    p->pitch = fmaxf(-1.553343f, fminf(1.553343f, p->pitch + pitch_delta));
    free_camera_forward(p, f);
    v[0] = f[0] * forward + cosf(p->yaw) * right;
    v[1] = f[1] * forward + up;
    v[2] = f[2] * forward + sinf(p->yaw) * right;
    length = sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (length > 1) for (int i = 0; i < 3; ++i) v[i] /= length;
    for (int i = 0; i < 3; ++i) p->eye[i] += v[i] * speed * dt;
}
#endif
