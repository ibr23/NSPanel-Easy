// versioning.h

#pragma once

#ifdef NSPANEL_EASY_VERSIONING

#include <string>

namespace esphome::nspanel_easy {

/**
 * @brief Compare two CalVer version strings segment by segment.
 *
 * Compares two version strings in the format `YYYY.M.seq` (e.g. "2026.10.2")
 * using numeric comparison per segment. This avoids the lexicographic ordering
 * pitfall where "2026.9.0" would incorrectly sort after "2026.10.0" as strings.
 *
 * The comparison evaluates as: version >= min_version
 *
 * Segment priority (left to right):
 *   1. Year  (YYYY) — most significant
 *   2. Month (M)    — no leading zero, 1–12
 *   3. Seq   (seq)  — release sequence within the month
 *
 * @param version     The version string to test (e.g. the installed blueprint version).
 * @param min_version The minimum required version string to compare against.
 * @return true  if version is equal to or newer than min_version.
 * @return false if version is older than min_version, or if either string is malformed.
 *
 * @note Returns false conservatively when either string cannot be parsed,
 *       to avoid incorrectly passing a version check on bad input.
 * @note Both strings must follow the `YYYY.M.seq` format with no leading
 *       zeros on month or sequence (as produced by the CI/CD versioning workflow).
 *
 * @code
 * calver_gte("2026.10.1", "2026.2.3")  // true  — month 10 > month 2
 * calver_gte("2026.2.3",  "2026.10.1") // false — month 2 < month 10
 * calver_gte("2026.4.1",  "2026.4.1")  // true  — equal
 * calver_gte("",          "2026.4.1")  // false — malformed input
 * @endcode
 */
bool calver_gte(const std::string &version, const std::string &min_version);

}  // namespace esphome::nspanel_easy

#endif  // NSPANEL_EASY_VERSIONING
