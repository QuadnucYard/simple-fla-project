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

    const E& error() const { return std::get<1>(value_).error(); };

    operator bool() const { return value_.index() == 0; }

  private:
    std::variant<T, unexpected<E>> value_;
};

} // namespace fla
