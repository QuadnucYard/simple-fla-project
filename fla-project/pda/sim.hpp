#pragma once

#include "pda.hpp"

#include <string_view>

namespace fla::pda {

class Simulator {
  public:
    Simulator(const Pda& pda) : pda{pda} {}

    bool operator()(std::string_view input);

  private:
    const Pda& pda;
};

} // namespace fla::pda
