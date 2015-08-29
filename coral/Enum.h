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

#ifndef CORAL_ENUM_H_
#define CORAL_ENUM_H_

#include <type_traits>
#include <iostream>

namespace coral {

// enum class operator support.

template<typename T> using Underlying = typename std::underlying_type<T>::type;

template <class T>
typename std::enable_if<std::is_enum<T>::value, T>::type&
operator&=(T& lhs, T rhs) {
  return lhs = static_cast<T>(lhs & rhs);
}

template <class T>
typename std::enable_if<std::is_enum<T>::value, T>::type&
operator|=(T& lhs, T rhs) {
  return lhs = static_cast<T>(lhs | rhs);
}

template <class T>
typename std::enable_if<std::is_enum<T>::value, T>::type&
operator^=(T& lhs, T rhs) {
  return lhs = static_cast<T>(lhs ^ rhs);
}

template <class T>
typename std::enable_if<std::is_enum<T>::value, std::ostream>::type&
operator<<(std::ostream& os, const T x) {
  os << static_cast<Underlying<T>>(x);
  return os;
}

} // namespace coral

#endif /* CORAL_ENUM_H_ */

