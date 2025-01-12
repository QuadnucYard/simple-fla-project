#pragma once

#include "cli/cli.hpp"
#include <filesystem>
#include <string>

namespace fla {

struct Cli {
    std::filesystem::path path;
    std::optional<std::string> input;
    bool check;
    bool debug;
    bool verbose;

    static Cli from_matches(const cli::ArgMatches& matches) {
        return Cli{
            .path = matches.get_one("path").value(),
            .input = matches.get_one("input"),
            .check = matches.get_flag("check"),
            .debug = matches.get_flag("debug"),
            .verbose = matches.get_flag("verbose"),
        };
    }
};

inline cli::Command cmd() {
    using namespace cli;

    return Command("fla")
        .help("A CLI tool to parse and simulate PDA or TM")
        .arg(Arg("path")
                 .help("The path of the PDA/TM syntax file. Must end with `.pda` or `.tm`")
                 .required(true))
        .arg(Arg("input")
                 .help("The input to the PDA or TM. Read from stdin if not provided")
                 .required(false))
        .arg(Arg("check").short_name('c').long_name("check").help(
            "Check the grammar of the syntax file"))
        .arg(Arg("debug").short_name('d').long_name("debug").help("Print the PDA/TM"))
        .arg(Arg("verbose").short_name('v').long_name("verbose").help(
            "Whether to display the whole simulation process"));
}

} // namespace fla
