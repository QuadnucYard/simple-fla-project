#pragma once

#include "../syntax/scanner.hpp"
#include "tm.hpp"

#include <cctype>

namespace fla::tm {

class Parser {
  public:
    Parser(std::string_view input) : scanner{input} {}

    Tm parse();

  private:
    Scanner scanner;
};

} // namespace fla::tm
