#pragma once

#include "pda.hpp"

#include <ostream>
#include <string_view>

namespace fla::pda {

class Simulator {
  public:
    Simulator(const Pda& pda, std::ostream& out, std::ostream& err, bool verbose = false)
        : pda{pda}, out{out}, err{err}, verbose{verbose} {}

    bool operator()(std::string_view input);

  private:
    const Pda& pda;
    std::ostream& out;
    std::ostream& err;
    bool verbose;
};

} // namespace fla::pda
