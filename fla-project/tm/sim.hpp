#pragma once

#include "tm.hpp"

#include <ostream>
#include <string_view>

namespace fla::tm {

enum class SimulationError { IllegalInput };

class Simulator {
  public:
    struct Config {
        bool verbose{};
        bool rich{};
    };

    Simulator(const Tm& tm, std::ostream& out, std::ostream& err, Config config)
        : tm{tm}, out{out}, err{err}, config{config} {}

    expected<std::string, SimulationError> operator()(std::string_view input);

  private:
    const Tm& tm;
    std::ostream& out;
    std::ostream& err;
    Config config;
};

} // namespace fla::tm
