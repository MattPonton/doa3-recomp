/*
 * xiso_extract.c - first-run asset install from a Dead or Alive 3 disc image.
 *
 * XDVDFS layout (what extract-xiso reads):
 *   sector 32 of the game partition holds the volume descriptor:
 *     +0x000  "MICROSOFT*XBOX*MEDIA"
 *     +0x014  u32 root directory sector
 *     +0x018  u32 root directory size
 *     +0x7EC  "MICROSOFT*XBOX*MEDIA" again
 *   a directory is a binary tree of 4-byte aligned entries:
 *     u16 left, u16 right (offsets in dwords, 0 = none), u32 sector,
 *     u32 size, u8 attributes (0x10 = directory), u8 name length, name
 *
 * An .xiso holds the game partition at offset 0. A full redump .iso has the
 * video partition first and the game partition further in, so the known
 * partition offsets are probed and whichever carries the descriptor wins.
 * That is why .xiso, .xiso.iso and .iso all work.
 *
 * The image is extracted into "<assets>.extracting", every file is checked
 * against the directory sizes, and only then moved into the assets folder,
 * so a failed or cancelled install never leaves a half-written assets folder
 * that looks complete.
 */

#include "xiso_extract.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define XDVDFS_SECTOR      2048u
#define XDVDFS_HEADER_SECT 32u
#define XDVDFS_MAGIC       "MICROSOFT*XBOX*MEDIA"
#define XDVDFS_ATTR_DIR    0x10

/* Game partition offsets: xiso, XGD1 (DOA3's disc), XGD2, XGD3. */
static const uint64_t k_partitions[] = { 0, 0x18300000ull, 0xFD90000ull, 0x2080000ull };

/* The release this port was recompiled from: the recompiled code is tied to
 * this exact default.xbe, so any other build of the game cannot run. */
#include "xbe_layout.h"   /* DOA3_TITLE_ID, DOA3_XBE_FILE_SIZE, DOA3_XBE_FNV64 */
#define DOA3_XBE_SIZE   DOA3_XBE_FILE_SIZE

/* The sizes below are the USA release's (3.0). Other releases are checked
 * for presence only (size 0 = any), except default.xbe, which must be the
 * exact executable the code was generated from. TODO(3.1): add this
 * release's disc listing. */

/* Every file on the disc, with its size. The game reads all of them
 * (the mv_*.sfd are the story endings), so a missing one is fatal later. */
static const struct { const char *name; uint32_t size; } k_required[] = {
    { "default.xbe",    3928064u }, { "dsstdfx.bin",      25728u },
    { "loadfile.afs", 365690880u }, { "bgm.afs",      226709504u },
    { "voice.afs",     10846208u }, { "ninja.sfd",     35231744u },
    { "ninja.aix",      3102720u }, { "mv_ay.sfd",    222840832u },
    { "mv_ba.sfd",    235456512u }, { "mv_fa.sfd",    202610688u },
    { "mv_go.sfd",    146843648u }, { "mv_gy.sfd",    243902464u },
    { "mv_hi.sfd",    200595456u }, { "mv_kl.sfd",    152932352u },
    { "mv_km.sfd",    170528768u }, { "mv_ko.sfd",    205934592u },
    { "mv_ks.sfd",    216549376u }, { "mv_le.sfd",    152907776u },
    { "mv_lu.sfd",    156659712u }, { "mv_ni.sfd",    137613312u },
    { "mv_sa.sfd",    189632512u }, { "mv_st.sfd",    454651904u },
    { "mv_su.sfd",    244328448u }, { "mv_tn.sfd",    211617792u },
};
#define N_REQUIRED (sizeof k_required / sizeof k_required[0])

static uint32_t required_size(size_t i)
{
#ifdef DOA3_XBE_ID_3_0
    return k_required[i].size;
#else
    return strcmp(k_required[i].name, "default.xbe") == 0 ? DOA3_XBE_SIZE : 0u;
#endif
}

typedef struct {
    WCHAR    rel[MAX_PATH];   /* path relative to the image root */
    char     name[64];        /* leaf name, for display */
    int      is_dir;
    uint32_t sector;
    uint32_t size;
} XisoEntry;

typedef struct {
    HANDLE     h;
    uint64_t   image_size;
    uint64_t   part;          /* game partition offset */
    XisoEntry *e;
    int        n, cap;
    char      *err;
    size_t     errlen;
} Xiso;

static int fail(Xiso *x, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(x->err, x->errlen, fmt, ap);
    va_end(ap);
    return 0;
}

