#include "pda.hpp"
#include "../utils/display.hpp"
#include "../utils/str.hpp"
#include "utils/ordered.hpp"
#include <algorithm>

namespace fla::pda {

std::optional<Pda::TransitionMap::const_iterator>
Pda::transit(const State& old_state, Symbol input_symbol, Symbol old_stack_top) const {
    if (auto it = transitions.find({old_state, input_symbol, old_stack_top});
        it != transitions.end()) {
        return it;
    }
    if (auto it = transitions.find({old_state, NULL_SYMBOL, old_stack_top});
        it != transitions.end()) {
        return it;
    }
    return std::nullopt;
}

expected<bool, std::vector<std::string>> Pda::validate_self() const {
    std::vector<std::string> errors;
    if (!has_state(start_state)) {
        errors.push_back(concat("start state `", start_state, "` is not in the state set"));
    }
    if (final_states.empty()) {
        errors.push_back(concat("no final state is provided"));
    }
    for (auto&& final_state : final_states) {
        if (!has_state(final_state)) {
            errors.push_back(concat("final state `", final_state, "` is not in the state set"));
        }
    }
    if (!has_stack_symbol(start_symbol)) {
        errors.push_back(concat("start symbol `", symbol_display(start_symbol),
                                "` is not in the stack alphabet"));
    }
    for (auto&& [tk, tv] : transitions) {
        if (!has_state(tk.old_state)) {
            errors.push_back(
                concat("old state `", tk.old_state, "` in the transition is not in the state set"));
        }
        if (!has_input_symbol(tk.input)) {
            errors.push_back(concat("input symbol `", symbol_display(tk.input),
                                    "` in the transition is not in the input alphabet"));
        }
        if (tk.old_stack_top != NULL_SYMBOL && !has_stack_symbol(tk.old_stack_top)) {
            errors.push_back(concat("stack symbol `", tk.old_stack_top,
                                    "` in the transition is not in the stack alphabet"));
        }
        if (!has_state(tv.new_state)) {
            errors.push_back(
                concat("new state `", tv.new_state, "` in the transition is not in the state set"));
        }
        for (auto sym : tv.push_symbols) {
            if (!has_stack_symbol(sym)) {
                errors.push_back(concat("stack symbol `", symbol_display(sym),
                                        "` in the transition is not in the stack alphabet"));
            }
        }
    }
    if (errors.empty()) {
        return true;
    } else {
        std::sort(errors.begin(), errors.end());
        errors.erase(std::unique(errors.begin(), errors.end()), errors.end());
        return unexpected{errors};
    }
}

expected<bool, size_t> Pda::validate_input(std::string_view input) const {
    for (size_t i = 0; i < input.size(); i++) {
        auto c = input[i];
        if (input_symbols.find(c) == input_symbols.end()) {
            return unexpected{i};
        }
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const Pda& pda) {
    using fla::operator<<;

    os << "[Deterministic Pushdown Automata]\n";
    os << "States (Q):              " << pda.states << "\n";
    os << "Input symbols (Sigma):   " << pda.input_symbols << "\n";
    os << "Stack symbols (Gamma):   " << pda.stack_symbols << "\n";
    os << "Start state (q0):        " << pda.start_state << "\n";
    os << "Start stack symbol (Z0): " << pda.start_symbol << "\n";
    os << "Final states (F):        " << pda.final_states << "\n";
    os << "Transitions:\n";
    for (auto&& [k, v] : to_ordered(pda.transitions)) {
        os << "    delta(" << k.old_state << ", " << symbol_display(k.input) << ", "
           << symbol_display(k.old_stack_top) << ") = (" << v.new_state << ", " << v.push_symbols
           << ")\n";
    }
    return os;
}

} // namespace fla::pda
