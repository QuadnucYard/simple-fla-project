#pragma once

#include "tm.hpp"

#include <ostream>
#include <string_view>

namespace fla::tm {

class Simulator {
  public:
    Simulator(const Tm& tm, std::ostream& out, bool verbose = false)
        : tm{tm}, out{out}, verbose{verbose} {}

    void operator()(std::string_view input);

  private:
    const Tm& tm;
    std::ostream& out;
    bool verbose;
};

} // namespace fla::tm
