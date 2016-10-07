#pragma once

#include <functional>
#include <vector>
#include <list>
#include <iostream>

template <template <class T, class Allocator = std::allocator<T>> class C, typename T>
class Seq {
public:
  Seq() {}

  typedef C<T> Memory;
  typedef std::function<void(T)> lambda;
  typedef typename Memory::iterator iterator;
  typedef typename Memory::const_iterator const_iterator;

  auto length() -> size_t {
    return memory.size();
  }

  auto foreach(const lambda& func) const -> void {
    for (auto const& i : memory) {
      func(i);
    }
  }

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

  auto operator[](size_t index) -> T& {
    return memory[index];
  }

  auto operator[](size_t index) const -> const T& {
    return memory[index];
  }

  auto operator+=(T elem) -> void {
    memory.push_back(elem);
  };

  template <typename F, typename R = typename std::result_of<F&(T)>::type>
  auto map(F func) const -> Seq<C, R> {
    auto a = Seq<C, R>();
    for (auto const& i : memory) {
      a += func(i);
    }
    return a;
  }

  template <typename F>
  auto fold(T const& start, F func) -> T {
    auto s = start;
    for (auto const& i : memory) {
      s = func(s, i);
    }
    return s;
  }

protected:
  Memory memory;
};

template <template <class T, class Allocator> class C, typename T>
auto operator<<(std::ostream& os, const Seq<C, T>& collection) -> std::ostream& {
  os << "[ ";
  for (auto it = collection.begin(); it != collection.end(); it++) {
    os << *it;
    if (it != (--collection.end())) {
      os << ", ";
    }
  }
  os << " ]";
  return os;
}

template<typename T>
class VectorSeq : public Seq<std::vector, T> {};

template<typename T>
class ListSeq : public Seq<std::list, T> {};
