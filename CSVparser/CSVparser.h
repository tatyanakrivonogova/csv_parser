#pragma once
#include <iostream>
#include <tuple>
#include <exception>
#include <vector>
#include "inputIterator.h"
#include "printTuple.h"

template <typename ... Args>
class CSVparser {
private:
	std::ifstream& inputFile;
	size_t skipLines;
	//int linesNumber = -1; // or 0
	size_t linesNumber = 0;
	char lineDelimeter;
	char columnDelimeter;

	template <typename CharT, typename Traits, typename Alloc>
	void getDataLine(std::basic_istream<CharT, Traits>& is, std::basic_string<CharT, Traits, Alloc>& str) {
		str.clear();
		char curr_symbol;
		is.get(curr_symbol);
		//is >> curr_symbol;
		while (curr_symbol != lineDelimeter) {
			str += curr_symbol;
			is.get(curr_symbol);
			//is >> curr_symbol;
		}
		/*std::cout << is.tellg() << std::endl;
		std::getline(is, str);
		std::cout << str << std::endl;*/
	}

	int getLinesNumber() {
		inputFile.clear();
		if (linesNumber == 0) {
			size_t lineCount = 0;
			std::string line;
			inputFile.seekg(0, std::ios::beg);
			while (std::getline(inputFile, line)) { // or getDataLine
				++lineCount;
			}
			inputFile.clear();
			std::cout << inputFile.tellg() << std::endl;
			linesNumber = lineCount;
		}
		inputFile.seekg(0, std::ios::beg);
		std::cout << inputFile.tellg() << std::endl;
		return linesNumber;
	}

	class inputIterator {
	private:
		std::ifstream& inputFile;
		std::string currentLine = "";
		size_t index;
		CSVparser<Args...>& parser;
		bool is_end = false;

		friend class CSVparser;
	public:
		inputIterator(std::ifstream& inputFile, size_t index, CSVparser<Args...>& parser) :
			inputFile(inputFile), index(index), parser(parser) {
			for (size_t i = 0; i < index; ++i) {
				parser.getDataLine(inputFile, currentLine);
			}
		}

		inputIterator operator++() {
			if (index < parser.linesNumber) {
				++index;
				inputFile.clear();
				inputFile.seekg(0, std::ios::beg);
				for (size_t i = 0; i < index; ++i) {
					parser.getDataLine(inputFile, currentLine);
				}
			}
			else {
				currentLine.clear();
				is_end = true;
			}
			return *this;
		}

		const bool operator==(const inputIterator& other) {
			/*if (inputFile != other.inputFile) {
				return false;
			}*/
			if (currentLine != other.currentLine) {
				return false;
			}
			if (index != other.index) {
				return false;
			}
			/*if (parser != other.parser) {
				return false;
			}*/
			if (is_end != other.is_end) {
				return false;
			}
			return true;
		}

		const bool operator!=(const inputIterator& other) {
			return !(*this == other);
		}

		std::tuple<Args...> operator*() {
			return parser.parseLine(currentLine, index);
		}
	};

public:

	CSVparser(std::ifstream& inputFile, size_t skipLines = 1, char lineDelimeter = '\n', char columnDelimeter = ';') :
		inputFile(inputFile), skipLines(skipLines), lineDelimeter(lineDelimeter), columnDelimeter(columnDelimeter) {
		linesNumber = getLinesNumber();
		if (skipLines < 0 or skipLines > linesNumber) {
			//throw std::invalid_argument("Wrong skip lines number");
		}
		if (!inputFile.is_open()) {
			//throw std::runtime_error("File is not opened");
		}
	}

	inputIterator begin() {
		inputIterator begin(inputFile, skipLines+1, *this);
		return begin;
	}

	inputIterator end() {
		inputIterator end(inputFile, 0, *this);
		end.is_end = true;
		end.currentLine.clear();
		return end;
	}

	std::tuple<Args...> parseLine(std::string& line, size_t lineIndex) { //to do
		size_t size = sizeof...(Args);

		if (line.empty()) {
			//throw std::invalid_argument("Empty line " + std::to_string(lineIndex));
		}

		std::vector<std::string> fields = stringToVector(line, lineIndex);

		if (fields.size() != size) {
			//throw std::invalid_argument("Number of fields in line " + std::to_string(lineIndex) + " is wrong");
		}

		std::tuple<Args...> types_tuple;
		std::tuple<Args...> data_tuple;
		//std::tuple<Args...>* data_tuple = new std::tuple<Args...>;

		convertVectorToTuple(types_tuple, data_tuple, fields, 0);


		return data_tuple;
	}


	std::vector<std::string> stringToVector(std::string& line, size_t lineIndex) {
		size_t pos = 0;

		std::vector<std::string> fieldsVector;
		std::string current_field = "";

		while (pos < line.length()) {
			if (line[pos] == columnDelimeter) {
				fieldsVector.push_back(current_field);
				current_field.clear();
				++pos;
			}
			current_field += line[pos];
			++pos;
		}

		if (!current_field.empty()) {
			fieldsVector.push_back(current_field);
		}

		return fieldsVector;
	}
};