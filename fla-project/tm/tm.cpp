#include "tm.hpp"
#include "../utils/str.hpp"
#include <unordered_set>

namespace fla::tm {

std::ostream& operator<<(std::ostream& os, Move move) {
    switch (move) {
    case Move::Left:  os << "L"; break;
    case Move::Right: os << "R"; break;
    case Move::Hold:  os << "H"; break;
    }
    return os;
}

bool Transition::matches(const State& state, const SymbolVec& peek_symbols) const {
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

std::optional<Transition> Tm::transit(const State& old_state, const SymbolVec& peek_symbols) const {
    for (auto& tr : transitions) {
        if (tr.matches(old_state, peek_symbols)) {
            return tr;
        }
    }
    return std::nullopt;
}

expected<bool, std::vector<std::string>> Tm::validate_self() const {
    std::vector<std::string> errors;
    if (!has_state(start_state)) {
        errors.push_back(concat("start state `", start_state, "` is not in the state set"));
    }
    for (auto&& final_state : final_states) {
        if (!has_state(final_state)) {
            errors.push_back(concat("final state `", final_state, "` is not in the state set"));
        }
    }
    if (!has_tape_symbol(blank_symbol)) {
        errors.push_back(concat("blank symbol `", blank_symbol, "` is not in the tape alphabet"));
    }
    for (auto&& t : transitions) {
        if (!has_state(t.old_state)) {
            errors.push_back(
                concat("old state `", t.old_state, "` in the transition is not in the state set"));
        }
        if (!has_state(t.new_state)) {
            errors.push_back(
                concat("new state `", t.new_state, "` in the transition is not in the state set"));
        }
        for (auto sym : t.old_symbols) {
            if (!has_tape_symbol(sym)) {
                errors.push_back(concat("tape symbol `", sym,
                                        "` in the transition is not in the tape alphabet"));
            }
        }
        for (auto sym : t.new_symbols) {
            if (!has_tape_symbol(sym)) {
                errors.push_back(concat("tape symbol `", sym,
                                        "` in the transition is not in the tape alphabet"));
            }
        }
    }
    if (errors.empty()) {
        return true;
    } else {
        return unexpected{errors};
    }
}

expected<bool, std::size_t> Tm::validate_input(std::string_view input) const {
    for (size_t i = 0; i < input.size(); i++) {
        auto c = input[i];
        if (input_symbols.find(c) == input_symbols.end()) {
            return unexpected{i};
        }
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const SymbolVec& symbols) {
    for (auto c : symbols) {
        os << c;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Move>& moves) {
    for (auto mv : moves) {
        os << mv;
    }
    return os;
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

std::ostream& operator<<(std::ostream& os, const Tm& tm) {
    os << "[Turing Machine]\n";
    os << "States (Q):            " << tm.states << "\n";
    os << "Input symbols (Sigma): " << tm.input_symbols << "\n";
    os << "Tape symbols (Gamma):  " << tm.tape_symbols << "\n";
    os << "Start state (q0):      " << tm.start_state << "\n";
    os << "Blank symbol (B):      " << tm.blank_symbol << "\n";
    os << "Final states (F):      " << tm.final_states << "\n";
    os << "Tape number (N):       " << tm.tape_num << "\n";
    os << "Transitions:\n";
    for (auto&& tr : tm.transitions) {
        os << "    " << tr.old_state << " -> " << tr.new_state << ", " << tr.old_symbols << " / "
           << tr.new_symbols << ", " << tr.moves << "\n";
    }
    return os;
}

} // namespace fla::tm
