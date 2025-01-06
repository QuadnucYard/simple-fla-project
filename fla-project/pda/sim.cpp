#include "sim.hpp"
#include "pda.hpp"
#include <iostream>
#include <stack>

namespace fla::pda {

bool Simulator::operator()(std::string_view input) {
    State state{pda.start_state};
    std::stack<Symbol> stack;
    stack.push(pda.start_stack_symbol);

    std::size_t cursor{};

    while (!pda.is_final(state)) {
        auto c = cursor < input.size() ? input[cursor++] : Pda::NULL_SYMBOL;
        auto top = stack.empty() ? Pda::NULL_SYMBOL : stack.top();
        if (auto tr = pda.transit(state, c, top)) {
            state = tr->new_state;
            if (!stack.empty()) {
                stack.pop();
            }
            for (auto z : tr->push_symbols) {
                stack.push(z);
            }
            std::cout << "|- " << state << "\n";
        } else {
            std::cout << "no trans for " << state << ", " << c << ", " << top << "\n";
            return false;
        }
    }

    return true;
}

} // namespace fla::pda
