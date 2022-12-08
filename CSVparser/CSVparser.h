#pragma once
#include <iostream>
#include <tuple>
#include <exception>
#include "inputIterator.h"

template <typename ... Args>
class CSVparser {
private:
	std::ifstream& inputFile;
	size_t skipLines;
	int linesNumber = -1; // or 0
	char lineDelimeter;
	char columnDelimeter;
	
	template <typename CharT, typename Traits, typename Alloc>
	void getLine(std::basic_istream<CharT, Traits>& is, std::basic_string<CharT, Traits, Alloc>& str) {
		str.clear();
		char curr_symbol;
		while (is.get(curr_symbol) != lineDelimeter) {
			str += curr_symbol;
		}
	}

	int getLinesNumber() {
		if (linesNumber == -1) {
			size_t lineCount = 0;
			std::string line;
			inputFile.seekg(0, std::ios::beg);
			while (std::getline(inputFile, line)) {
				++lineCount;
			}
			inputFile.seekg(0, std::ios::beg);
			linesNumber = lineCount;
		}
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
				parser.getLine(inputFile, currentLine);
			}
		}

		inputIterator operator++() { //to do
			if (index < parser.linesNumber) {
				++index;
				inputFile.seekg(0, std::ios::beg);
				for (size_t i = 0; i < index; ++i) {
					parser.getLine(inputFile, currentLine);
				}
			}
			else {
				currentLine.clear();
				is_end = true;
			}
			return *this;
		}

		const bool operator==(const inputIterator& other) {
			if (inputFile != other.inputFile) {
				return false;
			}
			if (index != other.index) {
				return false;
			}
			if (parser != other.parser) {
				return false;
			}
			if (is_end != other.is_end) {
				return false;
			}
			return true;
		}

		const bool operator!=(const inputIterator& other) {
			return !(*this == other);
		}

		std::tuple<Args...> operator*() {
			return CSVparser.parseLine(); // to do
		}
	};

public:

	CSVparser(std::ifstream& inputFile, size_t skipLines = 1, char lineDelimeter = '\n', char columnDelimeter = ';') :
		inputFile(inputFile), skipLines(skipLines), lineDelimeter(lineDelimeter), columnDelimeter(columnDelimeter) {
		linesNumber = getLinesNumber();
		if (skipLines < 0 or skipLines > linesNumber) {
			throw std::invalid_argument("Wrong skip lines number");
		}
		if (!inputFile.is_open()) {
			throw std::ofstream::failbit("File is not opened");
		}
	}

	inputIterator begin() {
		inputIterator begin(inputFile, 1, *this);
		return begin;
	}

	inputIterator end() {
		inputIterator end(inputFile, linesNumber, *this);
		end.is_end = true;
		end.currentLine.clear();
		return end;
	}

	std::tuple<Args...> parseLine(std::string& line, size_t lineIndex) { //to do

	}


};