#pragma once

#include "cli/cli.hpp"
#include <filesystem>
#include <string>

namespace fla {

struct Cli {
    std::filesystem::path path;
    std::string input;
    bool verbose;

    static Cli from_matches(const cli::ArgMatches& matches) {
        return Cli{
            .path = matches.get_one("path").value(),
            .input = matches.get_one("input").value_or(""),
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
        .arg(Arg("input").help("The input to the PDA or TM").required(false))
        .arg(Arg("verbose").short_name('v').long_name("verbose").help(
            "Whether to display the whole simulation process."));
}

} // namespace fla
