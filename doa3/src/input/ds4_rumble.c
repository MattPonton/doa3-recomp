#include "ds4_rumble.h"
#include <hidsdi.h>
#include <stdlib.h>
#include <string.h>

struct Ds4Rumble {
    HANDLE device, changed, stop, thread, complete;
    volatile LONG motors, result;
};

void ds4_rumble_report(BYTE report[32], WORD left, WORD right)
{
    memset(report, 0, 32);
    report[0] = 0x05; /* DS4 USB effects report. */
    report[1] = 0x01; /* Update motors only; leave lightbar and audio alone. */
    report[4] = (BYTE)(right >> 8);
    report[5] = (BYTE)(left >> 8);
}

static DWORD send_report(Ds4Rumble *rumble, DWORD motors)
{
    BYTE report[32];
    DWORD written = 0, error;
    OVERLAPPED operation = {0};
    operation.hEvent = rumble->complete;
    ResetEvent(operation.hEvent);
    ds4_rumble_report(report, (WORD)motors, (WORD)(motors >> 16));
    if (WriteFile(rumble->device, report, sizeof(report), &written, &operation))
        return written == sizeof(report) ? ERROR_SUCCESS : ERROR_WRITE_FAULT;
    error = GetLastError();
    if (error != ERROR_IO_PENDING) return error;
    if (WaitForSingleObject(operation.hEvent, 100) != WAIT_OBJECT_0) {
        CancelIoEx(rumble->device, &operation);
        /* Finish cancellation before the stack-owned buffer can go away. */
        GetOverlappedResult(rumble->device, &operation, &written, TRUE);
        return ERROR_TIMEOUT;
    }
    if (!GetOverlappedResult(rumble->device, &operation, &written, FALSE)) return GetLastError();
    return written == sizeof(report) ? ERROR_SUCCESS : ERROR_WRITE_FAULT;
}

static DWORD WINAPI rumble_worker(void *context)
{
    Ds4Rumble *rumble = context;
    HANDLE events[] = {rumble->stop, rumble->changed};
    int sent = 0;
    for (;;) {
        DWORD event = WaitForMultipleObjects(2, events, FALSE, INFINITE);
        if (event != WAIT_OBJECT_0 + 1) break;
        DWORD motors = (DWORD)InterlockedCompareExchange(&rumble->motors, 0, 0);
        sent = 1;
        InterlockedExchange(&rumble->result, (LONG)send_report(rumble, motors));
    }
    if (sent) send_report(rumble, 0);
    return 0;
}

Ds4Rumble *ds4_rumble_open(const WCHAR *path)
{
    PHIDP_PREPARSED_DATA data = NULL;
    HIDP_CAPS caps;
    Ds4Rumble *rumble = calloc(1, sizeof(*rumble));
    if (!rumble) return NULL;
    rumble->device = CreateFileW(path, GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (rumble->device == INVALID_HANDLE_VALUE) { free(rumble); return NULL; }
    /* USB only: Bluetooth uses a different report and CRC. */
    if (!HidD_GetPreparsedData(rumble->device, &data)) goto failed;
    NTSTATUS status = HidP_GetCaps(data, &caps);
    HidD_FreePreparsedData(data);
    if (status != HIDP_STATUS_SUCCESS || caps.OutputReportByteLength != 32) goto failed;
    rumble->changed = CreateEventW(NULL, FALSE, FALSE, NULL);
    rumble->stop = CreateEventW(NULL, TRUE, FALSE, NULL);
    rumble->complete = CreateEventW(NULL, TRUE, FALSE, NULL);
    if (!rumble->changed || !rumble->stop || !rumble->complete) goto failed;
    rumble->thread = CreateThread(NULL, 0, rumble_worker, rumble, 0, NULL);
    if (!rumble->thread) goto failed;
    return rumble;
failed:
    ds4_rumble_close(rumble);
    return NULL;
}

DWORD ds4_rumble_set(Ds4Rumble *rumble, WORD left, WORD right)
{
    if (!rumble) return ERROR_NOT_SUPPORTED;
    LONG motors = (LONG)((DWORD)left | ((DWORD)right << 16));
    LONG previous = InterlockedExchange(&rumble->motors, motors);
    DWORD result = (DWORD)InterlockedCompareExchange(&rumble->result, 0, 0);
    if (previous != motors || result != ERROR_SUCCESS) SetEvent(rumble->changed);
    return result;
}

void ds4_rumble_close(Ds4Rumble *rumble)
{
    if (!rumble) return;
    if (rumble->thread) {
        SetEvent(rumble->stop);
        WaitForSingleObject(rumble->thread, INFINITE);
        CloseHandle(rumble->thread);
    }
    if (rumble->complete) CloseHandle(rumble->complete);
    if (rumble->changed) CloseHandle(rumble->changed);
    if (rumble->stop) CloseHandle(rumble->stop);
    if (rumble->device && rumble->device != INVALID_HANDLE_VALUE) CloseHandle(rumble->device);
    free(rumble);
}
