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

#ifndef CORAL_MAPUTIL_H_
#define CORAL_MAPUTIL_H_

#include <coral/Conv.h>
#include <coral/Optional.h>

namespace coral {

/**
 * Given a map and a key, return the value corresponding to the key in the map,
 * or a given default value if the key doesn't exist in the map.
 */
template <class Map>
typename Map::mapped_type get_default(
    const Map& map, const typename Map::key_type& key,
    const typename Map::mapped_type& dflt =
    typename Map::mapped_type()) {
  auto pos = map.find(key);
  return (pos != map.end() ? pos->second : dflt);
}

/**
 * Given a map and a key, return the value corresponding to the key in the map,
 * or throw an exception of the specified type.
 */
template <class E = std::out_of_range, class Map>
typename Map::mapped_type get_or_throw(
    const Map& map, const typename Map::key_type& key,
    const std::string& exceptionStrPrefix = std::string()) {
  auto pos = map.find(key);
  if (pos != map.end()) {
    return pos->second;
  }
  throw E(coral::to<std::string>(exceptionStrPrefix, key));
}

/**
 * Given a map and a key, return a Optional<V> if the key exists and None if the
 * key does not exist in the map.
 */
template <class Map>
coral::Optional<typename Map::mapped_type> get_optional(
    const Map& map, const typename Map::key_type& key) {
  auto pos = map.find(key);
  if (pos != map.end()) {
    return coral::Optional<typename Map::mapped_type>(pos->second);
  } else {
    return coral::none;
  }
}

/**
 * Given a map and a key, return a reference to the value corresponding to the
 * key in the map, or the given default reference if the key doesn't exist in
 * the map.
 */
template <class Map>
const typename Map::mapped_type& get_ref_default(
    const Map& map, const typename Map::key_type& key,
    const typename Map::mapped_type& dflt) {
  auto pos = map.find(key);
  return (pos != map.end() ? pos->second : dflt);
}

/**
 * Given a map and a key, return a pointer to the value corresponding to the
 * key in the map, or nullptr if the key doesn't exist in the map.
 */
template <class Map>
const typename Map::mapped_type* get_ptr(
    const Map& map, const typename Map::key_type& key) {
  auto pos = map.find(key);
  return (pos != map.end() ? &pos->second : nullptr);
}

/**
 * Non-const overload of the above.
 */
template <class Map>
typename Map::mapped_type* get_ptr(
    Map& map, const typename Map::key_type& key) {
  auto pos = map.find(key);
  return (pos != map.end() ? &pos->second : nullptr);
}

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

}  // namespace coral

#endif /* CORAL_MAPUTIL_H_ */

