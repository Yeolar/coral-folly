/*
 * Copyright 2015 Yeolar
 * Copyright 2015 Facebook, Inc.
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

#ifndef FOLLY_EXT_MAPUTIL_H_
#define FOLLY_EXT_MAPUTIL_H_

#include <type_traits>

namespace folly {

template <class Container, class Map>
typename std::enable_if<
  std::is_same<
    typename Container::value_type,
    typename Map::key_type>::value,
  Container>::type
get_keys(const Map& map) {
  Container container;
  for (const auto& kv : map) {
    container.insert(container.end(), kv.first);
  }
  return container;
}

template <class Container, class MapIt>
typename std::enable_if<
  std::is_same<
    typename Container::value_type,
    typename MapIt::value_type::first_type>::value,
  Container>::type
get_keys(MapIt first, MapIt last) {
  Container container;
  for (; first != last; ++first) {
    container.insert(container.end(), first->first);
  }
  return container;
}

template <class Container, class Map>
typename std::enable_if<
  std::is_same<
    typename Container::value_type,
    typename Map::mapped_type>::value,
  Container>::type
get_values(const Map& map) {
  Container container;
  for (const auto& kv : map) {
    container.insert(container.end(), kv.second);
  }
  return container;
}

template <class Container, class MapIt>
typename std::enable_if<
  std::is_same<
    typename Container::value_type,
    typename MapIt::value_type::second_type>::value,
  Container>::type
get_values(MapIt first, MapIt last) {
  Container container;
  for (; first != last; ++first) {
    container.insert(container.end(), first->second);
  }
  return container;
}

}  // namespace folly

#endif /* FOLLY_EXT_MAPUTIL_H_ */