static int read_at(HANDLE h, uint64_t off, void *buf, DWORD len)
{
    OVERLAPPED ov;
    DWORD got = 0;
    memset(&ov, 0, sizeof ov);
    ov.Offset = (DWORD)off;
    ov.OffsetHigh = (DWORD)(off >> 32);
    return ReadFile(h, buf, len, &got, &ov) && got == len;
}

/* ── Reading the filesystem ───────────────────────────────────────────── */

static int find_partition(Xiso *x)
{
    static char sect[XDVDFS_SECTOR];
    for (size_t i = 0; i < sizeof k_partitions / sizeof k_partitions[0]; i++) {
        uint64_t off = k_partitions[i] + (uint64_t)XDVDFS_HEADER_SECT * XDVDFS_SECTOR;
        if (off + XDVDFS_SECTOR > x->image_size) continue;
        if (!read_at(x->h, off, sect, XDVDFS_SECTOR)) continue;
        if (memcmp(sect, XDVDFS_MAGIC, 20) || memcmp(sect + 0x7EC, XDVDFS_MAGIC, 20))
            continue;
        x->part = k_partitions[i];
        return 1;
    }
    return 0;
}

static int name_ok(const char *s, int len)
{
    if (len <= 0) return 0;
    if ((len == 1 && s[0] == '.') || (len == 2 && s[0] == '.' && s[1] == '.')) return 0;
    for (int i = 0; i < len; i++) {
        unsigned char c = (unsigned char)s[i];
        if (c < 0x20 || c >= 0x7F || strchr("\\/:*?\"<>|", c)) return 0;
    }
    return 1;
}

static int walk_dir(Xiso *x, uint32_t sector, uint32_t size, const WCHAR *prefix, int depth);

/* In-order walk of one directory's tree. `budget` bounds the node count so a
 * corrupt image with a cycle in it cannot loop forever. */
static int walk_node(Xiso *x, const uint8_t *tab, uint32_t tsize, uint32_t off,
                     const WCHAR *prefix, int depth, int *budget)
{
    uint16_t left, right;
    uint32_t sector, size;
    uint8_t attr, nlen;
    XisoEntry *e;
    WCHAR wname[64];
    size_t pos = (size_t)off * 4;

    if (--*budget < 0) return fail(x, "The disc image's file table is damaged (loop in directory).");
    if (pos + 14 > tsize) return fail(x, "The disc image's file table is damaged (entry out of range).");
    memcpy(&left, tab + pos, 2);
    memcpy(&right, tab + pos + 2, 2);
    memcpy(&sector, tab + pos + 4, 4);
    memcpy(&size, tab + pos + 8, 4);
    attr = tab[pos + 12];
    nlen = tab[pos + 13];
    if (left == 0xFFFF) return fail(x, "The disc image's file table is damaged (padding entry).");
    if (pos + 14 + nlen > tsize || !name_ok((const char *)tab + pos + 14, nlen))
        return fail(x, "The disc image's file table is damaged (bad file name).");

    if (left && !walk_node(x, tab, tsize, left, prefix, depth, budget)) return 0;

    if (x->n == x->cap) {
        int cap = x->cap ? x->cap * 2 : 64;
        XisoEntry *ne = (XisoEntry *)realloc(x->e, (size_t)cap * sizeof *ne);
        if (!ne) return fail(x, "Out of memory reading the disc image.");
        x->e = ne; x->cap = cap;
    }
    e = &x->e[x->n++];
    memset(e, 0, sizeof *e);
    memcpy(e->name, tab + pos + 14, nlen < 63 ? nlen : 63);
    MultiByteToWideChar(CP_ACP, 0, e->name, -1, wname, 64);
    if (swprintf_s(e->rel, MAX_PATH, prefix[0] ? L"%s\\%s" : L"%s%s", prefix, wname) < 0)
        return fail(x, "A path on the disc image is too long.");
    e->is_dir = (attr & XDVDFS_ATTR_DIR) != 0;
    e->sector = sector;
    e->size = size;
    if (!e->is_dir &&
        x->part + (uint64_t)sector * XDVDFS_SECTOR + size > x->image_size)
        return fail(x, "The disc image is incomplete: %s runs past the end of the file. "
                       "The download or dump was probably cut short.", e->name);

    if (e->is_dir) {
        WCHAR sub[MAX_PATH];
        wcscpy_s(sub, MAX_PATH, e->rel);
        if (!walk_dir(x, sector, size, sub, depth + 1)) return 0;
    }

    if (right && !walk_node(x, tab, tsize, right, prefix, depth, budget)) return 0;
    return 1;
}

