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

#include <folly/ext/TypeDef.h>

#include <folly/Conv.h>
#include <folly/String.h>

namespace folly {

std::ostream& operator<<(std::ostream& os, const StringVec& vec) {
  if (vec.empty()) {
    return os << "[]";
  }
  return os << "[ " << join(", ", vec) << " ]";
}

std::ostream& operator<<(std::ostream& os, const StringSet& set) {
  if (set.empty()) {
    return os << "()";
  }
  return os << "( " << join(", ", set) << " )";
}

std::ostream& operator<<(std::ostream& os, const StringMap& map) {
  if (map.empty()) {
    return os << "{}";
  }
  std::string out;
  auto it = map.begin();
  toAppend("\t{ ", it->first, ": ", it->second, " }", &out);
  for (++it; it != map.end(); ++it) {
    toAppend(",\n\t{ ", it->first, ": ", it->second, " }", &out);
  }
  return os << "{\n" << out << "\n}";
}

} // namespace folly

