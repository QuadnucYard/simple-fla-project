#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

template <class P>
std::string read_text(const P& path) {
    std::ifstream fin{path};
    if (!fin) {
        throw std::runtime_error{"fail to open file"};
    }
    std::stringstream stream;
    stream << fin.rdbuf();
    return stream.str();
}
