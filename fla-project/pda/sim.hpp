#pragma once

#include "pda.hpp"

#include <ostream>
#include <string_view>

namespace fla::pda {

enum class SimulationError { IllegalInput };

class Simulator {
  public:
    Simulator(const Pda& pda, std::ostream& out, std::ostream& err, bool verbose = false)
        : pda{pda}, out{out}, err{err}, verbose{verbose} {}

    expected<bool, SimulationError> operator()(std::string_view input);

  private:
    const Pda& pda;
    std::ostream& out;
    std::ostream& err;
    bool verbose;
};

} // namespace fla::pda
