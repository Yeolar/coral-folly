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

#ifndef FOLLY_EXT_TYPE_DEF_H_
#define FOLLY_EXT_TYPE_DEF_H_

#include <string>
#include <vector>
#include <set>
#include <map>

#include <folly/Hash.h>

namespace folly {

typedef std::basic_string<unsigned char> byte_string;
typedef std::vector<std::string> StringVec;
typedef std::set<std::string> StringSet;
typedef std::map<std::string, std::string> StringMap;

std::ostream& operator<<(std::ostream& os, const byte_string& str);
std::ostream& operator<<(std::ostream& os, const StringVec& vec);
std::ostream& operator<<(std::ostream& os, const StringSet& set);
std::ostream& operator<<(std::ostream& os, const StringMap& map);

} // namespace folly

namespace std {
  template <>
  struct hash<folly::byte_string> {
    size_t operator()(const folly::byte_string& s) const {
      return folly::hash::fnv32_buf(s.data(), s.size());
    }
  };
} // namespace std

#endif /* FOLLY_EXT_TYPE_DEF_H_ */

