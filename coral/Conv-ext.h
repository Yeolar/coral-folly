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

#ifndef CORAL_CONV_EXT_H_
#define CORAL_CONV_EXT_H_

#include <array>
#include <type_traits>
#include <boost/filesystem.hpp>

namespace coral {

#ifdef __APPLE__

#include <CoreFoundation/CoreFoundation.h>

///////////////////////////////////////////////////////////////////////////
// CFStringRef, CFErrorRef to std::string

template <class Tgt>
typename std::enable_if<IsSomeString<Tgt>::value>::type
toAppend(CFStringRef value, Tgt * result) {
  if (!value) {
    return;
  }
  const char* str = CFStringGetCStringPtr(value, kCFStringEncodingUTF8);
  if (str) {
    result->append(str);
    return;
  }
  CFIndex n;
  CFStringGetBytes(value, CFRangeMake(0, CFStringGetLength(value)),
                   kCFStringEncodingUTF8, 0, false,
                   NULL, 0, &n);
  if (n < 128) {
    std::array<char, 128> inline_buffer;
    Boolean r = CFStringGetCString(value,
                                   inline_buffer.data(),
                                   inline_buffer.size(),
                                   kCFStringEncodingUTF8);
    if (r) {
      result->append(inline_buffer.data());
    } else {
      throw std::runtime_error("CFStringGetCString error");
    }
  } else {
    std::unique_ptr<char[]> heap_buffer(new char[n + 1]);
    Boolean r = CFStringGetCString(value,
                                   heap_buffer.get(),
                                   n + 1,
                                   kCFStringEncodingUTF8);
    if (r) {
      result->append(heap_buffer.get());
    } else {
      throw std::runtime_error("CFStringGetCString error");
    }
  }
}

template <class Tgt>
typename std::enable_if<IsSomeString<Tgt>::value>::type
toAppend(CFErrorRef value, Tgt * result) {
  CFStringRef errStr = CFErrorCopyDescription(value);
  toAppend(errStr, result);
  CFRelease(errStr);
}

#endif

///////////////////////////////////////////////////////////////////////////
// path to std::string

template <class Tgt>
typename std::enable_if<IsSomeString<Tgt>::value>::type
toAppend(const boost::filesystem::path& value, Tgt * result) {
  result->append(value.string());
}

} // namespace coral

#endif /* CORAL_CONV_EXT_H_ */

