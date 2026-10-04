/*
 * xiso_extract.h - first-run asset install from a Dead or Alive 3 disc image.
 *
 * Reads the Xbox XDVDFS filesystem directly (the format extract-xiso
 * handles) from either an .xiso / .xiso.iso or a full redump .iso, checks
 * the image really is the release this port was recompiled from, and copies
 * its files into the assets folder next to the exe.
 */

#ifndef DOA3_XISO_EXTRACT_H
#define DOA3_XISO_EXTRACT_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct XisoProgress {
    volatile LONG64 bytes_done;
    volatile LONG64 bytes_total;
    volatile LONG   files_done;
    volatile LONG   files_total;
    volatile LONG   cancel;           /* set by the caller to abort */
    char            current_name[64]; /* file being written (display only) */
} XisoProgress;

/* Non-zero when every file the game needs is present in assets_dir. */
int xiso_assets_ready(const WCHAR *assets_dir);

/* Validate `image`, extract it into `assets_dir` and verify the result.
 * Blocking; run it on a worker thread and poll `prog`. Returns 1 on success.
 * On failure writes a user-facing explanation to err. */
int xiso_install(const WCHAR *image, const WCHAR *assets_dir,
                 XisoProgress *prog, char *err, size_t errlen);

#ifdef __cplusplus
}
#endif

#endif /* DOA3_XISO_EXTRACT_H */
