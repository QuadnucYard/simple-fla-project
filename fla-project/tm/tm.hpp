#pragma once

#include "../utils/expected.hpp"
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_set>

namespace fla::tm {

using State = std::string;
using Symbol = char;
using SymbolVec = std::vector<Symbol>;

enum class Move {
    Left,
    Right,
    Hold,
};

inline std::ostream& operator<<(std::ostream& os, Move move) {
    switch (move) {
    case Move::Left:  os << "L"; break;
    case Move::Right: os << "R"; break;
    case Move::Hold:  os << "H"; break;
    }
    return os;
}

struct Action {
    Symbol new_symbol;
    Move move;
};

struct Transition {
    static constexpr Symbol WILDCARD_SYMBOL = '*';

    State old_state;
    State new_state;
    SymbolVec old_symbols;
    SymbolVec new_symbols;
    std::vector<Move> moves;

    bool matches(const State& state, const SymbolVec& peek_symbols) const {
        if (old_state != state) {
            return false;
        }
        for (size_t i = 0; i < old_symbols.size(); i++) {
            if (old_symbols[i] != WILDCARD_SYMBOL && old_symbols[i] != peek_symbols[i]) {
                return false;
            }
        }
        return true;
    }
};

} // namespace fla::tm

namespace fla::tm {

struct Tm {

    std::unordered_set<State> states;
    std::unordered_set<Symbol> input_symbols;
    std::unordered_set<Symbol> tape_symbols;
    Symbol blank_symbol;
    State start_state;
    std::unordered_set<State> final_states;
    std::size_t tape_num{1};
    std::vector<Transition> transitions;

    bool is_final(State state) const { return final_states.find(state) != final_states.end(); }

    std::optional<Transition> transit(const State& old_state, const SymbolVec& peek_symbols) const {
        for (auto& tr : transitions) {
            if (tr.matches(old_state, peek_symbols)) {
                return tr;
            }
        }
        return std::nullopt;
    }

    expected<bool, std::size_t> validate(std::string_view input) const;
};

std::ostream& operator<<(std::ostream& os, const Tm& tm);

} // namespace fla::tm
