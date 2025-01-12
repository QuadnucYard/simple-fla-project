#include "cli.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace cli {

std::size_t get_first_width(const std::vector<std::pair<std::string, std::string>>& info) {
    std::size_t w{};
    for (auto& a : info) {
        w = std::max(w, a.first.length());
    }
    return w;
}

ArgMatches Command::get_matches(std::size_t argc, char* const* argv) {
    args_.push_back(Arg("help").short_name('h').long_name("help").help("Print help"));

    ArgMatches matches;
    std::vector<std::string> positionals;
    for (std::size_t i = 1; i < argc; i++) {
        std::string_view s{argv[i]};
        if (s.size() >= 2 && s[0] == '-') {
            decltype(args_)::const_iterator it;
            if (s[1] == '-') {
                // long
                auto name = s.substr(2);
                it = std::find_if(args_.begin(), args_.end(),
                                  [&](const Arg& arg) { return arg.long_ && *arg.long_ == name; });

            } else {
                // short
                auto name = s[1];
                it = std::find_if(args_.begin(), args_.end(), [&](const Arg& arg) {
                    return arg.short_ && *arg.short_ == name;
                });
            }
            if (it != args_.end()) {
                // help
                if (it->id_ == "help") {
                    print_help();
                    std::exit(0);
                }
                // non-flag args not supported
                matches.args_[it->id_].values_.push_back(argv[i]);
            } else {
                std::cerr << "\x1b[1;31m"
                             "error:"
                             "\x1b[0m"
                             " unexpected argument '"
                             "\x1b[33m"
                          << s
                          << "\x1b[0m"
                             "' found\n\n";
                print_usage(true);
                std::exit(1);
            }
        } else {
            positionals.push_back(std::string{s});
        }
    }
    // populate positional args
    size_t i{};
    for (auto& arg : args_) {
        if (arg.is_positional()) {
            if (i < positionals.size()) {
                matches.args_[arg.id_].values_.push_back(std::move(positionals[i]));
                i++;
            }
        }
    }
    // check requirements
    for (auto& arg : args_) {
        if (arg.is_required() && !matches.get_flag(arg.id_)) {
            std::cerr << "\x1b[1;31m"
                         "error:"
                         "\x1b[0m"
                         " ";
            if (arg.is_positional()) {
                std::cerr << "required argument `"
                             "\x1b[33m"
                          << arg.id_
                          << "\x1b[0m"
                             "` is missing";
            } else {
                std::cerr << "required option `"
                             "\x1b[33m"
                          << arg.id_
                          << "\x1b[0m"
                             "` is missing";
            }
            std::cerr << "\n\n";
            print_usage(true);
            std::exit(1);
        }
    }
    return matches;
}

void Command::print_help() const {
    auto& out = std::cout;
    out << name_ << "\n";
    if (help_) {
        out << *help_ << "\n";
    }
    out << "\n";
    print_usage(false);
}

void Command::print_usage(bool err) const {
    auto& out = err ? std::cerr : std::cout;

    std::vector<std::pair<std::string, std::string>> argument_usages;
    std::vector<std::pair<std::string, std::string>> option_usages;

    out << "\x1b[0;4;1m"
           "Usage:"
           "\x1b[0m"
           " "
        << name_ << "[EXE]";
    for (auto& arg : args_) {
        auto value_name = arg.get_value_name();
        if (arg.is_positional()) {
            if (arg.is_required()) {
                out << " <" << value_name << ">";
            } else {
                out << " [" << value_name << "]";
            }
            argument_usages.emplace_back(std::move(value_name), arg.get_help());
        } else {
            if (arg.is_required()) {
                if (arg.long_) {
                    out << " --" << *arg.long_ << " " << value_name;
                } else if (arg.short_) {
                    out << " -" << *arg.short_ << " " << value_name;
                }
            }
            std::string info;
            if (arg.short_) {
                info += '-';
                info += *arg.short_;
                if (arg.long_) {
                    info += ", ";
                }
            } else {
                info += "    ";
            }
            if (arg.long_) {
                info += "--";
                info += *arg.long_;
            }
            if (arg.is_required()) {
                info += value_name;
            }
            option_usages.emplace_back(std::move(info), arg.get_help());
        }
    }
    out << "\n\n";
    if (err) {
        out << "For more information, try '"
               "\x1b[0;1m"
               "--help"
               "\x1b[0m"
               "'.\n\n";
        return;
    }

    if (!argument_usages.empty()) {
        out << "\x1b[0;1m"
               "Arguments:"
               "\x1b[0m"
               "\n";
        auto w = get_first_width(argument_usages);
        for (auto& a : argument_usages) {
            out << "  " << std::left << std::setw(w + 2) << a.first << a.second << "\n";
        }
        out << "\n";
    }

    if (!option_usages.empty()) {
        out << "\x1b[0;1m"
               "Options:"
               "\x1b[0m"
               "\n";
        auto w = get_first_width(option_usages);
        for (auto& a : option_usages) {
            out << "  " << std::left << std::setw(w + 2) << a.first << a.second << "\n";
        }
        out << "\n";
    }

    std::exit(0);
}

} // namespace cli
