#include "pda.hpp"
#include <algorithm>
#include <unordered_set>

namespace fla::pda {

bool Pda::validate_self() const { return true; }

bool Pda::validate(std::string_view input) const {
    return std::all_of(input.begin(), input.end(),
                       [this](auto c) { return input_symbols.find(c) != input_symbols.end(); });
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::unordered_set<T>& set) {
    os << "{";
    auto it = set.begin();
    if (it != set.end()) {
        os << *it++;
    }
    while (it != set.end()) {
        os << ", " << *it++;
    }
    os << "}";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Pda& pda) {
    os << "[Deterministic Pushdown Automata]\n";
    os << "States (Q):              " << pda.states << "\n";
    os << "Input symbols (Sigma):   " << pda.input_symbols << "\n";
    os << "Stack symbols (Gamma):   " << pda.stack_symbols << "\n";
    os << "Start state (q0):        " << pda.start_state << "\n";
    os << "Start stack symbol (Z0): " << pda.start_stack_symbol << "\n";
    os << "Final states (F):        " << pda.final_states << "\n";
    os << "Transitions:\n";
    for (auto&& [k, v] : pda.transitions) {
        os << "    delta(" << k.old_state << ", " << k.input << ", " << k.old_stack_top << ") = ("
           << v.new_state << ", " << v.push_symbols << ")\n";
    }
    return os;
}

} // namespace fla::pda
