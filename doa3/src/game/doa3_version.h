/* Build version of DOA3.exe: <major>.<minor>.<update>.0. The update number
 * steps once per delivered update (the update bundles); the minor number
 * steps at a milestone and the update count starts again from 0 (0.0.197 ->
 * 0.1.0, the asynchronous GPU thread: full speed on the heaviest stage).
 * 1.0.0 is the first release (after 0.2.34). The update check compares the
 * GitHub release tag (v1.0.0) with these three numbers. Shown in
 * the exe's Properties > Details, in crash reports (Event Viewer "version:")
 * and on the first line of doa3_log.txt. */
#ifndef DOA3_VERSION_H
#define DOA3_VERSION_H
#define DOA3_VER_MAJOR  1
#define DOA3_VER_MINOR  0
#define DOA3_VER_UPDATE 1
#define DOA3_VER_BUILD  0
#define DOA3_VER_STR    "1.0.1"
#endif
