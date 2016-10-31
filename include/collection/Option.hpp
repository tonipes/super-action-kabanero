#pragma once

#include <iostream>
#include <vector>

template <typename T>
class Option {
public:
  Option(std::shared_ptr<T> valuePtr) : _isDefined(true), _valueStore(valuePtr) {}

  Option(T value) : _isDefined(true) {
    _valueStore = std::make_shared<T>(value);
  }

  Option() {}

  // `if(opt)` equals `if(opt.isDefined)`
  operator bool() const { return isDefined(); }

  auto isDefined() const -> bool {
    return _isDefined;
  }

  auto isEmpty() const -> bool {
    return !_isDefined;
  }

  auto get() const -> const T {
    return *_valueStore;
  }

  auto getOrElse(const T& v) const -> const T& {
    if (isDefined()) {
      return *_valueStore;
    } else {
      return v;
    }
  }

  template <typename F>
  auto foreach(F func) const -> void {
    if (isDefined()) {
      func(*_valueStore);
    }
  }

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
auto Some(std::shared_ptr<T>& value) -> Option<T> {
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
