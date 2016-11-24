#pragma once

#include <map>

#include "collection/Option.hpp"
#include "collection/mutable/KBVector.hpp"

template <template<
    class K,
    class T,
    class Hash,
    class KeyEqual,
    class Allocator = std::allocator< std::pair<const K, T> >
  > class M, typename K, typename T, typename Hash, typename KeyEqual>
class Dictionary {
public:
  typedef M<K, T, Hash, KeyEqual> Memory;
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

  auto operator[](K&& key) -> T& {
    return memory[key];
  }

  auto insert(const K& key, T value) -> void {
    memory.emplace(key, value);
  }

  auto remove(const K& key) -> void {
    memory.erase(key);
  }

  // You get a const! And You get const! Everybody gets a const!
  auto contains(const K& key) const -> const bool {
    return memory.find(key) != memory.end();
  }

  auto keys() const -> KBVector<K> {
    KBVector<K> r;
    for(auto i : memory)
      r += i.first;
    return r;
  }

  auto values() const -> KBVector<T> {
    KBVector<T> r;
    for(auto i : memory)
      r += i.second;
    return r;
  }

  auto length() const -> size_t {
    return keys().length();
  }

  auto isEmpty() const -> bool {
    return length() == 0;
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
  auto map(F func) const -> Dictionary<M, NK, NV, Hash, KeyEqual> {
    auto a = Dictionary<M, NK, NV, Hash, KeyEqual>();
    for (auto const& i : memory) {
      auto got = func(i);
      a[got.first] = got.second;
    }
    return a;
  }

  auto get(const K& key) -> Option<T> {
    auto it = memory.find(key);
    if (it != memory.end()) {
      return Option<T>((*it).second);
    } else {
      return Option<T>();
    }
  }

  auto get(const K& key) const -> Option<T> {
    auto it = memory.find(key);
    if (it != memory.end()) {
      return Option<T>((*it).second);
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
    class Hash,
    class KeyEqual,
    class Allocator = std::allocator< std::pair<const K, T> >
  > class M, typename K, typename T, typename Hash, typename KeyEqual>

auto operator<<(std::ostream& os, const Dictionary<M, K, T, Hash, KeyEqual>& collection) -> std::ostream& {
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
