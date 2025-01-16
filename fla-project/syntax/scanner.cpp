#include "scanner.hpp"
#include <cctype>

namespace fla {

std::optional<std::string_view> Scanner::next_line() {
    while (true) {
        if (cursor >= input.size()) {
            return std::nullopt;
        }
        line_++;
        auto start = cursor;
        while (cursor < input.size() && input[cursor] != '\n') {
            cursor++;
        }
        auto end = cursor;
        // trim
        while (start < end && std::isspace(input[start])) {
            start++;
        }
        while (end > start && std::isspace(input[end - 1])) {
            end--;
        }
        auto res = input.substr(start, end - start);
        cursor++; // eat linebreak
        if (auto i = res.find(';'); i != std::string_view::npos) {
            res = res.substr(0, i);
        }
        if (res.empty()) {
            continue;
        }
        return res;
    }
}

} // namespace fla
