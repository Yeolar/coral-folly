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

#ifndef FOLLY_EXT_NUMERIC_RANGE_H_
#define FOLLY_EXT_NUMERIC_RANGE_H_

#include <type_traits>
#include <utility>
#include <sys/types.h>
#include <boost/operators.hpp>

namespace folly {

template <class T>
struct NumericRange : private boost::totally_ordered<NumericRange<T>> {
  typedef T value_type;

  T begin;
  T end;

  constexpr NumericRange()
    : begin(), end() { }

  NumericRange(const T& b, const T& e)
    : begin(b), end(e) { }

  template<class U>
  NumericRange(const NumericRange<U>& r,
               typename std::enable_if<
                 std::is_convertible<const U&, T>::value>::type* = 0)
    : begin(r.begin), end(r.end) { }

  template<class U>
  NumericRange(NumericRange<U>&& r,
               typename std::enable_if<
                 std::is_convertible<U, T>::value>::type* = 0)
    : begin(r.begin), end(r.end) { }

  NumericRange(const NumericRange& r) = default;
  NumericRange(NumericRange&& r) = default;

  NumericRange& operator=(const NumericRange& r) noexcept {
    begin = r.begin;
    end = r.end;
    return *this;
  }

  NumericRange& operator=(NumericRange&& r) noexcept {
    begin = r.begin;
    end = r.end;
    return *this;
  }

  void swap(NumericRange& r) noexcept {
    std::swap(begin, r.begin);
    std::swap(end, r.end);
  }

  bool isReversed() const { return begin > end; }

  T size() const {
    return begin > end ? begin - end : end - begin;
  }

  bool contains(const T& pos) const {
    return pos > begin && pos < end;
  }

  bool contains(const NumericRange& r) const {
    return begin < r.begin && end > r.end;
  }
};

template <class T>
inline bool operator==(const NumericRange<T>& x, const NumericRange<T>& y) {
  return x.begin == y.begin && x.end == y.end;
}

template <class T>
inline bool operator<(const NumericRange<T>& x, const NumericRange<T>& y) {
  return x.begin < y.begin || (!(y.begin < x.begin) && x.end < y.end);
}

template <class T>
void swap(NumericRange<T>& x, NumericRange<T>& y) {
  x.swap(y);
}

template <class T>
NumericRange<T> numericRange(const T& b, const T& e) {
  return NumericRange<T>(b, e);
}

typedef NumericRange<size_t> SizeRange;
typedef NumericRange<ssize_t> SSizeRange;

} // namespace folly

#endif /* FOLLY_EXT_NUMERIC_RANGE_H_ */

