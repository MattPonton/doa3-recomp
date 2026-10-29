/* Build version of DOA3.exe: <major>.<minor>.<update>.<build>.
 *
 * Releases (1.0.2 onward) count up one at a time: v1.0.1, v1.0.2, v1.0.3.
 * Test builds between two releases carry the NEXT release's three numbers
 * plus a build number from 1 (after v1.0.1: 1.0.2.1, 1.0.2.2, ...). When one
 * is released, the build number goes back to 0 and the release is published
 * as v<major>.<minor>.<update>; the release workflow refuses a build number
 * other than 0. (Before 1.0.0 the update number stepped once per delivered
 * update and the minor at a milestone: 0.0.197 -> 0.1.0, 0.2.34 -> 1.0.0.)
 *
 * DOA3_VER_STR is the release version (three numbers, read by the release
 * workflow); DOA3_VER_FULL_STR adds the build number while it is not 0. The
 * update check compares the GitHub release tag with the three numbers, and
 * treats a test build as older than the release of the same version. The
 * full version is shown in the exe's Properties > Details, in crash reports
 * (Event Viewer "version:"), on the first line of doa3_log.txt and in the
 * General tab. */
#ifndef DOA3_VERSION_H
#define DOA3_VERSION_H
#define DOA3_VER_MAJOR  1
#define DOA3_VER_MINOR  0
#define DOA3_VER_UPDATE 2
#define DOA3_VER_BUILD  6
#define DOA3_VER_STR    "1.0.2"
#define DOA3_VER_FULL_STR "1.0.2.6"
#endif
