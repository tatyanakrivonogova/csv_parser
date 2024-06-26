#pragma once
#include <iostream>
#include <tuple>
#include <exception>
#include <vector>
#include "tupleTools.h"

template <typename ... Args>
class CSVparser {
private:
	std::ifstream& inputFile;
	size_t skipLines;
	size_t linesNumber = 0;
	std::streamoff charNumber = 0;
	char lineDelimeter;
	char columnDelimeter;

	template <typename CharT, typename Traits, typename Alloc>
	void getDataLine(std::basic_istream<CharT, Traits>& is, std::basic_string<CharT, Traits, Alloc>& str) {
		str.clear();
		char curr_symbol;
		is.get(curr_symbol);
		while (is.tellg() != -1 and is.tellg() <= charNumber and curr_symbol != lineDelimeter) {
			str += curr_symbol;
			is.get(curr_symbol);
		}
	}

	int getLinesNumber() {
		inputFile.clear();
		if (linesNumber == 0) {
			size_t lineCount = 0;
			std::string line;
			inputFile.seekg(0, std::ios::beg);
			getDataLine(inputFile, line);
			while (!line.empty()) {
				++lineCount;
				getDataLine(inputFile, line);
			}
			inputFile.clear();
			linesNumber = lineCount;
		}
		inputFile.seekg(0, std::ios::beg);
		return linesNumber;
	}

	class inputIterator {
		friend class CSVparser;
	public:
		using data_type = std::tuple<Args...>;


		inputIterator(std::ifstream& inputFile, size_t index, CSVparser<Args...>& parser = nullptr) :
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
			if (is_end == other.is_end) { //it == end()
				return true;
			}
			if (currentLine != other.currentLine) {
				return false;
			}
			if (index != other.index) {
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

		std::tuple<Args...>& operator*() {
			parser.parseLine(currentLine, current_tuple, index);
			return current_tuple;
		}

	private:
		data_type current_tuple;
		std::ifstream& inputFile;
		std::string currentLine = "";
		size_t index;
		CSVparser<Args...>& parser;
		bool is_end = false;
	};

public:

	CSVparser(std::ifstream& inputFile, size_t skipLines = 1, char lineDelimeter = '\n', char columnDelimeter = ';') :
		inputFile(inputFile), skipLines(skipLines), lineDelimeter(lineDelimeter), columnDelimeter(columnDelimeter) {
		
		inputFile.seekg(0, std::ios_base::end);
		charNumber = inputFile.tellg();
		inputFile.seekg(0, std::ios::beg);
		
		linesNumber = getLinesNumber();


		if (skipLines < 0 or skipLines > linesNumber) {
			throw std::invalid_argument("Wrong skip lines number");
		}
		if (!inputFile.is_open()) {
			throw std::runtime_error("File is not opened");
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

	void parseLine(std::string& line, std::tuple<Args...>& data_tuple, size_t lineIndex) { //to do
		int const size = std::tuple_size<std::tuple<Args...>>::value;

		if (line.empty()) {
			throw std::invalid_argument("Empty line number " + std::to_string(lineIndex));
		}

		escapeCharReplace(line);
		std::vector<std::string> fields = stringToVector(line, lineIndex);

		if (fields.size() != size) {
			throw std::invalid_argument("Number of fields in line " + std::to_string(lineIndex) + " is wrong");
		}

		addToTuple<size - 1, size, Args...>::Next(fields, data_tuple, lineIndex);
	}


	std::vector<std::string> stringToVector(std::string& line, size_t lineIndex) {
		size_t pos = 0;

		std::vector<std::string> fieldsVector;
		std::string current_field = "";

		size_t length = line.length();
		while (pos < length) {
			if (line[pos] == columnDelimeter) {
				fieldsVector.push_back(current_field);
				current_field.clear();
				++pos;
			}
			else {
				current_field += line[pos];
				++pos;
			}
		}

		if (!current_field.empty()) {
			fieldsVector.push_back(current_field);
		}

		return fieldsVector;
	}

	void escapeCharReplace(std::string& str) {
		auto it = str.begin();
		while (it < str.end()) {
			if (*it == '\\') {
				if (*(it + 1) == 'n') {
					str.erase(it, it + 2);
					str.insert(it, '\n');
				}
				else if (*(it + 1) == 't') {
					str.erase(it, it + 2);
					str.insert(it, '\t');
				}
				else if (*(it + 1) == 'r') {
					str.erase(it, it + 2);
					str.insert(it, '\r');
				}
				else if (*(it + 1) == 'b') {
					str.erase(it, it + 2);
					str.insert(it, '\b');
				}
				else if (*(it + 1) == '\'') {
					str.erase(it, it + 2);
					str.insert(it, '\'');
				}
				else if (*(it + 1) == '\"') {
					str.erase(it, it + 2);
					str.insert(it, '\"');
				}
				else if (*(it + 1) == '\\') {
					str.erase(it, it + 2);
					str.insert(it, '\\');
				}
			}
			++it;
		}
	}
};