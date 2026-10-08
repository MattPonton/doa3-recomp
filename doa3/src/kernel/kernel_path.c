/*
 * kernel_path.c - Xbox→Windows Path Translation
 *
 * The console's drives live in an "HDD" folder next to the exe, one folder
 * per partition, the way Cxbx-Reloaded lays out its emulated hard disk:
 *
 *   HDD\D\                       D:, \Device\CdRom0   the ripped disc (XISO)
 *   HDD\C\                       C:, Partition2        system
 *   HDD\E\                       E:, Partition1        data
 *   HDD\E\TDATA\<title id>\       T:                   title data
 *   HDD\E\UDATA\<title id>\       U:                   saved games
 *   HDD\X\  HDD\Y\  HDD\Z\          X: Y: Z:, Partition3-5  cache partitions
 *   HDD\F\  HDD\G\                 F: G:, Partition6-7   extended partitions
 *
 * <title id> is the XBE's (DOA3_TITLE_ID), lower-case hex, so a save from a
 * real console or from Cxbx-R can be dropped straight into UDATA. The game
 * maps its cache drive Z: itself; it lands in HDD\Z\ and is filled by the
 * game's own first-boot install.
 */

#include "kernel.h"
#include <stdio.h>
#include <string.h>

/* Base directories - set at init */
static WCHAR s_game_dir[MAX_PATH];    /* D: (the disc) */
static WCHAR s_hdd_dir[MAX_PATH];     /* HDD root: one folder per partition */
static BOOL  s_initialized = FALSE;
static volatile LONG s_title_redirect = 0;   /* see xbox_path_set_title_redirect */

static void copy_dir_files(const WCHAR *from, const WCHAR *to)
{
    WCHAR pat[MAX_PATH], src[MAX_PATH], dst[MAX_PATH];
    WIN32_FIND_DATAW fd;
    HANDLE h;
    CreateDirectoryW(to, NULL);
    swprintf_s(pat, MAX_PATH, L"%s\\*", from);
    h = FindFirstFileW(pat, &fd);
    if (h == INVALID_HANDLE_VALUE) return;
    do {
        if (!wcscmp(fd.cFileName, L".") || !wcscmp(fd.cFileName, L"..")) continue;
        swprintf_s(src, MAX_PATH, L"%s\\%s", from, fd.cFileName);
        swprintf_s(dst, MAX_PATH, L"%s\\%s", to, fd.cFileName);
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) copy_dir_files(src, dst);
        else CopyFileW(src, dst, FALSE);
    } while (FindNextFileW(h, &fd));
    FindClose(h);
}

static void tdata_dir(WCHAR *out, int netplay)
{
    swprintf_s(out, MAX_PATH, L"%s\\E\\TDATA\\%08x%s", s_hdd_dir,
               (unsigned)DOA3_TITLE_ID, netplay ? L"_netplay" : L"");
}

/* Create every directory along `dir` (absolute path). */
static void make_dirs(const WCHAR *dir)
{
    WCHAR tmp[MAX_PATH];
    size_t i, n;
    wcscpy_s(tmp, MAX_PATH, dir);
    n = wcslen(tmp);
    for (i = 3; i < n; i++) {
        if (tmp[i] == L'\\') {
            tmp[i] = 0;
            CreateDirectoryW(tmp, NULL);
            tmp[i] = L'\\';
        }
    }
    CreateDirectoryW(tmp, NULL);
}

void xbox_path_set_title_redirect(int on)
{
    if (!s_initialized)
        xbox_path_init(NULL, NULL);
    if (on) {
        WCHAR real_dir[MAX_PATH], copy_dir[MAX_PATH];
        tdata_dir(real_dir, 0);
        tdata_dir(copy_dir, 1);
        make_dirs(real_dir);
        copy_dir_files(real_dir, copy_dir);
    }
    InterlockedExchange(&s_title_redirect, on ? 1 : 0);
    xbox_log(XBOX_LOG_INFO, XBOX_LOG_PATH, "TitleData redirect %s", on ? "ON" : "OFF");
}

/* The HDD root next to the exe, and the disc inside it. A pre-HDD install
 * (an "assets" folder next to the exe) is moved to HDD\D once: a rename on
 * the same volume, nothing is copied or deleted. */
const WCHAR *xbox_path_disc_dir(void)
{
    if (!s_initialized)
        xbox_path_init(NULL, NULL);
    return s_game_dir;
}

void xbox_path_init(const char* game_dir, const char* save_dir)
{
    WCHAR exe_dir[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, exe_dir);         /* main() runs from the exe's folder */

    if (save_dir) MultiByteToWideChar(CP_UTF8, 0, save_dir, -1, s_hdd_dir, MAX_PATH);
    else          swprintf_s(s_hdd_dir, MAX_PATH, L"%s\\HDD", exe_dir);
    {   size_t len = wcslen(s_hdd_dir);
        if (len > 0 && s_hdd_dir[len - 1] == L'\\') s_hdd_dir[len - 1] = 0; }
    CreateDirectoryW(s_hdd_dir, NULL);

    if (game_dir) {
        MultiByteToWideChar(CP_UTF8, 0, game_dir, -1, s_game_dir, MAX_PATH);
    } else {
        WCHAR old_assets[MAX_PATH];
        swprintf_s(s_game_dir, MAX_PATH, L"%s\\D", s_hdd_dir);
        swprintf_s(old_assets, MAX_PATH, L"%s\\assets", exe_dir);
        if (GetFileAttributesW(s_game_dir) == INVALID_FILE_ATTRIBUTES &&
            (GetFileAttributesW(old_assets) & FILE_ATTRIBUTE_DIRECTORY) &&
            GetFileAttributesW(old_assets) != INVALID_FILE_ATTRIBUTES) {
            if (MoveFileW(old_assets, s_game_dir))
                xbox_log(XBOX_LOG_INFO, XBOX_LOG_PATH, "Moved %S to %S", old_assets, s_game_dir);
            else
                xbox_log(XBOX_LOG_WARN, XBOX_LOG_PATH, "Could not move %S to %S (error %lu)",
                         old_assets, s_game_dir, GetLastError());
        }
    }
    {   size_t len = wcslen(s_game_dir);
        if (len > 0 && s_game_dir[len - 1] == L'\\') s_game_dir[len - 1] = 0; }

    s_initialized = TRUE;
    xbox_log(XBOX_LOG_INFO, XBOX_LOG_PATH, "Path init: disc=%S, hdd=%S", s_game_dir, s_hdd_dir);
}

