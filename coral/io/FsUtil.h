/*
 * Copyright 2015 Yeolar
 * Copyright 2015 Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CORAL_IO_FSUTIL_H_
#define CORAL_IO_FSUTIL_H_

#include <vector>
#include <pwd.h>
#include <boost/filesystem.hpp>

#include <coral/Range.h>

namespace coral {
namespace fs {

/**
 * Shell escape.
 *
 * Rules:
 *   \n  ->  "'\n'"
 *   [^A-Za-z0-9_\\-.,:/@\x7F-\xFF]  ->  "\\C"
 */
template <class String>
void shellEscape(StringPiece str, String& out);

/**
 * Similar to shellEscape above, but returns the escaped string.
 */
template <class String>
String shellEscape(StringPiece str) {
  String out;
  shellEscape(str, out);
  return out;
}

/**
 * Shell unescape and split into words.
 */
template<class String>
void shellSplit(const String& input, std::vector<String>& out);

///////////////////////////////////////////////////////////////////////////

// Functions defined in this file are meant to extend the
// boost::filesystem library; functions will be named according to boost's
// naming conventions instead of ours.  For convenience, import the
// boost::filesystem namespace into coral::fs.
using namespace ::boost::filesystem;

/**
 * Different from path.normalize(), handle some special case.
 *
 * ./a/b/.  -> a/b
 * /..      -> ""
 * /../a    -> /a
 * //a/b    -> /a/b     path.normalize() recognize head "//" as protocol
 */
path normalize(const path& p);

/**
 * Check whether "path" starts with "prefix".
 * That is, if prefix has n path elements, then the first n elements of
 * path must be the same as prefix.
 *
 * There is a special case if prefix ends with a slash:
 * /foo/bar/ is not a prefix of /foo/bar, but both /foo/bar and /foo/bar/
 * are prefixes of /foo/bar/baz.
 */
bool starts_with(const path& p, const path& prefix);

/**
 * If "path" starts with "prefix", return "path" with "prefix" removed.
 * Otherwise, throw filesystem_error.
 */
path remove_prefix(const path& p, const path& prefix);

/**
 * Canonicalize the parent path, leaving the filename (last component)
 * unchanged.  You may use this before creating a file instead of
 * boost::filesystem::canonical, which requires that the entire path exists.
 */
path canonical_parent(const path& p, const path& basePath = current_path());

/**
 * Get the path to the current executable.
 *
 * Note that this is not reliable and not recommended in general; it may not be
 * implemented on your platform (in which case it will throw), the executable
 * might have been moved or replaced while running, and applications comprising
 * of multiple executables should use some form of configuration system to
 * find the other executables rather than relying on relative paths from one
 * to another.
 *
 * So this should only be used for tests, logging, or other innocuous purposes.
 */
path executable_path();

/**
 * Get device.
 */
dev_t device(const path& p);

/**
 * passwd entry get by getpwuid(getuid()).
 */
const passwd* passwd_entry();

/**
 * User home path.
 */
path home_path();

/**
 * If with home path prefix, return path relative to it: ~/... ,
 * else return p.
 */
path home_relative_path(const path& p);

/**
 * Returns relative path to base.
 */
path relative_path(const path& p, const path& base);

}  // namespace fs
}  // namespace coral

#include <coral/io/FsUtil-inl.h>

#endif /* CORAL_IO_FSUTIL_H_ */
