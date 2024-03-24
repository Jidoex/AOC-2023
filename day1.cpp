#include <fstream>
#include <iostream>
#include <optional>
#include <string>

std::pair<int, int> solveDay1(const std::string& p_line)
{
	std::optional<int> first_digit;
	int                last_digit;
	for (const auto& elem : p_line)
	{
		if ((!first_digit.has_value()) && (elem >= '0') && (elem <= '9'))
		{
			first_digit = elem - '0';
		}
		if ((elem >= '0') && (elem <= '9'))
		{
			last_digit = elem - '0';
		}
	}

	return std::make_pair(first_digit.value(), last_digit);
}

int main(int p_argv, char** p_argc)
{
	std::fstream input_file;
	input_file.open("input_day1.txt", std::ios::in);

	std::string line;

	long calibrationValueSum = 0;
	int  index               = 1;
	while (std::getline(input_file, line))
	{
		auto result = solveDay1(line);
		calibrationValueSum += result.first * 10 + result.second;
	}

	std::cout << "Calibration Value: " << calibrationValueSum << std::endl;

	input_file.close();

	return 0;
};
