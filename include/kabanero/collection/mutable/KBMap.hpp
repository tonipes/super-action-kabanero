#pragma once

#include "kabanero/collection/mutable/Map.hpp"

template<typename K, typename T>
class KBMap: public Dict<std::map, K, T> {

};
