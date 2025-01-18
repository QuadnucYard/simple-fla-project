#pragma once

#include "pda.hpp"

#include <ostream>
#include <string_view>

namespace fla::pda {

enum class SimulationError { IllegalInput };

class Simulator {
  public:
    struct Config {
        bool verbose{};
        bool rich{};
        unsigned limit{10000000};
    };

    Simulator(const Pda& pda, std::ostream& out, std::ostream& err, Config config)
        : pda{pda}, out{out}, err{err}, config{config} {}

    expected<bool, SimulationError> operator()(std::string_view input);

  private:
    const Pda& pda;
    std::ostream& out;
    std::ostream& err;
    Config config;
};

} // namespace fla::pda
