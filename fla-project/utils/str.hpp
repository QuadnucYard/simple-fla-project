#pragma once

#include <string>
#include <vector>

namespace fla {

template <class... Ts>
inline std::string concat(Ts&&... args) {
    std::string s;
    ((s += args), ...);
    return s;
}

inline std::vector<std::string> comma_separated(const std::string& s) {
    std::vector<std::string> res;
    std::size_t first = 0;
    while (first < s.size()) {
        size_t last = first;
        while (last < s.size() && s[last] != ',') {
            last++;
        }
        res.push_back(s.substr(first, last - first));
        first = last + 1;
    }
    return res;
}

} // namespace fla
