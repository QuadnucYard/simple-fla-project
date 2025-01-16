#pragma once

#include "../syntax/scanner.hpp"
#include "pda.hpp"

#include <cctype>

namespace fla::pda {

class Parser {
  public:
    Parser(std::string_view input) : scanner{input} {}

    Pda parse();

  private:
    Scanner scanner;
};

} // namespace fla::pda
