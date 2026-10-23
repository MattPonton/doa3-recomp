#ifndef DOA3_DS4_RUMBLE_H
#define DOA3_DS4_RUMBLE_H
#include <windows.h>
typedef struct Ds4Rumble Ds4Rumble;
Ds4Rumble *ds4_rumble_open(const WCHAR *path);
void ds4_rumble_close(Ds4Rumble *rumble);
DWORD ds4_rumble_set(Ds4Rumble *rumble, WORD left, WORD right);
void ds4_rumble_report(BYTE report[32], WORD left, WORD right);
void ds5_rumble_report(BYTE report[48], WORD left, WORD right);
#endif
