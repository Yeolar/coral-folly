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

#ifndef FOLLY_EXT_FIXED_STREAM_H_
#define FOLLY_EXT_FIXED_STREAM_H_

#include <istream>
#include <ostream>
#include <streambuf>
#include <string>

namespace folly {

/**
 * A simple streambuf with fixed size buffer.
 * Convenient for limitting size of output; faster than ostringstream
 * and friends, which require heap allocations
 */
class FixedStreamBuf : public std::streambuf {
public:
  FixedStreamBuf(char *buf, size_t len) {
    setp(buf, buf + len);
  }

  FixedStreamBuf(const char *buf, const char *next, const char *end) {
    setg((char *)buf, (char *)next, (char *)end);
  }

  std::string str() { return std::string(pbase(), pptr()); }
};

/**
 * Output stream using a fixed buffer
 */
class FixedOstream : private virtual FixedStreamBuf, public std::ostream {
public:
  typedef FixedStreamBuf StreamBuf;

  FixedOstream(char *buf, size_t len)
  : FixedStreamBuf(buf, len)
  , std::ostream(static_cast<StreamBuf *>(this)) {
  }

  char *output() { return StreamBuf::pbase(); }
  char *output_ptr() { return StreamBuf::pptr(); }
  char *output_end() { return StreamBuf::epptr(); }

  std::string str() { return StreamBuf::str(); }
};

/**
 * Input stream using a fixed buffer
 */
class FixedIstream : private virtual FixedStreamBuf, public std::istream {
public:
  typedef FixedStreamBuf StreamBuf;

  FixedIstream(const char *buf, const char *end)
  : FixedStreamBuf(buf, buf, end)
  , std::istream(static_cast<StreamBuf *>(this)) {
  }

  char *input() { return StreamBuf::eback(); }
  char *input_ptr() { return StreamBuf::gptr(); }
  char *input_end() { return StreamBuf::egptr(); }
};

} // namespace folly

#endif /* FOLLY_EXT_FIXED_STREAM_H_ */

