#pragma once
#include <tuple>
#include <iostream>

template <typename Head, typename... Tail>
std::tuple<Tail...> tuple_tail(const std::tuple<Head, Tail...>& t) {
    return std::apply([](auto head, auto... tail) {
        return std::make_tuple(tail...); }, t);
}

template <typename CharT, typename Traits, typename Head, typename ... Tail>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, std::tuple<Head, Tail...> const& t) {
    return os << std::get<0>(t) << "; " << tuple_tail(t);
}

template <typename CharT, typename Traits, typename Last>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, std::tuple<Last> const& t) {
    return os << std::get<0>(t);
}