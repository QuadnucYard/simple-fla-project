#include "tm.hpp"
#include <unordered_set>

namespace fla::tm {

expected<bool, std::size_t> Tm::validate(std::string_view input) const {
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
