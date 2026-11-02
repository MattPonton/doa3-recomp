/* Stand-ins for the QEMU headers xemu's DSP core includes (offline tool only). */
#pragma once
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif
#define g_new0(type, n) ((type *)calloc((n), sizeof(type)))
#define g_free free
#ifdef _MSC_VER
#include <basetsd.h>
typedef SSIZE_T ssize_t;
#endif
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
