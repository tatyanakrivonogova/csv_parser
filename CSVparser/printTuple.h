#pragma once
#include <tuple>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

template <typename Head, typename... Tail>
std::tuple<Tail...> tuple_tail(const std::tuple<Head, Tail...>& t) {
    return std::apply([](Head head, Tail... tail) {
        return std::make_tuple(tail...); }, t);
}

//template <typename Head, typename... Tail>
//std::tuple<Tail...> tuple_tail(const std::tuple<Head>& t) {
//    return std::make_tuple();
//}
//
//template <typename Head, typename... Tail>
//std::tuple<Tail...> tuple_tail(const std::tuple<>& t) {
//    return std::make_tuple();
//}

template <typename CharT, typename Traits, typename Head, typename ... Tail>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, std::tuple<Head, Tail...> const& t) {
    return os << std::get<0>(t) << "; " << tuple_tail(t);
}

template <typename CharT, typename Traits, typename Last>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, std::tuple<Last> const& t) {
    return os << std::get<0>(t);
}






template <typename T>
T type_cast(std::string& str) {
    std::stringstream ss;
    ss.str(str);

    T var;
    ss >> var;
    //std::string check_str = std::to_string(var);
    //if (str != check_str) {
    //    //throw std::invalid_argument("Bad type of field");
    //}
    return var;
}






template <typename newElement, typename ... TupleArgs>
std::tuple<TupleArgs..., newElement> tuple_append(const std::tuple<TupleArgs...>& tuple, const newElement& element) {
    return std::tuple_cat(tuple, std::make_tuple(element));
}


template <typename Head, typename ... Args>
void convertVectorToTuple(std::tuple<Head> typesTuple, std::tuple<Args...>& valuesTuple,
    std::vector<std::string, std::allocator<std::string>>& valuesVector, size_t index) {
    Head element = type_cast<Head>(valuesVector[index]);
    tuple_append(valuesTuple, element);
    //std::cout << "!!!";
}


template <typename Head, typename ... Tail, typename ... Args>
void convertVectorToTuple(std::tuple<Head, Tail...> typesTuple, std::tuple<Args...>& valuesTuple, std::vector<std::string, std::allocator<std::string>>& valuesVector, size_t index) {
    Head element = type_cast<Head>(valuesVector[index]);
    tuple_append(valuesTuple, element);
    convertVectorToTuple(tuple_tail(typesTuple), valuesTuple, valuesVector, index + 1);
    //std::cout << "***";
}

template <typename ... Args>
void convertVectorToTuple(std::tuple<> typesTuple, std::tuple<Args...>& valuesTuple,
    std::vector<std::string, std::allocator<std::string>>& valuesVector, size_t index) {
    std::cout << "!!!";
}

//template <typename ... Tail, typename ... Args>
//void convertVectorToTuple(std::tuple<Tail...> typesTuple, std::tuple<Args...>& valuesTuple, std::vector<std::string, std::allocator<std::string>>& valuesVector, size_t index) {
//    using newType = typename std::tuple_element<0, decltype(typesTuple)>::type;
//    newType element = type_cast<newType>(valuesVector[index]);
//    tuple_append(valuesTuple, element);
//    convertVectorToTuple(tuple_tail(typesTuple), valuesTuple, valuesVector, index + 1);
//    //std::cout << "***";
//}




