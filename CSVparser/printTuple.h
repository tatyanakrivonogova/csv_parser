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
void type_cast(const std::string& str, T& value) {
    std::stringstream ss;
    ss.str(str);

    ss >> value;
    std::stringstream check;
    check << value;
    if (str != check.str()) {
        //throw ParserError(line_number, index + 1);
    }
}

template<>
void type_cast<std::string>(const std::string& str, std::string& value) {
    value = str;
}







template <typename newElement, typename ... TupleArgs>
std::tuple<TupleArgs..., newElement> tuple_append(const std::tuple<TupleArgs...>& tuple, const newElement& element) {
    return std::tuple_cat(tuple, std::make_tuple(element));
}


template<int index, int size, typename... Args>
struct addToTuple {
    static void Next(std::vector<std::string, std::allocator<std::string>>& valuesVector, std::tuple<Args...>& tuple, int line_number) {
        addToTuple<index - 1, size, Args...>::Next(valuesVector, tuple, line_number);
        //Convert(line[index], std::get<index>(tuple), line_number, index, size);
        type_cast(valuesVector[index], std::get<index>(tuple));
    }
};

template<int size, typename... Args>
struct addToTuple<0, size, Args...> {
    static void Next(std::vector<std::string, std::allocator<std::string>>& valuesVector, std::tuple<Args...>& tuple, int line_number) {
        type_cast(valuesVector[0], std::get<0>(tuple));
    }
};


//template <typename Head, typename ... Args>
//void convertVectorToTuple(std::tuple<Head> typesTuple, std::tuple<Args...>& valuesTuple,
//    std::vector<std::string, std::allocator<std::string>>& valuesVector, size_t index) {
//    Head element = type_cast<Head>(valuesVector[index]);
//    tuple_append(valuesTuple, element);
//    //std::cout << "!!!";
//}
//
//
//template <typename Head, typename ... Tail, typename ... Args>
//void convertVectorToTuple(std::tuple<Head, Tail...> typesTuple, std::tuple<Args...>& valuesTuple, std::vector<std::string, std::allocator<std::string>>& valuesVector, size_t index) {
//    Head element = type_cast<Head>(valuesVector[index]);
//    tuple_append(valuesTuple, element);
//    //std::get<index>(valuesTuple) = element;
//    convertVectorToTuple(tuple_tail(typesTuple), valuesTuple, valuesVector, index + 1);
//    //std::cout << "***";
//}
//
//template <typename ... Args>
//void convertVectorToTuple(std::tuple<> typesTuple, std::tuple<Args...>& valuesTuple,
//    std::vector<std::string, std::allocator<std::string>>& valuesVector, size_t index) {
//    std::cout << "!!!";
//}

//template <typename ... Tail, typename ... Args>
//void convertVectorToTuple(std::tuple<Tail...> typesTuple, std::tuple<Args...>& valuesTuple, std::vector<std::string, std::allocator<std::string>>& valuesVector, size_t index) {
//    using newType = typename std::tuple_element<0, decltype(typesTuple)>::type;
//    newType element = type_cast<newType>(valuesVector[index]);
//    tuple_append(valuesTuple, element);
//    convertVectorToTuple(tuple_tail(typesTuple), valuesTuple, valuesVector, index + 1);
//    //std::cout << "***";
//}




