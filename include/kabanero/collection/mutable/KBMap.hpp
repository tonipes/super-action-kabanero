#pragma once

#include "kabanero/collection/mutable/Dictionary.hpp"

#include <unordered_map>

template<typename K, typename T>
class KBMap: public Dictionary<std::unordered_map, K, T, std::hash<K>, std::equal_to<K>> {

};
