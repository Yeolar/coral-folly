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

#ifndef FOLLY_EXT_STRING_INL_H_
#define FOLLY_EXT_STRING_INL_H_

#include <stdexcept>
#include <iterator>

#ifndef FOLLY_EXT_STRING_H_
#error This file may only be included from ext/String.h
#endif

namespace folly {

namespace detail {

/*
 * These output conversion templates allow us to support multiple
 * output string types, even when we are using an arbitrary
 * OutputIterator.
 */
template<class OutStringT> struct OutputConverter {};

template<> struct OutputConverter<std::string> {
  std::string operator()(StringPiece sp) const {
    return sp.toString();
  }
};

template<> struct OutputConverter<fbstring> {
  fbstring operator()(StringPiece sp) const {
    return sp.toFbstring();
  }
};

template<> struct OutputConverter<StringPiece> {
  StringPiece operator()(StringPiece sp) const { return sp; }
};

/*
 * For splitLines.
 */
template<class OutStringT, class OutputIterator>
void internalSplitLines(StringPiece sp, OutputIterator out) {
  assert(sp.empty() || sp.start() != nullptr);

  const char* s = sp.start();
  const size_t strSize = sp.size();

  OutputConverter<OutStringT> conv;

  size_t tokenStartPos = 0;
  size_t tokenSize = 0;
  for (size_t i = 0; i < strSize; ++i) {
    ++tokenSize;
    if (s[i] == '\n') {
      *out++ = conv(StringPiece(&s[tokenStartPos], tokenSize));
      tokenStartPos = i + 1;
      tokenSize = 0;
    }
  }
  tokenSize = strSize - tokenStartPos; // empty line also added
  *out++ = conv(StringPiece(&s[tokenStartPos], tokenSize));
}

#if FOLLY_HAVE_LIBICU

/*
 * For splitSoftBreakLines.
 */
template<class OutStringT, class OutputIterator>
void internalSplitSoftBreakLines(Utf8StringPiece sp,
                                 OutputIterator out,
                                 size_t width,
                                 size_t tabSize,
                                 size_t prefixSize) {
  assert(sp.empty() || &(sp.start()) != nullptr);

  const char* s = &sp.start();

  OutputConverter<OutStringT> conv;

  size_t i = 0;       // char byte position
  size_t col = 0;     // column position at current line
  size_t begin = 0;   // begin of soft breaked line to output
  size_t spaceCol = 0, spacePos = 0;
  bool firstLine = true;

  for (auto it = sp.begin(); it != sp.end(); ++it) {
    if (unicode::isNonBase(*it)) {
      continue;
    }
    size_t prevCol = col, prevPos = i;
    i += it.length();
    col += ((*it == '\t') ? tabSize - (col % tabSize)
                          : (unicode::isWide(*it) ? 2 : 1));

    if (*it == '\n') {  // new line
      col = 0;
      spaceCol = 0;
      spacePos = i;
      continue;
    }
    if (col > width) { // break the line
      if (spaceCol == 0) { // no space in line, just break at prevPos
        *out++ = conv(StringPiece(&s[begin], prevPos - begin));
        begin = prevPos;
        col -= prevCol;
        if (firstLine) {
          width -= prefixSize;
          firstLine = false;
        }
      } else { // have space, break at its position
        *out++ = conv(StringPiece(&s[begin], spacePos - begin));
        begin = spacePos;
        col -= spaceCol;
        spaceCol = 0;
        if (firstLine) {
          width -= prefixSize;
          firstLine = false;
        }
        // Still too wide, back to the break
        if (col > width) {
          it = Utf8CharIterator(s + spacePos);
          i = spacePos + it.length();
          col = (*it == '\t') ? tabSize : 1;
        }
      }
    } else if (*it == ' ') {
      spaceCol = col;
      spacePos = i;
    }
  }
  if (begin < sp.size()) {
    *out++ = conv(StringPiece(&s[begin], sp.size() - begin));
  }
}

#endif

} // namespace detail

template<class String, class OutputType>
void splitLines(const String& input, std::vector<OutputType>& out) {
  detail::internalSplitLines<OutputType>(
    StringPiece(input),
    std::back_inserter(out));
}

template<class String, class OutputType>
void splitLines(const String& input, fbvector<OutputType>& out) {
  detail::internalSplitLines<OutputType>(
    StringPiece(input),
    std::back_inserter(out));
}

#if FOLLY_HAVE_LIBICU

template<class String, class OutputType>
void splitSoftBreakLines(const String& input,
                         std::vector<OutputType>& out,
                         size_t width,
                         size_t tabSize,
                         size_t prefixSize/* = 0 */) {
  detail::internalSplitSoftBreakLines<OutputType>(
    Utf8StringPiece(input),
    std::back_inserter(out),
    width,
    tabSize,
    prefixSize);
}

template<class String, class OutputType>
void splitSoftBreakLines(const String& input,
                         fbvector<OutputType>& out,
                         size_t width,
                         size_t tabSize,
                         size_t prefixSize/* = 0 */) {
  detail::internalSplitSoftBreakLines<OutputType>(
    Utf8StringPiece(input),
    std::back_inserter(out),
    width,
    tabSize,
    prefixSize);
}

#endif

template <class String>
typename std::enable_if<IsSomeString<String>::value>::type
convertToUnixStyleLineEnding(String& str) {
  auto begin = str.begin();
  auto end = str.end();
  auto p = begin;

  for (auto it = begin; it != end; ++p) {
    if (*it == '\r') {
      *p = '\n';
      ++it;
      if (it != end && *it == '\n') {
        ++it;
      }
    } else {
      *p = *it;
      ++it;
    }
  }
  str.resize(p - begin);
}

}  // namespace folly

#endif /* FOLLY_EXT_STRING_INL_H_ */
