#include "sim.hpp"
#include "tm.hpp"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <deque>
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
            if (cursor == 1 && tape[0] == blank) {
                tape.pop_front();
            } else {
                cursor++;
            }
            index++;
            break;
        case Move::Hold: break;
        }
    }
};

std::size_t print_width(long long x) {
    return x == 0 ? 0 : static_cast<std::size_t>(std::log10(x));
}

std::string_view spaces(std::size_t width) {
    static const std::string_view SPACES{"            "};
    return SPACES.substr(0, width);
}

void Simulator::operator()(std::string_view input) {
    out << "Input: " << input << "\n";
    if (!tm.validate(input)) {
        out << "==================== ERR ====================\n";
        // error: 'A' was not declared in the set of input symbols
        // Input: 100A1A001
        //           ^
        out << "==================== END ====================\n";
        return;
    }
    out << "==================== RUN ====================\n";

    State state{tm.start_state};
    std::vector<Tape> tapes(tm.tape_num);
    for (size_t i = 0; i < tm.tape_num; i++) {
        tapes[i].blank = tm.blank_symbol;
    }
    for (auto c : input) {
        tapes[0].tape.push_back(c);
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
        out << "Step   : " << step << "\n";
        out << "State  : " << state << "\n";
        for (size_t i = 0; i < tm.tape_num; i++) {
            auto& tape = tapes[i];
            out << "Index" << i << " : ";
            for (size_t i = 0; i < tape.tape.size(); i++) {
                auto pos = std::abs(tape.pos(i));
                out << pos << " ";
            }
            out << "\n";
            out << "Tape" << i << "  : ";
            for (size_t i = 0; i < tape.tape.size(); i++) {
                auto pos = std::abs(tape.pos(i));
                out << tape.tape[i] << spaces(print_width(pos) + 1);
            }
            out << "\n";
            out << "Head" << i << "  : ";
            for (size_t i = 0; i < tape.cursor; i++) {
                auto pos = std::abs(tape.pos(i));
                out << spaces(print_width(pos) + 2);
            }
            out << "^\n";
        }
        out << "---------------------------------------------\n";
    };

    print_state();

    while (!tm.is_final(state)) {
        for (size_t i = 0; i < tm.tape_num; i++) {
            peek[i] = tapes[i].peek();
        }
        if (auto tr = tm.transit(state, peek)) {
            step++;
            state = tr->new_state;
            for (size_t i = 0; i < tm.tape_num; i++) {
                tapes[i].write(tr->new_symbols[i], tr->moves[i]);
            }
            print_state();
        } else {
            std::cout << "no trans\n";
            return;
        }
        if (step >= 100) {
            break;
        }
    }
}

} // namespace fla::tm
