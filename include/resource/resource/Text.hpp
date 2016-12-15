#pragma once

#include <iostream>
#include "resource/Resource.hpp"

/**
 * Text Resource.
 */
class Text : public Resource {
public:
  Text(std::string text) : _text(text) {}

  auto operator==(const std::string& s) -> bool {
    return s == _text;
  }

  auto operator==(const std::string& s) const -> bool {
    return s == _text;
  }

  auto operator==(const Text& t) -> bool {
    return t._text == _text;
  }

  auto operator==(const Text& t) const -> bool {
    return t._text == _text;
  }

  auto str() const -> const std::string& {
    return _text;
  }

  friend std::ostream& operator<<(std::ostream& os, Text text);

private:
  const std::string _text;
};

std::ostream& operator<<(std::ostream& os, Text text) {
  os << "Text( " << text._text << " )";
  return os;
}
