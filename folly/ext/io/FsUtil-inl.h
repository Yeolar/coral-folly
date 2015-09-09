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

#ifndef FOLLY_EXT_IO_FSUTIL_INL_H_
#define FOLLY_EXT_IO_FSUTIL_INL_H_

namespace folly {
namespace fs {

namespace detail {
// Map from character code to value of one-character escape sequence
// 'E' to escape, 'n' as "'\n'", 'R' and else should be printed as is.
extern const char shellEscapeTable[];
}  // namespace detail

template <class String>
void shellEscape(StringPiece str, String& out) {
  char esc[2];
  esc[0] = '\\';
  out.reserve(out.size() + str.size());
  auto p = str.begin();
  auto last = p;  // last regular character
  // We advance over runs of regular characters and copy them in one go;
  // this is faster than calling push_back repeatedly.
  while (p != str.end()) {
    char c = *p;
    unsigned char v = static_cast<unsigned char>(c);
    char e = detail::shellEscapeTable[v];
    if (e == 'E') {  // special 1-character escape
      out.append(&*last, p - last);
      esc[1] = c;
      out.append(esc, 2);
      ++p;
      last = p;
    } else if (e == 'n') {  // \n
      out.append(&*last, p - last);
      out.append("'\n'");
      ++p;
      last = p;
    } else {    // reserved
      ++p;
    }
  }
  out.append(&*last, p - last);
}

namespace detail {

template<class OutStringT, class OutputIterator>
void internalShellSplit(StringPiece sp, OutputIterator out) {
  assert(sp.empty() || sp.start() != nullptr);

  const char* s = sp.start();
  const size_t strSize = sp.size();

  bool escape = false;
  bool singleQuoted = false;
  bool doubleQuoted = false;
  size_t tokenStartPos = 0;
  size_t tokenSize = 0;
  OutStringT tmp;
  for (size_t i = 0; i < strSize; ++i) {
    if (s[i] == '\'' && !escape) {
      tmp.append(&s[tokenStartPos], tokenSize);
      tokenStartPos = i + 1;
      tokenSize = 0;
      singleQuoted = !singleQuoted;
    } else if (s[i] == '"' && !escape && !singleQuoted) {
      tmp.append(&s[tokenStartPos], tokenSize);
      tokenStartPos = i + 1;
      tokenSize = 0;
      doubleQuoted = !doubleQuoted;
    } else if (s[i] == '\\' && !escape && !singleQuoted) {
      tmp.append(&s[tokenStartPos], tokenSize);
      tokenStartPos = i + 1;
      tokenSize = 0;
      escape = true;
    } else if (s[i] == ' ' && !escape && !singleQuoted && !doubleQuoted) {
      tmp.append(&s[tokenStartPos], tokenSize);
      tokenStartPos = i + 1;
      tokenSize = 0;
      if (!tmp.empty()) {
        *out++ = tmp;
        tmp.clear();
      }
    } else {
      escape = false;
      ++tokenSize;
    }
  }
  tokenSize = strSize - tokenStartPos;
  tmp.append(&s[tokenStartPos], tokenSize);
  if (!tmp.empty()) {
    *out = tmp;
  }
}

} // namespace detail

template<class String>
void shellSplit(const String& input, std::vector<String>& out) {
  detail::internalShellSplit<String>(
    StringPiece(input),
    std::back_inserter(out));
}

}
}

#endif /* FOLLY_EXT_IO_FSUTIL_INL_H_ */

