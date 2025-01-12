#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace fla {

inline namespace {

template <class T>
inline void str_append(std::string& str, T value) {
    str += std::to_string(value);
}
template <>
inline void str_append(std::string& str, std::string& value) {
    str += value;
}
template <>
inline void str_append(std::string& str, std::string_view value) {
    str += value;
}
template <>
inline void str_append(std::string& str, const char* value) {
    str += value;
}

} // namespace

template <class... Ts>
inline std::string concat(Ts&&... args) {
    std::string s;
    ((str_append(s, args)), ...);
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
