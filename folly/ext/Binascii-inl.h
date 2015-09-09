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

#ifndef FOLLY_EXT_BINASCII_INL_H_
#define FOLLY_EXT_BINASCII_INL_H_

#ifndef FOLLY_EXT_BINASCII_H_
#error This file may only be included from Binascii.h
#endif

namespace folly {

namespace detail {

extern const char b32DecodeTable[];
extern const char b64DecodeTable[];
extern const char r13DecodeTable[];

} // namespace detail

template <class String>
void base32Decode(StringPiece str, String& out) {
  uint32_t value = 0;
  int bits = 0;

  for (uint8_t c : str) {
    if (c > 0x7f) {
      continue;
    }
    c = detail::b32DecodeTable[c];
    if (c == (uint8_t)-1) {
      continue;
    }
    value = (value << 5) | c;
    bits += 5;
    while (bits >= 8) {
      bits -= 8;
      out.push_back((value >> bits) & 0xff);
    }
  }
}

template <class String>
void base64Decode(StringPiece str, String& out) {
  uint32_t value = 0;
  int bits = 0;

  for (uint8_t c : str) {
    if (c > 0x7f) {
      continue;
    }
    c = detail::b64DecodeTable[c];
    if (c == (uint8_t)-1) {
      continue;
    }
    value = (value << 6) | c;
    bits += 6;
    while (bits >= 8) {
      bits -= 8;
      out.push_back((value >> bits) & 0xff);
    }
  }
}

template <class String>
void rot13Decode(StringPiece str, String& out) {
  for (uint8_t c : str) {
    if (c > 0x7f) {
      out.push_back(c);
    } else {
      out.push_back(detail::r13DecodeTable[c]);
    }
  }
}

} // namespace folly

#endif /* FOLLY_EXT_BINASCII_INL_H_ */

