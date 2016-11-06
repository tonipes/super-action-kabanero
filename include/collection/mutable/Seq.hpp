#pragma once

#include <functional>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
#include <tuple>

#include "collection/Option.hpp"
#include "util/PrintUtil.hpp"

/**
 * Sequence class.
 * The base for all sequence like collections.
 */
template <template <class T, class Allocator = std::allocator<T>> class C, typename T>
class Seq {
public:
  Seq() {}

  /**
   * Constructor for initializer list.
   */
  Seq(std::initializer_list<T> init) : memory(init) {}

  typedef C<T> Memory;
  typedef typename Memory::iterator iterator;
  typedef typename Memory::const_iterator const_iterator;

  /**
   * Returns the lenght of this collection.
   * @return lenght of this collection.
   */
  auto length() const -> size_t {
    return memory.size();
  }

  /**
   * Returns true if this collection is empty.
   * @return true if this collection is empty otherwise false.
   */
  auto isEmpty() const -> bool {
    return length() == 0;
  }

  /**
   * Applies a function to the values in this collection.
   * @param func function to apply to the values.
   */
  template <typename F>
  auto foreach(const F& func) const -> void {
    for (auto const& i : memory) {
      func(i);
    }
  }

  /**
   * Applies a function to the values in this collection.
   * @param func function to apply to the values.
   */
  template <typename F>
  auto foreach(F func) -> void {
    for (auto& i : memory) {
      func(i);
    }
  }

  /**
   * Returns the begin of the iterator.
   * @return begin of the iterator.
   */
  auto begin() -> iterator {
    return memory.begin();
  }

  /**
   * Returns the end of the iterator.
   * @return end of the iterator.
   */
  auto end() -> iterator {
    return memory.end();
  }

  /**
   * Returns the begin of the iterator.
   * @return begin of the iterator.
   */
  auto begin() const -> const_iterator {
    return memory.cbegin();
  }

  /**
   * Returns the end of the iterator.
   * @return end of the iterator.
   */
  auto end() const -> const_iterator {
    return memory.cend();
  }

  /**
   * Returns reference to the element at the index.
   * @param index index of the value.
   * @return reference to the value at the index.
   */
  auto operator[](size_t index) -> T& {
    return memory[index];
  }

  /**
   * Returns reference to the element at the index.
   * @param index index of the value.
   * @return reference to the value at the index.
   */
  auto operator[](size_t index) const -> const T& {
    return memory[index];
  }

  /**
   * Add element to this collection.
   * @param elem element to add.
   */
  auto operator+=(const T& elem) -> void {
    memory.push_back(elem);
  };

  /**
   * Add element to this collection.
   * @param elem element to add.
   */
  auto operator+=(T&& elem) -> void {
    memory.push_back(std::move(elem));
  };

  /**
   * Comapares collections element-wise.
   * @param other collection to compare with this collection.
   * @return true if all elements match, otherwise false.
   */
  auto operator==(const Seq& other) {
    return length() == other.length() && ([&](){
      for (auto i = 0; i < length(); i++) {
        if ((*this)[i] != other[i]) {
          return false;
        }
      }
      return true;
    })();
  }

  /**
   * Builds a new option by applying a function to the elements of this collection.
   * @param func function to apply to the elements of the collection.
   * @return A new collection resulting from applying the function func to the elements of this collection.
   */
  template <typename F, typename R = typename std::result_of<F&(T)>::type>
  auto map(F func) const -> Seq<C, R> {
    auto a = Seq<C, R>();
    for (auto const& i : memory) {
      a += func(i);
    }
    return a;
  }

  /**
   * Folds the elements of this collection using the specified function.
   * @param start first element for the fold operation.
   * @param func function to fold this collection with.
   * @return a new collection resulting from folding the function func between the elements of this collection.
   */
  template <typename F>
  auto fold(T const& start, F func) -> T {
    auto s = start;
    for (auto const& i : memory) {
      s = func(s, i);
    }
    return s;
  }

  /**
   * Finds the first element that matches the given function.
   * @param func function to test elements with.
   * @return an option value that contains the found value.
   */
  template <typename F>
  auto find(F func) const -> const Option<T> {
    const auto it = std::find_if(begin(), end(), func);
    if (it == end()) {
      return Option<T>();
    } else {
      return Some(*it);
    }
  }

  /**
   * Checks if there is an element that matches the given function.
   * @param func function to test elements with.
   * @return true if element is found, otherwise false.
   */
  template <typename F>
  auto exists(F func) const -> bool {
    const auto it = std::find_if(begin(), end(), func);
    return it != end();
  }

  template <typename F>
  auto sortBy(F func) const -> Seq<C, T> {

  }

  /**
   * Returns a new collection with all elements that matches the given function.
   * @param func function to test elements with.
   * @return a new collection with all elements that matches the given function.
   */
  template <typename F>
  auto filter(F func) const -> Seq<C, T> {
    auto a = Seq<C, T>();
    for (auto const& i : memory) {
      if (func(i)) {
        a += i;
      }
    }
    return a;
  }

  /**
   * Removes an element at index.
   * @param i the index to be removed.
   * @return the removed element.
   */
  auto remove(const int i) -> const T {
    const auto r = memory[i];
    memory.erase(memory.begin() + i);
    return r;
  }

  /**
   * Removes all elements that matches the given function.
   * @param func function to test elements with.
   */
  template <typename F>
  auto remove(F func) -> void {
    memory.erase(std::remove_if(memory.begin(), memory.end(), func));
  }

  /**
   * Returns a new collection with all elements paired by index with elements from the other collection.
   * @param other collection to pair with.
   * @return a new collection with all elements paired by index with elements from the other collection.
   */
  template <template <class R, class Allocator = std::allocator<T>> class C2, typename R>
  auto zip(const Seq<C2, R>& other) const -> Seq<C, std::tuple<T, R>> {
    auto r = Seq<C, std::tuple<T, R>>();
    auto minLength = std::min(this->length(), other.length());
    for (auto i = 0; i < minLength; ++i) {
      r += std::make_tuple((*this)[i], other[i]);
    }
    return r;
  }

  /**
   * Converts this collection to std::vector
   * @return a vector containing all elements of this collection.
   */
  auto toVector() -> std::vector<T> {
    return std::vector<T>(begin(), end());
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