/*
 * Helper: check if an ANSI string starts with a prefix (case-insensitive).
 * Returns the number of chars consumed from the prefix, or 0 if no match.
 */
static int match_prefix(const char* path, const char* prefix)
{
    int i = 0;
    while (prefix[i]) {
        if (tolower((unsigned char)path[i]) != tolower((unsigned char)prefix[i]))
            return 0;
        i++;
    }
    return i;
}

/* Host directory for an Xbox drive letter, or 0 if it is not one we map. */
static int drive_dir(char letter, WCHAR *out)
{
    letter = (char)toupper((unsigned char)letter);
    switch (letter) {
    case 'D': wcscpy_s(out, MAX_PATH, s_game_dir); return 1;
    case 'T': tdata_dir(out, s_title_redirect != 0); return 1;
    case 'U': swprintf_s(out, MAX_PATH, L"%s\\E\\UDATA\\%08x", s_hdd_dir, (unsigned)DOA3_TITLE_ID); return 1;
    case 'C': case 'E': case 'F': case 'G': case 'X': case 'Y': case 'Z':
        swprintf_s(out, MAX_PATH, L"%s\\%c", s_hdd_dir, letter); return 1;
    default: return 0;
    }
}

BOOL xbox_translate_path(const char* xbox_path, WCHAR* win_path_buf, DWORD buf_size)
{
    static const struct { const char *prefix; char drive; } devices[] = {
        { "\\Device\\CdRom0\\",             'D' },
        { "\\Device\\Harddisk0\\Partition1\\", 'E' },
        { "\\Device\\Harddisk0\\Partition2\\", 'C' },
        { "\\Device\\Harddisk0\\Partition3\\", 'X' },
        { "\\Device\\Harddisk0\\Partition4\\", 'Y' },
        { "\\Device\\Harddisk0\\Partition5\\", 'Z' },
        { "\\Device\\Harddisk0\\Partition6\\", 'F' },
        { "\\Device\\Harddisk0\\Partition7\\", 'G' },
    };
    const char* remainder = NULL;
    const char* p;
    WCHAR base[MAX_PATH];
    size_t k;

    if (!xbox_path || !win_path_buf || buf_size == 0)
        return FALSE;
    if (!s_initialized)
        xbox_path_init(NULL, NULL);

    for (k = 0; k < sizeof devices / sizeof devices[0]; k++) {
        int skip = match_prefix(xbox_path, devices[k].prefix);
        if (skip) {
            drive_dir(devices[k].drive, base);
            remainder = xbox_path + skip;
            goto translate;
        }
    }
    /* X:\ or the NT object-manager form \??\X:\ */
    p = xbox_path;
    if (match_prefix(p, "\\??\\")) p += 4;
    if (p[0] && p[1] == ':' && (p[2] == '\\' || p[2] == '/') && drive_dir(p[0], base)) {
        remainder = p + 3;
        goto translate;
    }

    /* Unrecognized path - try to use as-is by converting to wide */
    xbox_log(XBOX_LOG_WARN, XBOX_LOG_PATH, "Unrecognized Xbox path: %s", xbox_path);
    MultiByteToWideChar(CP_ACP, 0, xbox_path, -1, win_path_buf, buf_size);
    return TRUE;

translate:
    {
        WCHAR remainder_wide[MAX_PATH];
        {   /* name each Xbox prefix -> host folder once, so a game that reaches
             * the same partition through two spellings shows up in the log */
            static WCHAR s_seen[16][MAX_PATH]; static int s_nseen;
            int j, seen = 0;
            for (j = 0; j < s_nseen; j++) if (!wcscmp(s_seen[j], base)) { seen = 1; break; }
            if (!seen && s_nseen < 16) {
                wcscpy_s(s_seen[s_nseen++], MAX_PATH, base);
                xbox_log(XBOX_LOG_INFO, XBOX_LOG_PATH, "%.*s -> %S",
                         (int)(remainder - xbox_path), xbox_path, base);
            }
        }
        MultiByteToWideChar(CP_ACP, 0, remainder, -1, remainder_wide, MAX_PATH);
        for (WCHAR* q = remainder_wide; *q; q++)
            if (*q == L'/') *q = L'\\';
        swprintf_s(win_path_buf, buf_size, L"%s\\%s", base, remainder_wide);
        /* The writable partitions are created on first use; the disc is not. */
        if (wcscmp(base, s_game_dir) != 0)
            make_dirs(base);
        XBOX_TRACE(XBOX_LOG_PATH, "%s -> %S", xbox_path, win_path_buf);
        return TRUE;
    }
}
