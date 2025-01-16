#pragma once

#include <variant>

namespace fla {

template <class E>
class unexpected {
  public:
    template <class Err = E>
    explicit unexpected(Err err) : error_{std::forward<Err>(err)} {}

    const E& error() const { return error_; }

  private:
    E error_;
};

template <class E>
unexpected(E err) -> unexpected<E>;

template <class T, class E>
class expected {
  public:
    expected(T value) : value_{value} {}
    expected(unexpected<E> err) : value_{err} {}

    bool has_value() const { return value_.index() == 0; }

    bool is_error() const { return value_.index() == 1; }

    const T& value() const { return std::get<0>(value_); }

    const E& error() const { return std::get<1>(value_).error(); };

    operator bool() const { return has_value(); }

    const T& operator*() const { return value(); }

  private:
    std::variant<T, unexpected<E>> value_;
};

} // namespace fla
