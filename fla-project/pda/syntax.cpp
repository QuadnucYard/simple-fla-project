#include "syntax.hpp"
#include "../syntax/err.hpp"
#include "../utils/str.hpp"
#include "pda.hpp"

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>

using svmatch = std::match_results<std::string_view::const_iterator>;

namespace fla::pda {

Symbol as_symbol(std::string_view s) {
    if (s.size() != 1) {
        throw SyntaxError(concat("\"", s, "\" is not a valid symbol"));
    }
    return s[0] == '_' ? Pda::NULL_SYMBOL : s[0];
}

Pda Parser::parse() {
    static const std::regex q_regex{R"(^#Q = \{(.+)\}$)"};
    static const std::regex s_regex{R"(^#S = \{(.+)\}$)"};
    static const std::regex g_regex{R"(^#G = \{(.+)\}$)"};
    static const std::regex q0_regex{R"(^#q0 = (\w+)$)"};
    static const std::regex z0_regex{R"(^#z0 = (\w+)$)"};
    static const std::regex f_regex{R"(^#F = \{(.+)\}$)"};
    static const std::regex delta_regex{R"(^(\w+) (\S) (\S) (\w+) (\S+)\s*$)"};

    Pda pda;
    while (auto line = scanner.next_line()) {
        std::cout << "line: " << *line << "\n";
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
            throw SyntaxError{concat("unknown input line: ", *line)};
        }
    }

    return pda;
}

} // namespace fla::pda
