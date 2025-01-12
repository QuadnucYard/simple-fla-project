#include "syntax.hpp"
#include "../syntax/err.hpp"
#include "../utils/str.hpp"
#include "pda.hpp"

#include <algorithm>
#include <regex>
#include <string>

namespace fla::pda {

Symbol as_symbol(std::string_view s) {
    if (s.size() != 1) {
        throw SyntaxError(concat("\"", s, "\" is not a valid symbol"));
    }
    return s[0] == '_' ? Pda::NULL_SYMBOL : s[0];
}

Pda Parser::parse() {
    using svmatch = std::match_results<std::string_view::const_iterator>;

    static const std::regex q_regex{R"(^#Q\s*=\s*\{(.+)\}$)"};
    static const std::regex s_regex{R"(^#S\s*=\s*\{(.+)\}$)"};
    static const std::regex g_regex{R"(^#G\s*=\s*\{(.+)\}$)"};
    static const std::regex q0_regex{R"(^#q0\s*=\s*(\w+)$)"};
    static const std::regex z0_regex{R"(^#z0\s*=\s*(\w+)$)"};
    static const std::regex f_regex{R"(^#F\s*=\s*\{(.+)\}$)"};
    static const std::regex delta_regex{R"(^(\w+)\s+(\S)\s+(\S)\s+(\w+)\s+(\S+)\s*$)"};

    Pda pda;
    while (auto line = scanner.next_line()) {
        if (svmatch match; std::regex_search(line->begin(), line->end(), match, q_regex)) {
            for (auto q : comma_separated(match[1])) {
                pda.states.insert(std::string{q});
            }
        } else if (svmatch match; std::regex_search(line->begin(), line->end(), match, s_regex)) {
            for (auto s : comma_separated(match[1])) {
                pda.input_symbols.insert(as_symbol(s));
            }
        } else if (svmatch match; std::regex_search(line->begin(), line->end(), match, g_regex)) {
            for (auto s : comma_separated(match[1])) {
                pda.stack_symbols.insert(as_symbol(s));
            }
        } else if (svmatch match; std::regex_search(line->begin(), line->end(), match, q0_regex)) {
            pda.start_state = match[1];
        } else if (svmatch match; std::regex_search(line->begin(), line->end(), match, z0_regex)) {
            pda.start_stack_symbol = as_symbol(match[1].str());
        } else if (svmatch match; std::regex_search(line->begin(), line->end(), match, f_regex)) {
            for (auto q : comma_separated(match[1])) {
                pda.final_states.insert(std::string{q});
            }
        } else if (svmatch match;
                   std::regex_search(line->begin(), line->end(), match, delta_regex)) {
            auto push_symbols{match[5] == "_" ? std::string{""} : match[5]};
            std::reverse(push_symbols.begin(), push_symbols.end());
            pda.transitions.emplace(
                TransitionKey{match[1], as_symbol(match[2].str()), as_symbol(match[3].str())},
                TransitionValue{match[4], push_symbols});
        } else {
            throw SyntaxError{concat("invalid line: ", *line)};
        }
    }

    return pda;
}

} // namespace fla::pda
