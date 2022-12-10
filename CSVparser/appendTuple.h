#pragma once
#include <tuple>
#include <vector>
#include <string>

//template <typename newElement, typename Tuple>
//std::tuple<Tuple, newElement> tuple_append(const Tuple& tuple, const newElement& element) {
//    return std::tuple_cat(tuple, std::make_tuple(element));
//}
//
//template <typename Head, typename... Tail>
//std::tuple<Tail...> tuple_tail(const std::tuple<Head, Tail...>& t) {
//    return std::apply([](auto head, auto... tail) {
//        return std::make_tuple(tail...); }, t);
//}
//
//template <typename Head, typename ... Tail>
//void convertVectorToTuple(std::tuple<Head, Tail...> const& typesTuple, std::tuple<Head, Tail...>& valuesTuple, 
//        std::vector<std::string>& valuesVector, size_t index) {
//    Head element = valuesVector[index];
//    tuple_append(valuesTuple, element);
//    convertVectorToTuple(tuple_tail(typesTuple), valuesTuple, valuesVector, index+1);
//}
//
//template <typename Last>
//void convertVectorToTuple(std::tuple<Last> const& valuesTuple, std::vector<std::string>& valuesVector, size_t index) {
//    Last element = valuesVector[index];
//    tuple_append(valuesTuple, element);
//}