static int walk_dir(Xiso *x, uint32_t sector, uint32_t size, const WCHAR *prefix, int depth)
{
    uint8_t *tab;
    int ok, budget;
    uint64_t off = x->part + (uint64_t)sector * XDVDFS_SECTOR;

    if (depth > 32) return fail(x, "The disc image's file table is damaged (too deep).");
    if (size == 0) return 1;                                  /* empty directory */
    if (size > 16u * 1024 * 1024 || off + size > x->image_size)
        return fail(x, "The disc image's file table is damaged (directory out of range).");
    tab = (uint8_t *)malloc(size);
    if (!tab) return fail(x, "Out of memory reading the disc image.");
    if (!read_at(x->h, off, tab, size)) { free(tab); return fail(x, "Could not read the disc image."); }
    /* A directory whose first sector is all padding is empty. */
    if (size >= 4 && tab[0] == 0xFF && tab[1] == 0xFF && tab[2] == 0xFF && tab[3] == 0xFF) {
        free(tab);
        return 1;
    }
    budget = (int)(size / 14) + 1;
    ok = walk_node(x, tab, size, 0, prefix, depth, &budget);
    free(tab);
    return ok;
}

static const XisoEntry *find_root_file(const Xiso *x, const char *name)
{
    for (int i = 0; i < x->n; i++)
        if (!x->e[i].is_dir && !wcschr(x->e[i].rel, L'\\') && !_stricmp(x->e[i].name, name))
            return &x->e[i];
    return NULL;
}

/* default.xbe must be the exact executable the port was recompiled from. */
static int check_xbe(Xiso *x)
{
    const XisoEntry *xbe = find_root_file(x, "default.xbe");
    uint8_t *d;
    uint32_t base, cert, title_id;
    uint64_t h = 0xCBF29CE484222325ull;

    if (!xbe)
        return fail(x, "This disc image has no default.xbe, so it is not an Xbox game disc.");
    if (xbe->size < 0x200 || xbe->size > 64u * 1024 * 1024)
        return fail(x, "This disc image's default.xbe is damaged.");
    d = (uint8_t *)malloc(xbe->size);
    if (!d) return fail(x, "Out of memory reading the disc image.");
    if (!read_at(x->h, x->part + (uint64_t)xbe->sector * XDVDFS_SECTOR, d, xbe->size)) {
        free(d);
        return fail(x, "Could not read default.xbe from the disc image.");
    }
    memcpy(&base, d + 0x104, 4);
    memcpy(&cert, d + 0x118, 4);
    if (memcmp(d, "XBEH", 4) || cert < base || (uint64_t)cert - base + 0xC + 80 > xbe->size) {
        free(d);
        return fail(x, "This disc image's default.xbe is damaged.");
    }
    cert -= base;
    memcpy(&title_id, d + cert + 8, 4);
    if (title_id != DOA3_TITLE_ID) {
        WCHAR wt[41];
        char title[128];
        memcpy(wt, d + cert + 0xC, 80);
        wt[40] = 0;
        if (!WideCharToMultiByte(CP_UTF8, 0, wt, -1, title, sizeof title, NULL, NULL) || !title[0])
            strcpy_s(title, sizeof title, "another game");
        free(d);
        return fail(x, "This disc image is \"%s\", not Dead or Alive 3.", title);
    }
    for (uint32_t i = 0; i < xbe->size; i++)
        h = (h ^ d[i]) * 0x100000001B3ull;
    free(d);
    if (xbe->size != DOA3_XBE_SIZE || h != DOA3_XBE_FNV64)
        return fail(x, "This is a different release of Dead or Alive 3. This build is "
                       "generated from DOA3 " DOA3_XBE_VERSION " and cannot run "
                       "any other version.");
    return 1;
}

/* ── Writing ──────────────────────────────────────────────────────────── */

static void delete_tree(const WCHAR *dir)
{
    WCHAR pat[MAX_PATH], p[MAX_PATH];
    WIN32_FIND_DATAW fd;
    HANDLE h;
    swprintf_s(pat, MAX_PATH, L"%s\\*", dir);
    h = FindFirstFileW(pat, &fd);
    if (h != INVALID_HANDLE_VALUE) {
        do {
            if (!wcscmp(fd.cFileName, L".") || !wcscmp(fd.cFileName, L"..")) continue;
            swprintf_s(p, MAX_PATH, L"%s\\%s", dir, fd.cFileName);
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) delete_tree(p);
            else { SetFileAttributesW(p, FILE_ATTRIBUTE_NORMAL); DeleteFileW(p); }
        } while (FindNextFileW(h, &fd));
        FindClose(h);
    }
    RemoveDirectoryW(dir);
}

