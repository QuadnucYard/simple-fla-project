#pragma once

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

template <class T>
std::vector<T> to_ordered(const std::unordered_set<T>& set) {
    std::vector<T> vec(set.begin(), set.end());
    std::sort(vec.begin(), vec.end());
    return vec;
}

template <class K, class V>
std::vector<std::pair<K, V>> to_ordered(const std::unordered_map<K, V>& map) {
    std::vector<std::pair<K, V>> vec(map.begin(), map.end());
    std::sort(vec.begin(), vec.end(), [](auto& a, auto& b) { return a.first < b.first; });
    return vec;
}
