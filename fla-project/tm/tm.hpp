#pragma once

#include "../utils/expected.hpp"
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace fla::tm {

using State = std::string;
using Symbol = char;
using SymbolVec = std::vector<Symbol>;

enum class Move {
    Left,
    Right,
    Hold,
};

std::ostream& operator<<(std::ostream& os, Move move);

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

    bool matches(const State& state, const SymbolVec& peek_symbols) const;
};

} // namespace fla::tm

namespace fla::tm {

struct Tm {
    using StateSet = std::unordered_set<State>;
    using SymbolSet = std::unordered_set<Symbol>;
    using TransitionList = std::vector<Transition>;
    using TransitionMap = std::unordered_map<State, TransitionList>;

    StateSet states{};
    SymbolSet input_symbols{};
    SymbolSet tape_symbols{};
    Symbol blank_symbol{};
    State start_state{};
    StateSet final_states{};
    std::size_t tape_num{1};
    TransitionMap transitions{};

    bool has_state(const State& state) const { return states.find(state) != states.end(); }

    bool has_input_symbol(Symbol symbol) const {
        return symbol == Transition::WILDCARD_SYMBOL ||
               input_symbols.find(symbol) != input_symbols.end();
    }

    bool has_tape_symbol(Symbol symbol) const {
        return symbol == Transition::WILDCARD_SYMBOL ||
               tape_symbols.find(symbol) != tape_symbols.end();
    }

    bool is_final(State state) const { return final_states.find(state) != final_states.end(); }

    std::optional<TransitionList::const_iterator> transit(const State& old_state,
                                                          const SymbolVec& peek_symbols) const;

    // Validate whether this PDA is self-consistent.
    expected<bool, std::vector<std::string>> validate_self() const;

    // Validate whether input symbols are all in the alphabet.
    expected<bool, std::size_t> validate_input(std::string_view input) const;
};

std::ostream& operator<<(std::ostream& os, const Tm& tm);

} // namespace fla::tm
