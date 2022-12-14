#pragma once
#include <tuple>
#include <type_traits>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>


//for writing tuple
template <typename CharT, typename Traits, typename T>
struct printElement {
    static void print(std::basic_ostream<CharT, Traits>& os, T elem) {
        os << elem << " ";
    }
};


//template <typename CharT, typename Traits>
//struct printElement<CharT, Traits, std::string> {
//    static void print(std::basic_ostream<CharT, Traits>& os, std::string elem) {
//        for (auto it = elem.begin(); it < elem.end(); ++it) {
//            if (*it == '\\') {
//                if (*(it+1) == 'n') {
//                    os << "\n";
//                    ++it;
//                }
//                else if (*(it + 1) == 't') {
//                    os << "\t";
//                    ++it;
//                }
//            }
//            else {
//                os << *it;
//            }
//        }
//        os << " ";
//    }
//};

template <typename Head, typename... Tail>
std::tuple<Tail...> tuple_tail(const std::tuple<Head, Tail...>& t) {
    return std::apply([](Head head, Tail... tail) {
        return std::make_tuple(tail...); }, t);
}

template <typename CharT, typename Traits, typename Head, typename ... Tail>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, std::tuple<Head, Tail...> const& t) {
    printElement<CharT, Traits, Head>::print(os, std::get<0>(t));
    //printElement(os, std::get<0>(t));
    return os << tuple_tail(t);
}

template <typename CharT, typename Traits, typename Last>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, std::tuple<Last> const& t) {
    printElement<CharT, Traits, Last>::print(os, std::get<0>(t));
    //printElement(os, std::get<0>(t));
    return os;
}

//for type_cast
template <typename T>
void type_cast(const std::string& str, T& value, size_t lineIndex, size_t index) {
    std::stringstream ss;
    ss.str(str);

    ss >> value;
    std::stringstream check;
    check << value;
    if (str != check.str()) {
        throw std::runtime_error("Bad type of field number " + std::to_string(index+1) + " in line " + std::to_string(lineIndex));
    }
}

template <>
void type_cast<char>(const std::string& str, char& value, size_t lineIndex, size_t index) {
    /*std::stringstream ss;
    ss.str(str);

    ss >> value;
    std::stringstream check;
    check << value;
    if (str != check.str()) {
        throw std::runtime_error("Bad type of field number " + std::to_string(index + 1) + " in line " + std::to_string(lineIndex));
    }*/
    if (str.length() > 1) {
        throw std::runtime_error("Bad type of field number " + std::to_string(index + 1) + " in line " + std::to_string(lineIndex));
    }
    else {
        value = str[0];
    }
}

template<>
void type_cast<std::string>(const std::string& str, std::string& value, size_t lineIndex, size_t index) {
    value = str;
}


// for reading tuple
template<size_t index, size_t size, typename... Args>
struct addToTuple {
    static void Next(std::vector<std::string, std::allocator<std::string>>& valuesVector, std::tuple<Args...>& tuple, int lineIndex) {
        addToTuple<index - 1, size, Args...>::Next(valuesVector, tuple, lineIndex);
        type_cast(valuesVector[index], std::get<index>(tuple), lineIndex, index);
    }
};

template<size_t size, typename... Args>
struct addToTuple<0, size, Args...> {
    static void Next(std::vector<std::string, std::allocator<std::string>>& valuesVector, std::tuple<Args...>& tuple, int lineIndex) {
        type_cast(valuesVector[0], std::get<0>(tuple), lineIndex, 0);
    }
};