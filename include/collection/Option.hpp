#pragma once

#include <iostream>
#include <vector>
#include <memory>

/**
 * Option class.
 */
template <typename T>
class Option {
public:
  Option(std::shared_ptr<T> valuePtr) : _isDefined(true), _valueStore(valuePtr) {}

  Option(T value) : _isDefined(true) {
    _valueStore = std::make_shared<T>(value);
  }

  Option() {}

  operator bool() const { return isDefined(); }

  /**
   * Returns true if this contains a value.
   * @return true if this contains a value otherwise false.
   */
  auto isDefined() const -> bool {
    return _isDefined;
  }

  /**
   * Returns true if this doesn't contain a value.
   * @see isDefined
   * @return false if value is not contained otherwise true.
   */
  auto isEmpty() const -> bool {
    return !_isDefined;
  }

  /**
   * Returns pointer to contained value.
   * @return pointer to contained value. Returns nullptr if no value is contained.
   */
   auto get() const -> const T& {
     return *_valueStore;
   }

   auto gets() -> T& {
     return *_valueStore;
   }

  /**
   * Returns pointer to contained value or default value.
   * @param v the default value.
   * @return pointer to contained value. Returns default value if no value is contained.
   */
  auto getOrElse(const T& v) const -> const T& {
    if (isDefined()) {
      return *_valueStore;
    } else {
      return v;
    }
  }

  /**
   * Applies a function to the contained value.
   * @param func function to apply to the contained value.
   */
  template <typename F>
  auto foreach(F func) const -> void {
    if (isDefined()) {
      func(*_valueStore);
    }
  }

  /**
   * Builds a new option by applying a function to the contained value.
   * @param func function to apply to the contained value.
   * @return A new option resulting from applying the function func to the element.
   */
  template <typename F, typename R = typename std::result_of<F&(T)>::type>
  auto map(F func) const -> const Option<R> {
    if (isDefined()) {
      return Option<R>(func(*_valueStore));
    } else {
      return Option<R>();
    }
  }

private:
  std::shared_ptr<T> _valueStore;
  bool _isDefined = false;
};


template <typename T>
auto Some(std::shared_ptr<T> value) -> Option<T> {
  return Option<T>(value);
}

template <typename T>
auto Some(T& value) -> Option<T> {
  return Option<T>(value);
}

template <typename T>
auto Some(const T& value) -> Option<T> {
  return Option<T>(value);
}

template <typename T>
auto Some(const T value[]) -> Option<const T*> {
  return Option<const T*>(&value[0]);
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
