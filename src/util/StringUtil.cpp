#include "util/StringUtil.hpp"

auto split(const std::string& s, const char& c) -> KBVector<std::string> {
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
