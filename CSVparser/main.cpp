#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include "CSVparser.h"
#include "tupleTools.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    std::ifstream file("out.csv");
    if (!file.is_open()) {
        std::cout << "Impossible to read input file" << std::endl;
        return -1;
    }

    try {
        CSVparser<std::string, int, double> parser(file, 0, '\n', ';');
        for (std::tuple<std::string, int, double> rs : parser) {
            std::cout << rs << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}