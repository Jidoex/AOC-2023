#include "TextUtils.hpp"
#include <cmath>
#include <map>

int solve(const std::string &line)
{
    auto split_line = TextUtils::Split(line, ":|");
    auto winning_numbers = TextUtils::Split(split_line[1], " ");
    auto my_numbers = TextUtils::Split(split_line[2], " ");

    int exponent = 0;

    for (auto winning_number : winning_numbers)
    {
        for (auto my_number : my_numbers)
        {
            if (my_number != "" && winning_number != "" && my_number == winning_number)
            {
                exponent += 1;
            }
        }
    }

    return exponent;
}

int main(int p_argv, char **p_argc)
{
    std::fstream input_file;
    input_file.open("input_day4.txt", std::ios::in);

    std::cout << std::boolalpha;
    std::cout << "Day 4, lets go!" << std::endl;

    std::string line;

    int resultPart1 = 0;
    int resultPart2 = 0;
    int line_index = 0;
    std::map<int, int> numberOfCopies;
    while (std::getline(input_file, line))
    {
        int match = solve(line);

        resultPart1 += (match > 0) ? pow(2, match - 1) : 0;

        numberOfCopies[line_index] += 1;
        resultPart2 += numberOfCopies[line_index];

        for (int i = line_index + 1; i <= line_index + match; i++)
        {
            numberOfCopies[i] += numberOfCopies[line_index];
        }

        line_index++;
    }

    std::cout << "Part 1: " << resultPart1 << std::endl;
    std::cout << "Part 2: " << resultPart2 << std::endl;
}
