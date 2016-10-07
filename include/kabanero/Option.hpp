#pragma once

#include <iostream>

template <typename T>
class Option {
public:
  Option(T& value) : valuePtr(&value) {}
  Option(const T& value) : valuePtr(&value) {}

  Option() : valuePtr(nullptr) {}

  auto isDefined() const -> bool {
    return valuePtr != nullptr;
  }

  auto isEmpty() const -> bool {
    return valuePtr == nullptr;
  }

  auto get() const -> const T& {
    return *valuePtr;
  }

  auto getOrElse(T& v) -> T& {
    if (valuePtr != nullptr) {
      return *valuePtr;
    } else {
      return v;
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
