#pragma once

#include "cli/cli.hpp"
#include <cstdint>
#include <filesystem>
#include <string>

namespace fla {

struct Cli {
    std::filesystem::path path;
    std::optional<std::string> input;
    uint32_t limit;
    bool check;
    bool debug;
    bool verbose;
    bool rich;

    static Cli from_matches(const cli::ArgMatches& matches) {
        return Cli{
            .path = matches.get_one<std::string>("path").value(),
            .input = matches.get_one<std::string>("input"),
            .limit = matches.get_one<uint32_t>("limit").value_or(10000000),
            .check = matches.get_flag("check"),
            .debug = matches.get_flag("debug"),
            .verbose = matches.get_flag("verbose"),
            .rich = matches.get_flag("rich"),
        };
    }
};

inline cli::Command cmd() {
    using namespace cli;

    return Command("fla")
        .help("A CLI tool to parse and simulate PDA or TM")
        .arg(Arg("path")
                 .help("Path to the PDA/TM syntax file. Must end with `.pda` or `.tm`")
                 .required(true))
        .arg(Arg("input")
                 .help("Input for the PDA or TM. Reads from stdin if not provided")
                 .required(false))
        .arg(Arg("limit").short_name('l').long_name("limit").help("Limit of simulation steps"))
        .arg(Arg("check")
                 .short_name('c')
                 .long_name("check")
                 .action(ArgAction::SetTrue)
                 .help("Check the grammar of the syntax file"))
        .arg(Arg("debug")
                 .short_name('d')
                 .long_name("debug")
                 .action(ArgAction::SetTrue)
                 .help("Print the parameters of the PDA/TM"))
        .arg(Arg("verbose")
                 .short_name('v')
                 .long_name("verbose")
                 .action(ArgAction::SetTrue)
                 .help("Display the entire simulation process in details"))
        .arg(Arg("rich")
                 .short_name('r')
                 .long_name("rich")
                 .action(ArgAction::SetTrue)
                 .help("Enable rich style output"));
}

} // namespace fla
