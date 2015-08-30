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

#ifndef CORAL_PLATFORM_MAC_CFTYPE_WRAPPER_H_
#define CORAL_PLATFORM_MAC_CFTYPE_WRAPPER_H_

#ifdef __APPLE__

#include <vector>
#include <set>
#include <map>
#include <stdexcept>
#include <type_traits>
#include <boost/filesystem.hpp>

#include <CoreFoundation/CoreFoundation.h>

#include <coral/Conv.h>
#include <coral/Range.h>

namespace coral {

class CFStringWrapper {
public:
  CFStringWrapper(const StringPiece& sp) {
    CFStringRef cfstr =
    CFStringCreateWithBytes(kCFAllocatorDefault,
                            (UInt8*)sp.data(), sp.size(),
                            kCFStringEncodingUTF8, false);
    cfstr_.reset(cfstr, CFRelease);
  }

  CFStringRef get() const {
    return cfstr_.get();
  }

  // should release by self
  CFMutableStringRef getMutableCopy() const {
    return CFStringCreateMutableCopy(kCFAllocatorDefault, 0, cfstr_.get());
  }

  operator CFStringRef() const {
    return cfstr_.get();
  }

  explicit operator bool() const {
    return !!cfstr_.get();
  }

private:
  std::shared_ptr<const __CFString> cfstr_;
};

inline CFStringWrapper wrapToCFObject(const StringPiece& sp) {
  return CFStringWrapper(sp);
}

template <class String>
typename std::enable_if<
  std::is_convertible<String, StringPiece>::value ||
  IsSomeString<String>::value,
  CFStringWrapper>::type
wrapToCFObject(const String& str) {
  return CFStringWrapper(StringPiece(str));
}

class CFNumberWrapper {
public:
  CFNumberWrapper(long long i) {
    CFNumberRef cfn =
    CFNumberCreate(kCFAllocatorDefault, kCFNumberLongLongType, &i);
    cfn_.reset(cfn, CFRelease);
  }
  CFNumberWrapper(CFIndex i) {
    CFNumberRef cfn =
    CFNumberCreate(kCFAllocatorDefault, kCFNumberCFIndexType, &i);
    cfn_.reset(cfn, CFRelease);
  }

  CFNumberRef get() const {
    return cfn_.get();
  }

  operator CFNumberRef() const {
    return cfn_.get();
  }

  explicit operator bool() const {
    return !!cfn_.get();
  }

private:
  std::shared_ptr<const __CFNumber> cfn_;
};

template <class T>
typename std::enable_if<
  std::is_integral<T>::value, CFNumberWrapper>::type
wrapToCFObject(T i) {
  return CFNumberWrapper(static_cast<long long>(i));
}

inline CFNumberWrapper wrapToCFObject(CFIndex i) {
  return CFNumberWrapper(i);
}

class CFArrayWrapper {
public:
  template <class T>
  CFArrayWrapper(const std::vector<T>& v) {
    CFMutableArrayRef cfarray =
    CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
    for (auto& e : v) {
      CFArrayAppendValue(cfarray, wrapToCFObject(e));
    }
    cfarray_.reset(cfarray, CFRelease);
  }

  CFArrayRef get() const {
    return cfarray_.get();
  }

  operator CFArrayRef() const {
    return cfarray_.get();
  }

  explicit operator bool() const {
    return !!cfarray_.get();
  }

private:
  std::shared_ptr<const __CFArray> cfarray_;
};

template <class T>
CFArrayWrapper wrapToCFObject(const std::vector<T>& v) {
  return CFArrayWrapper(v);
}

class CFSetWrapper {
public:
  template <class T>
  CFSetWrapper(const std::set<T>& s) {
    CFMutableSetRef cfset =
    CFSetCreateMutable(kCFAllocatorDefault, 0, &kCFTypeSetCallBacks);
    for (auto& e : s) {
      CFSetAddValue(cfset, wrapToCFObject(e));
    }
    cfset_.reset(cfset, CFRelease);
  }

  CFSetRef get() const {
    return cfset_.get();
  }

  operator CFSetRef() const {
    return cfset_.get();
  }

  explicit operator bool() const {
    return !!cfset_.get();
  }

private:
  std::shared_ptr<const __CFSet> cfset_;
};

template <class T>
CFSetWrapper wrapToCFObject(const std::set<T>& s) {
  return CFSetWrapper(s);
}

class CFDictionaryWrapper {
public:
  template <class K, class V>
  CFDictionaryWrapper(const std::map<K, V>& d) {
    CFMutableDictionaryRef cfdict =
    CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                              &kCFTypeDictionaryKeyCallBacks,
                              &kCFTypeDictionaryValueCallBacks);
    for (auto& kv : d) {
      CFDictionaryAddValue(cfdict,
                           wrapToCFObject(kv.first),
                           wrapToCFObject(kv.second));
    }
    cfdict_.reset(cfdict, CFRelease);
  }

  CFDictionaryRef get() const {
    return cfdict_.get();
  }

  operator CFDictionaryRef() const {
    return cfdict_.get();
  }

  explicit operator bool() const {
    return !!cfdict_.get();
  }

private:
  std::shared_ptr<const __CFDictionary> cfdict_;
};

template <class K, class V>
CFDictionaryWrapper wrapToCFObject(const std::map<K, V>& d) {
  return CFDictionaryWrapper(d);
}

class CFURLWrapper {
public:
  CFURLWrapper(const boost::filesystem::path& p) {
    boost::filesystem::path::string_type s(p.string());
    CFURLRef cfurl =
    CFURLCreateFromFileSystemRepresentation(kCFAllocatorDefault,
                                            (UInt8*)s.data(), s.size(),
                                            boost::filesystem::is_directory(p));
    cfurl_.reset(cfurl, CFRelease);
  }

  CFURLRef get() const {
    return cfurl_.get();
  }

  operator CFURLRef() const {
    return cfurl_.get();
  }

  explicit operator bool() const {
    return !!cfurl_.get();
  }

private:
  std::shared_ptr<const __CFURL> cfurl_;
};

inline CFURLWrapper wrapToCFObject(const boost::filesystem::path& p) {
  return CFURLWrapper(p);
}

} // namespace coral

#endif

#endif /* CORAL_PLATFORM_MAC_CFTYPE_WRAPPER_H_ */

