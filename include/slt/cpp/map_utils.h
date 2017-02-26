#ifndef SLT_CPP_MAP_UTILS_H
#define SLT_CPP_MAP_UTILS_H

namespace slt {

// Attempts to find the key in the map, failing that, this function will invoke 
// CB, insert its result in the map and return a reference to the inserted
// value.
template <typename CONTAINER_T, typename KEY_T, typename CB_T>
inline typename CONTAINER_T::mapped_type& findOrInsert(CONTAINER_T& map,
                                                       KEY_T&& key, CB_T cb);
}

#include "slt/cpp/details/map_utils_impl.h"

#endif