#include "pda/sim.hpp"
#include "pda/syntax.hpp"
#include "tm/sim.hpp"
#include "tm/syntax.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string_view>

template <class P>
std::string read_text(P path) {
    std::ifstream fin{path};
    if (!fin) {
        throw std::runtime_error{"fail to open file"};
    }
    std::stringstream stream;
    stream << fin.rdbuf();
    return stream.str();
}

int main(int argc, char* argv[]) {
    std::cout << "cwd:" << std::filesystem::current_path() << "\n";
    for (int i = 0; i < argc; i++) {
        printf("- %s\n", argv[i]);
    }

    {
        auto text = read_text("pda/anbn.pda");
        fla::pda::Parser parser{text};
        auto pda = parser.parse();
        std::cout << pda;
        fla::pda::Simulator sim{pda};
        std::cout << sim("aaabbb") << "\n";
        std::cout << sim("aaabb") << "\n";
        std::cout << sim("aabbb") << "\n";
        std::cout << sim("") << "\n";
    }

    {
        auto text = read_text("tm/palindrome_detector_2tapes.tm");
        fla::tm::Parser parser{text};
        auto tm = parser.parse();
        std::cout << tm;
        fla::tm::Simulator sim{tm, std::cout};
        sim("1001001");
        sim("100010001");
        // sim("10001000");
        // sim("100A1A001");
    }
}
