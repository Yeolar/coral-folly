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

#ifndef FOLLY_EXT_REGEX_H_
#define FOLLY_EXT_REGEX_H_

#if FOLLY_HAVE_LIBONIG

#include <oniguruma.h>

#include <folly/Range.h>
#include <folly/ext/NumericRange.h>

namespace folly {

class Regex {
public:
  Regex() : pattern_("(?=un)initialized") { }

  Regex(StringPiece pattern, OnigOptionType options = ONIG_OPTION_NONE);

  bool isValid() const {
    return re_ != nullptr;
  }

  std::shared_ptr<OnigRegexType> regex() const { return re_; }

  const std::string& str() const { return pattern_; }

  explicit operator bool() const { return bool(re_); }

private:
  std::shared_ptr<OnigRegexType> re_;
  std::string pattern_;
};

inline bool operator==(const Regex& lhs, const Regex& rhs) {
  return lhs.str() == rhs.str();
}
inline bool operator!=(const Regex& lhs, const Regex& rhs) {
  return !(lhs == rhs);
}

class Match {
public:
  Match() { }

  Match(const StringPiece& sp,
        const std::shared_ptr<OnigRegexType>& regex,
        const std::shared_ptr<OnigRegion>& region)
    : buffer_(sp)
    , re_(regex)
    , region_(region) {
  }

  size_t size() const {
    return region_ ? region_->num_regs : 0;
  }

  bool matched(size_t i = 0) const {
    return i < size() && region_->beg[i] != -1;
  }

  ptrdiff_t begin(size_t i = 0) const {
    return i < size() ? region_->beg[i] : -1;
  }

  ptrdiff_t end(size_t i = 0) const {
    return i < size() ? region_->end[i] : -1;
  }

  bool empty(size_t i = 0) const {
    return begin(i) == end(i);
  }

  SizeRange operator[](size_t i) const {
    if (matched(i)) {
      return SizeRange(begin(i), end(i));
    } else {
      return SizeRange();
    }
  }

  const StringPiece& origin() const { return buffer_; }

  StringPiece matchPiece(size_t i) const {
    return buffer_.subpiece((*this)[i]);
  }

  explicit operator bool() const { return bool(region_); }

  std::map<std::string, std::string> captures() const;
  std::multimap<std::string, SizeRange> captureIndices() const;

private:
  StringPiece buffer_;
  std::shared_ptr<OnigRegexType> re_;
  std::shared_ptr<OnigRegion> region_;
};

bool regexValidate(StringPiece pattern, std::string* error = nullptr);

bool regexSearch(StringPiece sp, Match& match, const Regex& regex,
                 const char* from = nullptr,
                 const char* to = nullptr,
                 OnigOptionType options = ONIG_OPTION_NONE);

} // namespace folly

#endif

#endif /* FOLLY_EXT_REGEX_H_ */

