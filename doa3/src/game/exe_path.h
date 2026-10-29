/* 1.0.2: the folder of DOA3.exe as an ANSI path, for the *A file APIs and
 * GetPrivateProfileString/WritePrivateProfileString that the settings use.
 *
 * GetModuleFileNameA turns characters outside the system code page (a
 * Japanese, Cyrillic or accented user or folder name on an English Windows)
 * into '?', so every settings file path was wrong in such a folder and Save
 * failed. This takes the wide path, and when it does not convert exactly,
 * uses its short (8.3) form, which is plain ASCII. Returns the folder with a
 * trailing backslash, or "" if neither works. */
#ifndef DOA3_EXE_PATH_H
#define DOA3_EXE_PATH_H
#include <windows.h>
#include <string.h>
#include <wchar.h>

static void doa3_exe_dir_ansi(char *out, size_t n)
{
    WCHAR wide[MAX_PATH], shortp[MAX_PATH];
    WCHAR *slash;
    BOOL lossy = FALSE;
    out[0] = 0;
    if (!GetModuleFileNameW(NULL, wide, MAX_PATH)) return;
    slash = wcsrchr(wide, L'\\');
    if (slash) slash[1] = 0;
    if (WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, wide, -1, out, (int)n, NULL, &lossy) && !lossy)
        return;
    out[0] = 0;
    if (GetShortPathNameW(wide, shortp, MAX_PATH)) {
        lossy = FALSE;
        if (!WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, shortp, -1, out, (int)n, NULL, &lossy) || lossy)
            out[0] = 0;
    }
}

/* The folder of DOA3.exe followed by `name` (a file next to the exe). */
static void doa3_exe_file_ansi(const char *name, char *out, size_t n)
{
    doa3_exe_dir_ansi(out, n);
    if (strlen(out) + strlen(name) + 1 > n) out[0] = 0;
    strcat_s(out, n, name);
}
#endif
