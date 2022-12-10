#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include "CSVparser.h"
#include "printTuple.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    std::ifstream file("test.csv");
    std::cout << file.tellg() << std::endl;
    if (!file.is_open()) {
        std::cout << "Impossible to read file" << std::endl;
        return -1;
    }
    std::cout << file.tellg() << std::endl;
    //std::string str;
    //while (std::getline(file, str)) {
    //    std::cout << str << std::endl;
    //}

    CSVparser<std::string, int, double> parser(file, 0 /*skip first lines count*/);
    for (std::tuple<std::string, int, double> rs : parser) {
        std::cout << rs << std::endl;
    }

    auto t = std::make_tuple(1, "abc", 5.0);
    std::cout << t;
}