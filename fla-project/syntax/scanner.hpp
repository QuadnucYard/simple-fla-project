#pragma once

#include <optional>
#include <string_view>

namespace fla {

class Scanner {
  public:
    Scanner(std::string_view input) : input{input} {}

    std::size_t line() const { return line_; }

    std::optional<std::string_view> next_line();

  private:
    std::string_view input;
    std::size_t cursor{0};
    std::size_t line_{0};
};

} // namespace fla
