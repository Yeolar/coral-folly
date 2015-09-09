/*
 * Copyright (C) 2015, Yeolar
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

#ifndef FOLLY_EXT_IO_FSUTIL_H_
#define FOLLY_EXT_IO_FSUTIL_H_

#include <vector>
#include <pwd.h>

#include <folly/Range.h>
#include <folly/experimental/io/FsUtil.h>

namespace folly {
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
}  // namespace folly

#include <folly/ext/io/FsUtil-inl.h>

#endif /* FOLLY_EXT_IO_FSUTIL_H_ */
