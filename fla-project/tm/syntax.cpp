#include "syntax.hpp"
#include "../syntax/err.hpp"
#include "../utils/str.hpp"
#include "tm.hpp"

#include <regex>
#include <string>
#include <vector>

using svmatch = std::match_results<std::string_view::const_iterator>;

namespace fla::tm {

Symbol as_symbol(std::string_view s) {
    if (s.size() != 1) {
        throw SyntaxError(concat("\"", s, "\" is not a valid symbol"));
    }
    return s[0];
}

Tm Parser::parse() {
    static const std::regex q_regex{R"(^#Q *= *\{(.+)\}$)"};
    static const std::regex s_regex{R"(^#S *= *\{(.+)\}$)"};
    static const std::regex g_regex{R"(^#G *= *\{(.+)\}$)"};
    static const std::regex q0_regex{R"(^#q0 *= *(\w+)$)"};
    static const std::regex b_regex{R"(^#B *= *(\w+)$)"};
    static const std::regex f_regex{R"(^#F *= *\{(.+)\}$)"};
    static const std::regex n_regex{R"(^#N *= *(\d+)$)"};
    static const std::regex delta_regex{R"(^(\w+) +(\S+) +(\S+) +(\S+) +(\w+)\s*$)"};

    Tm tm;
    while (auto line = scanner.next_line()) {
        if (svmatch match; std::regex_search(line->begin(), line->end(), match, q_regex)) {
            for (auto q : comma_separated(match[1])) {
                tm.states.insert(std::string{q});
            }
        } else if (svmatch match; std::regex_search(line->begin(), line->end(), match, s_regex)) {
            for (auto s : comma_separated(match[1])) {
                tm.input_symbols.insert(as_symbol(s));
            }
        } else if (svmatch match; std::regex_search(line->begin(), line->end(), match, g_regex)) {
            for (auto s : comma_separated(match[1])) {
                tm.tape_symbols.insert(as_symbol(s));
            }
        } else if (svmatch match; std::regex_search(line->begin(), line->end(), match, q0_regex)) {
            tm.start_state = match[1];
        } else if (svmatch match; std::regex_search(line->begin(), line->end(), match, b_regex)) {
            tm.blank_symbol = as_symbol(match[1].str());
        } else if (svmatch match; std::regex_search(line->begin(), line->end(), match, f_regex)) {
            for (auto q : comma_separated(match[1])) {
                tm.final_states.insert(std::string{q});
            }
        } else if (svmatch match; std::regex_search(line->begin(), line->end(), match, n_regex)) {
            tm.tape_num = std::stoi(match[1]);
        } else if (svmatch match;
                   std::regex_search(line->begin(), line->end(), match, delta_regex)) {
            Transition tr;
            tr.old_state = match[1];
            tr.new_state = match[5];
            for (auto c : match[2].str()) {
                tr.old_symbols.push_back(c);
            }
            for (auto c : match[3].str()) {
                tr.new_symbols.push_back(c);
            }
            for (auto c : match[4].str()) {
                Move move;
                switch (c) {
                case 'l': move = Move::Left; break;
                case 'r': move = Move::Right; break;
                case '*': move = Move::Hold; break;
                default:  throw SyntaxError{concat("unknown move: ", c)};
                }
                tr.moves.push_back(move);
            }
            tm.transitions.push_back(std::move(tr));
        } else {
            throw SyntaxError{concat("unknown input line: ", *line)};
        }
    }

    return tm;
}

} // namespace fla::tm