/* Move everything under `from` into `to`, replacing same-named files. Both
 * sit next to the exe, so every move is a rename on the same volume. */
static int merge_move(const WCHAR *from, const WCHAR *to)
{
    WCHAR pat[MAX_PATH], s[MAX_PATH], d[MAX_PATH];
    WIN32_FIND_DATAW fd;
    HANDLE h;
    int ok = 1;
    CreateDirectoryW(to, NULL);
    swprintf_s(pat, MAX_PATH, L"%s\\*", from);
    h = FindFirstFileW(pat, &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;
    do {
        if (!wcscmp(fd.cFileName, L".") || !wcscmp(fd.cFileName, L"..")) continue;
        swprintf_s(s, MAX_PATH, L"%s\\%s", from, fd.cFileName);
        swprintf_s(d, MAX_PATH, L"%s\\%s", to, fd.cFileName);
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (!merge_move(s, d)) ok = 0;
        } else {
            SetFileAttributesW(d, FILE_ATTRIBUTE_NORMAL);
            if (!MoveFileExW(s, d, MOVEFILE_REPLACE_EXISTING)) ok = 0;
        }
    } while (FindNextFileW(h, &fd));
    FindClose(h);
    RemoveDirectoryW(from);
    return ok;
}

static int extract_file(Xiso *x, const XisoEntry *e, const WCHAR *dst,
                        uint8_t *buf, DWORD bufsize, XisoProgress *p)
{
    HANDLE out = CreateFileW(dst, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    uint64_t src = x->part + (uint64_t)e->sector * XDVDFS_SECTOR;
    uint32_t left = e->size;
    if (out == INVALID_HANDLE_VALUE)
        return fail(x, "Could not create %s in the assets folder (error %lu). "
                       "Check the game folder is not read-only.", e->name, GetLastError());
    while (left) {
        DWORD chunk = left < bufsize ? left : bufsize, wrote = 0;
        if (p->cancel) { CloseHandle(out); return fail(x, "Cancelled."); }
        if (!read_at(x->h, src, buf, chunk)) {
            CloseHandle(out);
            return fail(x, "Could not read %s from the disc image (error %lu).", e->name, GetLastError());
        }
        if (!WriteFile(out, buf, chunk, &wrote, NULL) || wrote != chunk) {
            DWORD le = GetLastError();
            CloseHandle(out);
            if (le == ERROR_DISK_FULL || le == ERROR_HANDLE_DISK_FULL)
                return fail(x, "The drive ran out of space while writing %s.", e->name);
            return fail(x, "Could not write %s (error %lu).", e->name, le);
        }
        src += chunk;
        left -= chunk;
        InterlockedExchangeAdd64(&p->bytes_done, chunk);
    }
    CloseHandle(out);
    return 1;
}

static int file_size_is(const WCHAR *path, uint32_t size)
{
    WIN32_FILE_ATTRIBUTE_DATA fa;
    if (!GetFileAttributesExW(path, GetFileExInfoStandard, &fa)) return 0;
    if (fa.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) return 0;
    if (size == 0) return 1;   /* presence only */
    return fa.nFileSizeHigh == 0 && fa.nFileSizeLow == size;
}

/* ── Public ───────────────────────────────────────────────────────────── */

int xiso_assets_ready(const WCHAR *assets_dir)
{
    WCHAR p[MAX_PATH], wn[64];
    for (size_t i = 0; i < N_REQUIRED; i++) {
        MultiByteToWideChar(CP_ACP, 0, k_required[i].name, -1, wn, 64);
        swprintf_s(p, MAX_PATH, L"%s\\%s", assets_dir, wn);
        if (!file_size_is(p, required_size(i))) return 0;
    }
    return 1;
}

int xiso_install(const WCHAR *image, const WCHAR *assets_dir,
                 XisoProgress *prog, char *err, size_t errlen)
{
    Xiso x;
    LARGE_INTEGER sz;
    WCHAR stage[MAX_PATH], parent[MAX_PATH], p[MAX_PATH], *slash;
    ULARGE_INTEGER avail;
    uint64_t total = 0;
    uint8_t *buf = NULL;
    const DWORD bufsize = 4u * 1024 * 1024;
    int ok = 0, files = 0;

    memset(&x, 0, sizeof x);
    stage[0] = 0;
    x.err = err; x.errlen = errlen;
    err[0] = 0;

    x.h = CreateFileW(image, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (x.h == INVALID_HANDLE_VALUE) {
        fail(&x, "Could not open the selected file (error %lu).", GetLastError());
        return 0;
    }
    if (!GetFileSizeEx(x.h, &sz)) { fail(&x, "Could not read the selected file."); goto done; }
    x.image_size = (uint64_t)sz.QuadPart;

    if (!find_partition(&x)) {
        fail(&x, "This file is not an Xbox disc image. Select the Dead or Alive 3 "
                 ".xiso / .xiso.iso (or the original .iso dump).");
        goto done;
    }
    {
        static char sect[XDVDFS_SECTOR];
        uint32_t root_sect, root_size;
        if (!read_at(x.h, x.part + (uint64_t)XDVDFS_HEADER_SECT * XDVDFS_SECTOR, sect, XDVDFS_SECTOR)) {
            fail(&x, "Could not read the disc image.");
            goto done;
        }
        memcpy(&root_sect, sect + 20, 4);
        memcpy(&root_size, sect + 24, 4);
        if (!walk_dir(&x, root_sect, root_size, L"", 0)) goto done;
    }
    if (!check_xbe(&x)) goto done;
    for (size_t i = 0; i < N_REQUIRED; i++) {
        const XisoEntry *e = find_root_file(&x, k_required[i].name);
        if (!e || (required_size(i) && e->size != required_size(i))) {
            fail(&x, "The disc image is damaged: %s is %s.", k_required[i].name,
                 e ? "the wrong size" : "missing");
            goto done;
        }
    }

    for (int i = 0; i < x.n; i++)
        if (!x.e[i].is_dir) { total += x.e[i].size; files++; }
    prog->bytes_total = (LONG64)total;
    prog->files_total = files;

    /* Stage next to the assets folder: same volume, so the final move is a rename. */
    swprintf_s(stage, MAX_PATH, L"%s.extracting", assets_dir);
    wcscpy_s(parent, MAX_PATH, assets_dir);
    slash = wcsrchr(parent, L'\\');
    if (slash) slash[1] = 0;
    if (GetDiskFreeSpaceExW(parent, &avail, NULL, NULL) &&
        avail.QuadPart < total + 64ull * 1024 * 1024) {
        fail(&x, "Not enough free space: the game files need %.1f GB and the drive "
                 "has %.1f GB free.", total / 1073741824.0, avail.QuadPart / 1073741824.0);
        goto done;
    }
    delete_tree(stage);                     /* left over from an interrupted install */
    if (!CreateDirectoryW(stage, NULL)) {
        fail(&x, "Could not create a folder next to the game (error %lu). "
                 "Move the game out of Program Files or any read-only folder.", GetLastError());
        goto done;
    }

    buf = (uint8_t *)malloc(bufsize);
    if (!buf) { fail(&x, "Out of memory."); goto done; }
    for (int i = 0; i < x.n; i++) {
        const XisoEntry *e = &x.e[i];
        swprintf_s(p, MAX_PATH, L"%s\\%s", stage, e->rel);
        if (e->is_dir) {
            if (!CreateDirectoryW(p, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
                fail(&x, "Could not create folder %s (error %lu).", e->name, GetLastError());
                goto done;
            }
            continue;
        }
        strcpy_s(prog->current_name, sizeof prog->current_name, e->name);
        if (!extract_file(&x, e, p, buf, bufsize, prog)) goto done;
        InterlockedIncrement(&prog->files_done);
    }

    /* Verify what landed on disk before it replaces anything. */
    for (int i = 0; i < x.n; i++) {
        if (x.e[i].is_dir) continue;
        swprintf_s(p, MAX_PATH, L"%s\\%s", stage, x.e[i].rel);
        if (!file_size_is(p, x.e[i].size)) {
            fail(&x, "Verification failed: %s did not extract correctly.", x.e[i].name);
            goto done;
        }
    }
    if (!merge_move(stage, assets_dir)) {
        fail(&x, "Could not move the extracted files into the assets folder. "
                 "Close anything that has files in it open and try again.");
        goto done;
    }
    if (!xiso_assets_ready(assets_dir)) {
        fail(&x, "Verification failed: the assets folder is still missing game files.");
        goto done;
    }
    ok = 1;

done:
    if (!ok && stage[0]) delete_tree(stage);
    free(buf);
    free(x.e);
    CloseHandle(x.h);
    prog->current_name[0] = 0;
    return ok;
}
