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

#ifndef FOLLY_EXT_STRING_H_
#define FOLLY_EXT_STRING_H_

#include <string>
#include <boost/type_traits.hpp>

#include <folly/Conv.h>
#include <folly/FBString.h>
#include <folly/FBVector.h>
#include <folly/Portability.h>
#include <folly/Range.h>
#include <folly/Unicode.h>
#include <folly/ext/Utf8StringPiece.h>

// Compatibility function, to make sure toStdString(s) can be called
// to convert a std::string or fbstring variable s into type std::string
// with very little overhead if s was already std::string
namespace folly {

/**
 * Case insensitive string less.
 */
struct StrCaseLess {
  bool operator()(const folly::fbstring& x, const folly::fbstring& y) const {
    return strcasecmp(x.c_str(), y.c_str()) < 0;
  }

  bool operator()(const std::string& x, const std::string& y) const {
    return strcasecmp(x.c_str(), y.c_str()) < 0;
  }
};

/*
 * Split a string into lines by '\n', '\n' IS included.
 */
template<class String, class OutputType>
void splitLines(const String& input, std::vector<OutputType>& out);

template<class String, class OutputType>
void splitLines(const String& input, folly::fbvector<OutputType>& out);

#if FOLLY_HAVE_LIBICU

/*
 * Split a string into lines using soft break mode.
 */
template<class String, class OutputType>
void splitSoftBreakLines(const String& input,
                         std::vector<OutputType>& out,
                         size_t width,
                         size_t tabSize,
                         size_t prefixSize = 0);

template<class String, class OutputType>
void splitSoftBreakLines(const String& input,
                         folly::fbvector<OutputType>& out,
                         size_t width,
                         size_t tabSize,
                         size_t prefixSize = 0);

#endif

/**
 * Returns a subpiece with all trim chars removed from the front and end of @sp.
 */
StringPiece trim(StringPiece sp, StringPiece chars);

enum class LineEnding {
  NONE,
  LF,
  CR,
  CRLF,
  MIXED,
};

LineEnding estimateLineEnding(StringPiece sp);

template <class String>
typename std::enable_if<IsSomeString<String>::value>::type
convertToUnixStyleLineEnding(String& str);

} // namespace folly

#include <folly/ext/String-inl.h>

#endif
