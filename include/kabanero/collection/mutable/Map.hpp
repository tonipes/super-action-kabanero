#pragma once

#include <map>

#include "kabanero/Option.hpp"
#include "kabanero/collection/mutable/KBVector.hpp"

template <template<
    class K,
    class T,
    class Compare = std::less<K>,
    class Allocator = std::allocator< std::pair<const K, T> >
  > class M, typename K, typename T>
class Dict {
public:
  typedef M<K, T> Memory;
  typedef typename Memory::iterator iterator;
  typedef typename Memory::const_iterator const_iterator;

  auto begin() -> iterator {
    return memory.begin();
  }

  auto end() -> iterator {
    return memory.end();
  }

  auto begin() const -> const_iterator {
    return memory.cbegin();
  }

  auto end() const -> const_iterator {
    return memory.cend();
  }

  auto operator[](const K& key) -> T& {
    return memory[key];
  }

  // You get a const! And You get const! Everybody gets a const!
  auto contains(const K& key) const -> const bool {
    return memory.find(key) != memory.end();
  }

  auto keys() -> KBVector<K> {
    KBVector<K> r;
    for(auto i : memory)
      r += i.first;
    return r;
  }

  auto values() -> KBVector<T> {
    KBVector<T> r;
    for(auto i : memory)
      r += i.second;
    return r;
  }

  template <typename F>
  auto foreach(F func) -> void {
    for (auto& i : memory) {
      func(i);
    }
  }

  // Func is type f(std::pair<Key,Value>) -> std::pair(NewKey, NewValue)
  template <typename F,
    typename R = typename std::result_of<F&(std::pair<K, T>)>::type,
    typename NK = typename R::first_type, typename NV = typename R::second_type
  >
  auto map(F func) const -> Dict<M, NK, NV> {
    auto a = Dict<M, NK, NV>();
    for (auto const& i : memory) {
      auto got = func(i);
      a[got.first] = got.second;
    }
    return a;
  }

  auto get(const K& key) -> Option<T> {
    if(memory.find(key) != memory.end()) {
      return Some(memory[key]);
    } else {
      return Option<T>();
    }
  }

  auto getOrElse(const K& key, const T& v) -> const T& {
    return get(key).getOrElse(v);
  }

protected:
  Memory memory;

};

template <template<
    class K,
    class T,
    class Compare = std::less<K>,
    class Allocator = std::allocator< std::pair<const K, T> >
  > class M, typename K, typename T>

auto operator<<(std::ostream& os, const Dict<M, K, T>& collection) -> std::ostream& {
  os << "{ ";
  for (auto it = collection.begin(); it != collection.end(); it++) {
    os << "("<< it->first << " => " << it->second << ")";
    if (it != (--collection.end())) {
      os << ", ";
    }
  }
  os << " }";
  return os;
}

template<typename K, typename T>
class KBMap: public Dict<std::map, K, T> {

};
