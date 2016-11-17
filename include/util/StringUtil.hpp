#pragma once

#include "collection/mutable/KBVector.hpp"

const KBVector<std::string> split(const std::string& s, const char& c) {
	std::string buff{""};
	KBVector<std::string> v;

	for(auto n : s) {
		if (n != c) {
      buff += n;
    } else {
      if (n == c && buff != "") {
        v += (buff);
        buff = "";
      }
    }
	}
	if (buff != "") {
    v += buff;
  }

	return v;
}
