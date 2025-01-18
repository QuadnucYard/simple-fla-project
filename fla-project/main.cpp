#include "args.hpp"
#include "pda/parser.hpp"
#include "pda/pda.hpp"
#include "pda/sim.hpp"
#include "syntax/err.hpp"
#include "tm/parser.hpp"
#include "tm/sim.hpp"
#include "utils/file.hpp"

#include <filesystem>
#include <iostream>
#include <string_view>

std::string get_stdin() {
    std::string buf;
    std::getline(std::cin, buf);
    return buf;
}

fla::pda::Pda parse_pda(std::string_view text, bool verbose) {
    try {
        fla::pda::Parser parser{text};
        return parser.parse();
    } catch (const fla::SyntaxError& err) {
        if (verbose) {
            std::cerr << "\x1b[1;31m"
                         "error:"
                         "\x1b[0m"
                      << " " << err.what() << "\n";
        } else {
            std::cerr << "syntax error\n";
        }
        std::exit(1);
    }
}

void run_pda(const fla::Cli& cli) {
    auto text = read_text(cli.path);
    auto pda = parse_pda(text, cli.verbose);
    if (cli.debug) {
        std::cout << pda << "\n";
    }
    if (auto res = pda.validate_self(); !res) {
        if (cli.verbose) {
            std::cerr << "\x1b[1;31m"
                         "error:"
                         "\x1b[0m"
                         " the PDA is erroneous\n";
            for (auto err : res.error()) {
                std::cerr << "error: " << err << "\n";
            }
        } else {
            std::cerr << "syntax error\n";
        }
        std::exit(1);
    }
    if (cli.check) {
        std::cout << "The PDA looks good\n";
        return;
    }
    fla::pda::Simulator sim{
        pda, std::cout, std::cerr,
        fla::pda::Simulator::Config{.verbose = cli.verbose, .rich = cli.rich, .limit = cli.limit}};
    auto res = sim(cli.input ? *cli.input : get_stdin());
    if (!res) {
        std::exit(1);
    }
    std::cout << std::boolalpha << *res << "\n";
}

fla::tm::Tm parse_tm(std::string_view text, bool verbose) {
    try {
        fla::tm::Parser parser{text};
        return parser.parse();
    } catch (const fla::SyntaxError& err) {
        if (verbose) {
            std::cerr << "\x1b[1;31m"
                         "error:"
                         "\x1b[0m"
                      << " " << err.what() << "\n";
        } else {
            std::cerr << "syntax error\n";
        }
        std::exit(1);
    }
}

void run_tm(const fla::Cli& cli) {
    auto text = read_text(cli.path);
    auto tm = parse_tm(text, cli.verbose);
    if (cli.debug) {
        std::cout << tm << "\n";
    }
    if (auto res = tm.validate_self(); !res) {
        if (cli.verbose) {
            std::cerr << "\x1b[1;31m"
                         "error:"
                         "\x1b[0m"
                         " the TM is erroneous\n";
            for (auto err : res.error()) {
                std::cerr << "error: " << err << "\n";
            }
        } else {
            std::cerr << "syntax error\n";
        }
        std::exit(1);
    }
    if (cli.check) {
        std::cout << "The TM looks good\n";
        return;
    }
    fla::tm::Simulator sim{
        tm, std::cout, std::cerr,
        fla::tm::Simulator::Config{.verbose = cli.verbose, .rich = cli.rich, .limit = cli.limit}};
    auto res = sim(cli.input ? *cli.input : get_stdin());
    if (!res) {
        std::exit(1);
    }
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
        run_pda(cli);
    } else if (ext == ".tm") {
        run_tm(cli);
    } else {
        std::cerr << "error: unknown file format: " << ext << "\n";
        std::exit(1);
    }
}
