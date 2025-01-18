#include "cli.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace cli {

namespace impl {

template <> auto parse<int>(const std::string& s) -> int { return std::stoi(s); }
template <> auto parse<unsigned int>(const std::string& s) -> unsigned int { return std::stoul(s); }
template <> auto parse<long>(const std::string& s) -> long { return std::stol(s); }
template <> auto parse<unsigned long>(const std::string& s) -> unsigned long {
    return std::stoul(s);
}
template <> auto parse<long long>(const std::string& s) -> long long { return std::stoll(s); }
template <> auto parse<unsigned long long>(const std::string& s) -> unsigned long long {
    return std::stoull(s);
}
template <> auto parse<float>(const std::string& s) -> float { return std::stof(s); }
template <> auto parse<double>(const std::string& s) -> double { return std::stod(s); }
template <> auto parse<bool>(const std::string& s) -> bool {
    return s == "y" || s == "yes" || s == "true";
}
template <> auto parse<std::string>(const std::string& s) -> std::string { return s; }

} // namespace impl

ArgMatches Command::get_matches(std::size_t argc, char* const* argv) const {
    ArgMatches matches;

    // set default values
    for (auto& arg : args_) {
        if (arg.default_value_) {
            matches.args_[arg.id_].set(arg.default_value_.value());
        }
    }

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
                auto& matched_arg = matches.args_[it->id_];
                // perform arg action
                switch (it->action_) {
                case ArgAction::Set:      matched_arg.set(argv[++i]); break;
                case ArgAction::SetTrue:  matched_arg.set("true"); break;
                case ArgAction::SetFalse: matched_arg.set("false"); break;
                case ArgAction::Append:   matched_arg.append(argv[i]); break;
                case ArgAction::Help:     print_help(); std::exit(0);
                }
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
                matches.args_[arg.id_].append(std::move(positionals[i]));
                i++;
            }
        }
    }
    // check requirements
    for (auto& arg : args_) {
        if (arg.is_required() && !matches.contains(arg.id_)) {
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

    struct Entry {
        std::string name;
        std::string help;
        Arg::DisplayOrder order;
    };
    struct Group {
        using Order = int;
        std::string name;
        Order order{};
        std::vector<Entry> entries{};
    };

    std::vector<Group> groups;
    groups.push_back(Group{.name = "Arguments"});
    groups.push_back(Group{.name = "Options"});
    auto& argument_usages = groups[0];
    auto& option_usages = groups[1];

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
            argument_usages.entries.push_back(
                Entry{std::move(value_name), arg.get_help(), arg.display_order_});
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
            option_usages.entries.push_back(
                Entry{std::move(info), arg.get_help(), arg.display_order_});
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

    for (auto& group : groups) {
        if (group.entries.empty()) {
            continue;
        }
        auto& entries = group.entries;
        std::stable_sort(entries.begin(), entries.end(),
                         [](auto&& a, auto&& b) { return a.order < b.order; });
        out << "\x1b[0;1m" << group.name
            << ":"
               "\x1b[0m"
               "\n";
        auto w = std::reduce(entries.begin(), entries.end(), (std::size_t)0,
                             [](auto x, Entry& e) { return std::max(x, e.name.length()); });
        for (auto&& entry : entries) {
            out << "  " << std::left << std::setw(w + 2) << entry.name << entry.help << "\n";
        }
        out << "\n";
    }

    std::exit(0);
}

} // namespace cli
