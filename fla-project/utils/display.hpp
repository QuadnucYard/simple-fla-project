#pragma once

#include "utils/ordered.hpp"
#include <ostream>
#include <unordered_set>

namespace fla {

template <class T>
std::ostream& operator<<(std::ostream& os, const std::unordered_set<T>& set) {
    auto vec = to_ordered(set);
    os << "{";
    auto it = vec.begin();
    if (it != vec.end()) {
        os << *it++;
    }
    while (it != vec.end()) {
        os << ", " << *it++;
    }
    os << "}";
    return os;
}

} // namespace fla
