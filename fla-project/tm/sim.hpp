#pragma once

#include "tm.hpp"

#include <ostream>
#include <string_view>

namespace fla::tm {

class Simulator {
  public:
    Simulator(const Tm& tm, std::ostream& out) : tm{tm}, out{out} {}

    void operator()(std::string_view input);

  private:
    const Tm& tm;
    std::ostream& out;
};

} // namespace fla::tm
