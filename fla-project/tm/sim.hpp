#pragma once

#include "tm.hpp"

#include <ostream>
#include <string_view>

namespace fla::tm {

class Simulator {
  public:
    Simulator(const Tm& tm, std::ostream& out, std::ostream& err, bool verbose = false)
        : tm{tm}, out{out}, err{err}, verbose{verbose} {}

    bool operator()(std::string_view input);

  private:
    const Tm& tm;
    std::ostream& out;
    std::ostream& err;
    bool verbose;
};

} // namespace fla::tm
