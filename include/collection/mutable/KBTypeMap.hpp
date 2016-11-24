#pragma once

#include "collection/mutable/Dictionary.hpp"
#include "exception/EngineException.hpp"

#include <unordered_map>
#include <type_traits>
#include <typeinfo>

using TypeInfoRef = std::reference_wrapper<const std::type_info>;

struct Hasher {
  auto operator()(TypeInfoRef code) const -> size_t {
    return code.get().hash_code();
  }
};

struct EqualTo {
  auto operator()(TypeInfoRef lhs, TypeInfoRef rhs) const -> bool {
    return lhs.get() == rhs.get();
  }
};

template <typename T>
class KBTypeMap: public Dictionary<std::unordered_map, TypeInfoRef, T, Hasher, EqualTo> {
public:

  template <typename A>
  auto get() const -> Option<T> {
    auto it = this->memory.find(typeid(A));
    if (it != this->memory.end()) {
      return Option<T>((*it).second);
    } else {
      return Option<T>();
      throw EngineException("Value: " + std::string(typeid(A).name()) + " not found");
    }
  }
private:
  bool isPointerType;
};
