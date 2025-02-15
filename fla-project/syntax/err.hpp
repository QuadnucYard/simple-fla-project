#include <stdexcept>
#include <string>

namespace fla {

struct SyntaxError : std::runtime_error {
    SyntaxError(std::string hint) : std::runtime_error{"failed to parse model: " + hint} {}
};

} // namespace fla
