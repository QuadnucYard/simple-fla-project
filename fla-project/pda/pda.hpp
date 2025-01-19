#pragma once

#include "../utils/expected.hpp"
#include "../utils/hash.hpp"

#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace fla::pda {

using State = std::string;
using Symbol = char;
using SymbolVec = std::string;

struct TransitionKey {
    State old_state;
    Symbol input;
    Symbol old_stack_top;

    bool operator==(const TransitionKey& rhs) const {
        return old_state == rhs.old_state && input == rhs.input &&
               old_stack_top == rhs.old_stack_top;
    }
};

struct TransitionValue {
    State new_state;
    SymbolVec push_symbols;
};

} // namespace fla::pda

template <>
struct std::hash<fla::pda::TransitionKey> {
    std::size_t operator()(const fla::pda::TransitionKey& k) const noexcept {
        std::size_t seed{};
        fla::hash_combine(seed, k.old_state);
        fla::hash_combine(seed, k.input);
        fla::hash_combine(seed, k.old_stack_top);
        return seed;
    }
};

namespace fla::pda {

struct Pda {
    using StateSet = std::unordered_set<State>;
    using SymbolSet = std::unordered_set<Symbol>;
    using TransitionMap = std::unordered_map<TransitionKey, TransitionValue>;

    static constexpr Symbol NULL_SYMBOL = {};

    StateSet states{};
    SymbolSet input_symbols{};
    SymbolSet stack_symbols{};
    State start_state{};
    Symbol start_symbol{};
    StateSet final_states{};
    TransitionMap transitions{};

    bool has_state(const State& state) const { return states.find(state) != states.end(); }

    bool has_input_symbol(Symbol symbol) const {
        return symbol == NULL_SYMBOL || input_symbols.find(symbol) != input_symbols.end();
    }

    bool has_stack_symbol(Symbol symbol) const {
        return stack_symbols.find(symbol) != stack_symbols.end();
    }

    bool is_final(const State& state) const {
        return final_states.find(state) != final_states.end();
    }

    std::optional<TransitionMap::const_iterator>
    transit(const State& old_state, Symbol input_symbol, Symbol old_stack_top) const;

    // Validate whether this PDA is self-consistent.
    expected<bool, std::vector<std::string>> validate_self() const;

    // Validate whether input symbols are all in the alphabet.
    expected<bool, size_t> validate_input(std::string_view input) const;
};

std::ostream& operator<<(std::ostream& os, const Pda& pda);

inline char symbol_display(Symbol symbol) { return symbol == Pda::NULL_SYMBOL ? '_' : symbol; }

} // namespace fla::pda
