#include "sim.hpp"
#include "tm.hpp"
#include <cmath>
#include <cstdint>
#include <deque>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <vector>

namespace fla::tm {

struct Tape {
    std::deque<Symbol> tape;
    Symbol blank;
    std::size_t cursor{};
    std::ptrdiff_t index{};

    Symbol peek() const { return tape[cursor]; }

    std::ptrdiff_t pos(size_t i) const {
        return index + (std::ptrdiff_t)i - (std::ptrdiff_t)cursor;
    }

    void write(Symbol new_symbol, Move move) {
        if (new_symbol != Transition::WILDCARD_SYMBOL) {
            tape[cursor] = new_symbol;
        }
        switch (move) {
        case Move::Left:
            if (cursor == 0) {
                tape.push_front(blank);
            } else {
                cursor--;
            }
            if (cursor == tape.size() - 2 && tape[cursor + 1] == blank) {
                tape.pop_back();
            }
            index--;
            break;
        case Move::Right:
            if (cursor + 1 == tape.size()) {
                tape.push_back(blank);
            }
            if (cursor == 0 && tape[0] == blank) {
                tape.pop_front();
            } else {
                cursor++;
            }
            index++;
            break;
        case Move::Hold: break;
        }
    }

    auto begin() const {
        // strip leading blank symbol
        auto it = tape.begin();
        if (it != tape.end() && *it == blank) {
            ++it;
        }
        return it;
    }

    auto end() const {
        // strip trailing blank symbol
        auto it = tape.end();
        if (!(tape.size() == 1 && tape[0] == blank) && tape[tape.size() - 1] == blank) {
            --it;
        }
        return it;
    }
};

std::size_t print_width(long long x) {
    return (x == 0 ? 0 : static_cast<std::size_t>(std::log10(x))) + 1;
}

expected<std::string, SimulationError> Simulator::operator()(std::string_view input) {
    auto verbose = config.verbose;
    auto rich = config.rich;

    if (auto res = tm.validate_input(input); !res) {
        if (!verbose) {
            err << "illegal input\n";
            return unexpected{SimulationError::IllegalInput};
        }

        // error: 'A' was not declared in the set of input symbols
        // Input: 100A1A001
        //           ^
        auto err_index = res.error();
        err << "Input: " << input << "\n";
        err << "==================== ERR ====================\n";
        err << "error: '" << input[err_index] << "' was not declared in the set of input symbols\n";
        err << "Input: " << input << "\n";
        err << "       " << std::setw(err_index + 1) << '^' << "\n";
        err << "==================== END ====================\n";
        return unexpected{SimulationError::IllegalInput};
    }
    if (verbose) {
        out << "Input: " << input << "\n";
        out << "==================== RUN ====================\n";
    }

    // setup tapes
    State state{tm.start_state};
    std::vector<Tape> tapes(tm.tape_num);
    for (size_t i = 0; i < tm.tape_num; i++) {
        tapes[i].blank = tm.blank_symbol;
    }
    for (auto c : input) {
        tapes[0].tape.push_back(c);
    }
    if (tapes[0].tape.empty()) {
        tapes[0].tape.push_back(tm.blank_symbol);
    }
    for (size_t i = 1; i < tm.tape_num; i++) {
        tapes[i].tape.push_back(tm.blank_symbol);
    }
    SymbolVec peek(tapes.size());
    for (size_t i = 0; i < tm.tape_num; i++) {
        tapes[i].blank = tm.blank_symbol;
    }
    std::uint32_t step{};

    auto print_state = [&]() {
        if (rich) out << "\x1b[38;5;45m";
        out << "Step   : ";
        out << step;
        if (rich) out << "\x1b[0m";
        out << "\n";

        if (rich) out << "\x1b[38;5;219m";
        out << "State  : ";
        out << state;
        if (rich) out << "\x1b[0m";
        out << "\n";

        for (size_t i = 0; i < tm.tape_num; i++) {
            auto& tape = tapes[i];

            if (rich) out << "\x1b[38;5;28m";
            out << "Index" << i << " : ";
            if (rich) out << "\x1b[0m";
            for (size_t i = 0; i < tape.tape.size(); i++) {
                auto pos = std::abs(tape.pos(i));
                if (rich) {
                    if (rich && i == tape.cursor) out << "\x1b[1;4;38;5;34m";
                    else out << "\x1b[38;5;36m";
                }
                out << pos;
                if (rich) out << "\x1b[0m";
                out << " ";
            }
            out << "\n";

            if (rich) out << "\x1b[38;5;178m";
            out << "Tape" << i << "  : ";
            if (rich) out << "\x1b[0m";
            for (size_t i = 0; i < tape.tape.size(); i++) {
                auto pos = std::abs(tape.pos(i));
                if (rich) {
                    if (rich && i == tape.cursor) out << "\x1b[38;5;208m";
                    else out << "\x1b[38;5;220m";
                }
                out << std::left << std::setw(print_width(pos)) << tape.tape[i];
                if (rich) out << "\x1b[0m";
                out << ' ';
            }
            out << "\n";

            if (!rich) {
                out << "Head" << i << "  : ";
                for (size_t i = 0; i < tape.cursor; i++) {
                    auto pos = std::abs(tape.pos(i));
                    out << std::setw(print_width(pos)) << " ";
                }
                out << "^";
                out << '\n';
            }
        }

        if (rich) out << "\x1b[38;5;105m";
        out << "---------------------------------------------\n";
        if (rich) out << "\x1b[0m";
    };

    if (verbose) {
        print_state();
    }

    while (!tm.is_final(state)) {
        for (size_t i = 0; i < tm.tape_num; i++) {
            peek[i] = tapes[i].peek();
        }
        if (auto tr = tm.transit(state, peek)) {
            step++;
            state = (*tr)->new_state;
            for (size_t i = 0; i < tm.tape_num; i++) {
                tapes[i].write((*tr)->new_symbols[i], (*tr)->moves[i]);
            }
            if (verbose) {
                print_state();
            }
        } else {
            break;
        }
    }

    if (verbose) {
        out << "Result: ";
    }

    // print final tape result
    std::string result;
    for (auto c : tapes[0]) {
        result += c;
        out << c;
    }
    out << "\n";

    if (verbose) {
        out << "==================== END ====================\n";
    }
    return result;
}

} // namespace fla::tm
