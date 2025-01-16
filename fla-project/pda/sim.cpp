#include "sim.hpp"
#include "pda.hpp"
#include "utils/expected.hpp"
#include <deque>
#include <iomanip>

namespace fla::pda {

expected<bool, SimulationError> Simulator::operator()(std::string_view input) {
    if (auto res = pda.validate_input(input); !res) {
        if (!verbose) {
            err << "illegal input\n";
            return unexpected{SimulationError::IllegalInput};
        }

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

    State state{pda.start_state};
    std::deque<Symbol> stack;
    stack.push_back(pda.start_symbol);

    std::size_t cursor{};

    auto print_state = [&]() {
        out << "|- " << state << ": ";
        for (auto c : stack) {
            out << c;
        }
        out << " - ";
        for (size_t i = cursor; i < input.length(); i++) {
            out << input[i];
        }
        out << "\n";
    };

    if (verbose) {
        print_state();
    }

    while (!pda.is_final(state)) {
        auto c = cursor < input.size() ? input[cursor] : Pda::NULL_SYMBOL;
        auto top = stack.empty() ? Pda::NULL_SYMBOL : stack.back();
        if (auto tr = pda.transit(state, c, top)) {
            state = (*tr)->second.new_state;
            if ((*tr)->first.input != Pda::NULL_SYMBOL) {
                cursor++;
                if (!stack.empty()) {
                    stack.pop_back();
                }
            }
            for (auto z : (*tr)->second.push_symbols) {
                stack.push_back(z);
            }
            if (verbose) {
                print_state();
            }
        } else {
            if (verbose) {
                out << "no trans for " << state << ", " << c << ", " << top << "\n";
            }
            return false;
        }
    }

    return cursor == input.length();
}

} // namespace fla::pda
