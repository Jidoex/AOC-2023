#include "TextUtils.hpp"
#include <map>
#include <limits>

int main()
{
    std::fstream input_file;
    input_file.open("input_day5.txt", std::ios::in);

    std::cout << std::boolalpha;
    std::cout << "Day 5, lets go!" << std::endl;

    std::string line;

    int resultPart2 = 0;

    // Get Seeds
    std::getline(input_file, line);
    std::vector<std::string> seeds = TextUtils::Split(line, " ");
    // std::vector<int> seeds;
    // std::transform(first_line.begin() + 1, first_line.end(),
    //                std::back_inserter(seeds), [](std::string input) -> int
    //                { return std::stoi(input); });
    std::getline(input_file, line);

    // Get Almanac Maps
    std::vector<std::map<int, int>> almanac;

    for (int i = 0; i < 7; i++)
    {
        if (!std::getline(input_file, line))
        {
            break;
        }

        std::map<int, int> almanac_map;
        while (std::getline(input_file, line) && line != "")
        {
            auto almanac_elem = TextUtils::Split(line, " ");
            for (int j = 0; j < std::stoi(almanac_elem[2]); j++)
            {
                almanac_map[std::stoi(almanac_elem[1]) + j] = j + std::stoi(almanac_elem[0]);
            }
        }
        almanac.push_back(almanac_map);
    }

    // Solve Part1
    int resultPart1 = std::numeric_limits<int>::max();
    for (int i = 1; i < seeds.size(); i++)
    {   
        int seed = std::stoi(seeds[i]);
        int location = seed;
        for (auto map : almanac)
        {
            location = (map.count(location)) ? map[location] : location;
        }
        resultPart1 = std::min(resultPart1, location);
    }

    std::cout << "Part 1: " << resultPart1 << std::endl;

    return 0;
}
