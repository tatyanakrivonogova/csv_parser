#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include "CSVparser.h"
#include "tupleTools.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    std::ifstream file("test.csv");
    if (!file.is_open()) {
        std::cout << "Impossible to read input file" << std::endl;
        return -1;
    }

    try {
        CSVparser<int, char> parser(file, 0, ';', ',');
        for (std::tuple<int, char> rs : parser) {
            std::cout << rs << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}