#include "TextUtils.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

bool solveDay2Part1(const std::string& p_line)
{
	static std::map<std::string, int> color_to_threshold {
		{  "red", 12},
		{"green", 13},
		{ "blue", 14}
	};

	std::vector<std::string> parsed_line = TextUtils::Split(p_line, ':');
	for (std::string round : TextUtils::Split(parsed_line.at(1), ';'))
	{
		for (std::string result : TextUtils::Split(round, ','))
		{
			// result " number color"
			std::vector<std::string> pair   = TextUtils::Split(result, ' ');
			std::string              color  = pair.at(2);
			int                      number = std::stoi(pair.at(1));

			// std::cout <<  color << " " << number << ", ";

			if (number > color_to_threshold.at(color))
			{
				// std::cout << std::endl << std::endl;
				return false;
			}
		}
		// std::cout << "; ";
	}
	// std::cout << std::endl << std::endl;
	return true;
}

int solveDay2Part2(const std::string& p_line)
{
	std::map<std::string, int> color_to_threshold {
		{  "red", 0},
		{"green", 0},
		{ "blue", 0}
	};

	std::vector<std::string> parsed_line = TextUtils::Split(p_line, ':');
	for (std::string round : TextUtils::Split(parsed_line.at(1), ';'))
	{
		for (std::string result : TextUtils::Split(round, ','))
		{
			std::vector<std::string> pair   = TextUtils::Split(result, ' ');
			std::string              color  = pair.at(2);
			int                      number = std::stoi(pair.at(1));

			if (number > color_to_threshold.at(color))
			{
				color_to_threshold[color] = number;
			}
		}
	}
	return color_to_threshold["red"] * color_to_threshold["green"] * color_to_threshold["blue"];
}

int main(int p_argv, char** p_argc)
{
	std::fstream input_file;
	input_file.open("input_day2.txt", std::ios::in);

	std::string line;

	int resultPart1Sum = 0;
	int resultPart2Sum = 0;
	int index          = 1;
	while (std::getline(input_file, line))
	{
		// std::cout << index;
		if (solveDay2Part1(line))
		{
			resultPart1Sum += index;
		}
		index++;

		resultPart2Sum += solveDay2Part2(line);
	}

	std::cout << "Part 1 result: " << resultPart1Sum << std::endl;
	std::cout << "Part 2 result: " << resultPart2Sum << std::endl;

	input_file.close();

	return 0;
};
