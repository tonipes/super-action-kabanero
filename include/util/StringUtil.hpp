#pragma once

#include "collection/mutable/KBVector.hpp"

auto split(const std::string& s, const char& c) -> KBVector<std::string>;
