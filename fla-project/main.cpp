#include "pda/sim.hpp"
#include "pda/syntax.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char* argv[]) {
    std::cout << "cwd:" << std::filesystem::current_path() << "\n";
    for (int i = 0; i < argc; i++) {
        printf("- %s\n", argv[i]);
    }

    std::ifstream fin{"pda/anbn.pda"};
    if (!fin) {
        std::cerr << "fail to open file\n";
        return 1;
    }
    std::stringstream stream;
    stream << fin.rdbuf();
    auto text = stream.str();
    fla::pda::PdaParser parser{text};
    auto pda = parser.parse();
    std::cout << pda;
    fla::pda::Simulator sim{pda};
    std::cout << sim("aaabbb") << "\n";
    std::cout << sim("aaabb") << "\n";
    std::cout << sim("aabbb") << "\n";
    std::cout << sim("") << "\n";
}
