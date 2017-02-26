#ifndef SLT_CPP_MAP_UTILS_IMPL_H
#define SLT_CPP_MAP_UTILS_IMPL_H

namespace slt {

// Attempts to find the key in the map, failing that, this function will invoke 
// CB, insert its result in the map and return a reference to the inserted
// value.
template <typename CONTAINER_T, typename KEY_T, typename CB_T>
typename CONTAINER_T::mapped_type& findOrInsert(CONTAINER_T& map, KEY_T&& key,
                                                CB_T cb) {
  auto found = map.find(key);
  if(found == map.end()) {
    found = map.emplace(std::forward<KEY_T>(key), cb()).first;
  }

  return found->second;
}
}

#endif