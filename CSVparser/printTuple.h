#pragma once
#include <tuple>
#include <iostream>

//template <typename Tuple, size_t N>
//struct printTuple {
//    static std::ostream& print(std::ostream& out, const Tuple& tuple) {
//        TuplePrinter<Tuple, N - 1>::print(out, tuple);
//        return out << ", " << std::get<N - 1>(tuple);
//    }
//};
//
//template <typename Tuple>
//struct printTuple<Tuple, 1> {
//    static std::ostream& print(std::ostream& out, const Tuple& tuple) {
//        return out << std::get<0>(tuple);
//    }
//};
//
//template <typename ... TList>
//std::ostream& tuple_printer(
//    std::ostream& out,
//    const std::tuple<TList...>& tuple) {
//
//    return TuplePrinter<
//        decltype(tuple),
//        sizeof...(TList)
//    >::print(out << "(", tuple) << ")";
//}

namespace Tuple {

	template <typename Head, typename... Tail>
	std::tuple<Tail...> tuple_tail(const std::tuple<Head, Tail...>& t) {
		return std::apply([](auto head, auto... tail) {
			return std::make_tuple(tail...); }, t);
	}

	template <typename CharT, typename Traits, typename Head, typename ... Tail>
	auto operator<<(std::basic_ostream<CharT, Traits>& os, std::tuple<Head, Tail...> const& t) {
		os << std::get<0>(t) << " ";
		return os << tuple_tail(t);
	}

	template<typename CharT, typename Traits, typename Last>
	auto operator<<(std::basic_ostream<CharT, Traits>& os, Last const& last) {
		return os << last << " ";
	}
}