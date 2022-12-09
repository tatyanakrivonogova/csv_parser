#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include "CSVparser.h"
#include "printTuple.h"

int main()
{
    //std::ifstream file("test.csv");
    //CSVparser<int, std::string> parser(file, 0 /*skip first lines count*/);
    //for (std::tuple<int, std::string> rs : parser) {
    //    std::cout << rs << std::endl;
    //}

    auto t = std::make_tuple(1, "abc", 5.0);
    std::cout << t;
}
