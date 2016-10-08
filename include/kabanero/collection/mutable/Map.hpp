#pragma once

#include <map>

#include "kabanero/Option.hpp"
#include "kabanero/collection/mutable/Seq.hpp"

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

  auto keys() -> VectorSeq<K> {
    VectorSeq<K> r;
    for(auto i : memory)
      r += i.first;
    return r;
  }

  auto values() -> VectorSeq<T> {
    VectorSeq<T> r;
    for(auto i : memory)
      r += i.second;
    return r;
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
class MapDict: public Dict<std::map, K, T> {

};
