#pragma once

#include <cctype>
#include <string>
#include <vector>

namespace fla {

inline namespace {

template <class T>
inline void str_append(std::string& str, T value) {
    str += value;
}
template <>
inline void str_append(std::string& str, int value) {
    str += std::to_string(value);
}
template <>
inline void str_append(std::string& str, unsigned value) {
    str += std::to_string(value);
}
template <>
inline void str_append(std::string& str, long value) {
    str += std::to_string(value);
}
template <>
inline void str_append(std::string& str, unsigned long value) {
    str += std::to_string(value);
}
template <>
inline void str_append(std::string& str, long long value) {
    str += std::to_string(value);
}
template <>
inline void str_append(std::string& str, unsigned long long value) {
    str += std::to_string(value);
}
template <>
inline void str_append(std::string& str, float value) {
    str += std::to_string(value);
}
template <>
inline void str_append(std::string& str, double value) {
    str += std::to_string(value);
}
template <>
inline void str_append(std::string& str, long double value) {
    str += std::to_string(value);
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
        while (first < s.size() && (s[first] == ',' || std::isspace(s[first]))) {
            first++;
        }
        size_t last = first;
        while (last < s.size() && s[last] != ',' && !std::isspace(s[last])) {
            last++;
        }
        res.push_back(s.substr(first, last - first));
        first = last + 1;
    }
    return res;
}

} // namespace fla
