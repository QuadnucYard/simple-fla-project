#pragma once

#include <algorithm>
#include <cassert>
#include <cctype>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace cli {

class MatchedArg {
  private:
    std::vector<std::string> values_;

    friend class Command;
    friend class ArgMatches;
};

class ArgMatches {
  public:
    std::optional<std::string> get_one(const std::string& id) const {
        if (auto it = args_.find(id); it != args_.end()) {
            if (it->second.values_.size() > 0) {
                return it->second.values_[0];
            }
        }
        return std::nullopt;
    }

    bool get_flag(const std::string& id) const { return args_.find(id) != args_.end(); }

  private:
    std::unordered_map<std::string, MatchedArg> args_;

    friend class Command;
};

class Arg {
  public:
    Arg(std::string id) : id_{std::move(id)} {}

    Arg& short_name(char c) {
        short_ = c;
        return *this;
    }

    Arg& long_name(std::string s) {
        assert(s.size() > 1);
        long_ = std::move(s);
        return *this;
    }

    Arg& default_value(std::string s) {
        default_value_ = std::move(s);
        required_ = false;
        return *this;
    }

    Arg& value_name(std::string s) {
        value_name_ = std::move(s);
        return *this;
    }

    Arg& help(std::string s) {
        help_ = std::move(s);
        return *this;
    }

    Arg& required(bool yes) {
        required_ = yes;
        return *this;
    }

    std::string get_help() const {
        if (help_) {
            return *help_;
        }
        return "";
    }

    std::string get_value_name() const {
        if (value_name_) {
            return *value_name_;
        }
        auto s = id_;
        std::transform(s.begin(), s.end(), s.begin(), [](auto c) { return std::toupper(c); });
        return s;
    }

    bool is_positional() const { return !short_ && !long_; }

    bool is_required() const { return required_; }

  private:
    std::string id_;
    std::optional<char> short_{};
    std::optional<std::string> long_{};
    std::optional<std::string> default_value_{};
    std::optional<std::string> value_name_{};
    std::optional<std::string> help_{};
    bool required_{};

    friend class Command;
};

class Command {
  public:
    Command(std::string name) : name_{std::move(name)} {}

    Command& help(std::string s) {
        help_ = std::move(s);
        return *this;
    }

    Command& arg(Arg arg) {
        args_.push_back(std::move(arg));
        return *this;
    }

    ArgMatches get_matches(std::size_t argc, char* const* argv);

  private:
    void print_help() const;

    void print_usage(bool err) const;

  private:
    std::string name_;
    std::optional<std::string> help_;
    std::vector<Arg> args_;
};

} // namespace cli
