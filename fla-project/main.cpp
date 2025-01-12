#include "args.hpp"
#include "pda/sim.hpp"
#include "pda/syntax.hpp"
#include "tm/sim.hpp"
#include "tm/syntax.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

template <class P>
std::string read_text(const P& path) {
    std::ifstream fin{path};
    if (!fin) {
        throw std::runtime_error{"fail to open file"};
    }
    std::stringstream stream;
    stream << fin.rdbuf();
    return stream.str();
}

int main(int argc, char* argv[]) {
    auto matches = fla::cmd().get_matches(argc, argv);
    auto cli = fla::Cli::from_matches(matches);

    if (!std::filesystem::exists(cli.path)) {
        std::cerr << "error: the path to syntax file does not exist\n";
        std::exit(1);
    }

    auto ext = cli.path.extension();
    if (ext == ".pda") {
        auto text = read_text(cli.path);
        fla::pda::Parser parser{text};
        auto pda = parser.parse();
        fla::pda::Simulator sim{pda, std::cout, cli.verbose};
        std::cout << std::boolalpha << sim(cli.input) << "\n";
    } else if (ext == ".tm") {
        auto text = read_text(cli.path);
        fla::tm::Parser parser{text};
        auto tm = parser.parse();
        fla::tm::Simulator sim{tm, std::cout, std::cerr, cli.verbose};
        auto res = sim(cli.input);
        if (!res) {
            std::exit(1);
        }
    } else {
        std::cerr << "error: unknown file format: " << ext << "\n";
        std::exit(1);
    }
}
