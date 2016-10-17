#pragma once

#include <iostream>

template <typename T>
class Option {
public:
  Option(T& value) : valuePtr(&value) {}
  Option(const T& value) : valuePtr(&value) {}

  Option() : valuePtr(nullptr) {}

  // Now you can `if(opt)`. This equals to `if(opt.isDefined)`
  operator bool() const { return isDefined(); }

  auto isDefined() const -> bool {
    return valuePtr != nullptr;
  }

  auto isEmpty() const -> bool {
    return valuePtr == nullptr;
  }

  auto get() const -> const T& {
    return *valuePtr;
  }

  auto getOrElse(const T& v) -> const T& {
    if (valuePtr != nullptr) {
      return *valuePtr;
    } else {
      return v;
    }
  }

  template <typename F, typename R = typename std::result_of<F&(T)>::type>
  auto map(F func) -> const Option<R> {
    if (isDefined()) {
      return Option<R>(func(*valuePtr));
    } else {
      return Option<R>();
    }
  }

private:
  const T* valuePtr;
};

template <typename T>
auto Some(T& value) -> Option<T> {
  return Option<T>(value);
}

template <typename T>
auto Some(const T& value) -> Option<T> {
  return Option<T>(value);
}

template <typename T>
auto operator<<(std::ostream& os, const Option<T>& op) -> std::ostream& {
  if (op.isDefined()) {
    os << "Option(" << op.get() << ")";
  } else {
    os << "None";
  }
  return os;
}
