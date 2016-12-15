#pragma once

#include "collection/mutable/Dictionary.hpp"

#include <unordered_map>

/**
 * Unordered dictionary 
 */
template<typename K, typename T>
class KBMap: public Dictionary<std::unordered_map, K, T, std::hash<K>, std::equal_to<K>> {

};
