#pragma once

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
    static constexpr Symbol NULL_SYMBOL = {};

    std::unordered_set<State> states;
    std::unordered_set<Symbol> input_symbols;
    std::unordered_set<Symbol> stack_symbols;
    State start_state;
    Symbol start_stack_symbol;
    std::unordered_set<State> final_states;
    std::unordered_map<TransitionKey, TransitionValue> transitions;

    bool is_final(State state) const { return final_states.find(state) != final_states.end(); }

    std::optional<TransitionValue> transit(const State& old_state, Symbol input_symbol,
                                           Symbol old_stack_top) const {
        TransitionKey key{old_state, input_symbol, old_stack_top};
        if (auto it = transitions.find(key); it != transitions.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    bool validate(std::string_view input) const {
        for (auto c : input) {
            if (input_symbols.find(c) == input_symbols.end()) {
                return false;
            }
        }
        return true;
    }
};

std::ostream& operator<<(std::ostream& os, const Pda& pda);

} // namespace fla::pda
