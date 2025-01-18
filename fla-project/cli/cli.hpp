#pragma once

#include <algorithm>
#include <cassert>
#include <cctype>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace cli {

namespace impl {

template <class T>
auto parse(const std::string& s) -> T;

}

enum class ArgAction { Set, SetTrue, SetFalse, Append, Help };

class MatchedArg {
  public:
    void set(std::string value) {
        values_.clear();
        values_.push_back(std::move(value));
    }

    void append(std::string value) { values_.push_back(std::move(value)); }

  private:
    std::vector<std::string> values_;

    friend class ArgMatches;
};

class ArgMatches {
  public:
    template <class T>
    std::optional<T> get_one(const std::string& id) const {
        if (auto it = args_.find(id); it != args_.end()) {
            if (it->second.values_.size() > 0) {
                return impl::parse<T>(it->second.values_[0]);
            }
        }
        return std::nullopt;
    }

    bool get_flag(const std::string& id) const { return get_one<bool>(id).value(); }

    bool contains(const std::string& id) const {
        if (auto it = args_.find(id); it != args_.end()) {
            if (it->second.values_.size() > 0) {
                return true;
            }
        }
        return false;
    }

  private:
    std::unordered_map<std::string, MatchedArg> args_;

    friend class Command;
};

class Arg {
  public:
    using DisplayOrder = int;

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

    Arg& action(ArgAction action) {
        action_ = action;
        if (action == ArgAction::SetTrue) {
            default_value_ = "false";
        } else if (action == ArgAction::SetFalse) {
            default_value_ = "true";
        }
        return *this;
    }

    Arg& display_order(DisplayOrder order) {
        display_order_ = order;
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
    ArgAction action_{ArgAction::Set};
    DisplayOrder display_order_{0};

    friend class Command;
};

class Command {
  public:
    Command(std::string name) : name_{std::move(name)} {}

    Command& help(std::string s) {
        help_ = std::move(s);

        args_.push_back(Arg("help")
                            .short_name('h')
                            .long_name("help")
                            .help("Print help")
                            .action(ArgAction::Help)
                            .display_order(0x7fffffff));

        return *this;
    }

    Command& arg(Arg arg) {
        args_.push_back(std::move(arg));
        return *this;
    }

    ArgMatches get_matches(std::size_t argc, char* const* argv) const;

  private:
    void print_help() const;

    void print_usage(bool err) const;

  private:
    std::string name_;
    std::optional<std::string> help_;
    std::vector<Arg> args_;
};

} // namespace cli
