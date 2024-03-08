#include "TextUtils.hpp"
#include <map>
#include <limits>

struct AlmanacConversion
{
    unsigned long input_min;
    unsigned long input_max;
    unsigned long shift;
};

int main()
{
    std::fstream input_file;
    input_file.open("input_day5.txt", std::ios::in);

    std::cout << std::boolalpha;
    std::cout << "Day 5, lets go!" << std::endl;

    std::string line;

    unsigned long resultPart2 = 0;

    // Get Seeds
    std::getline(input_file, line);
    std::vector<std::string> seeds = TextUtils::Split(line, " ");
    
    std::getline(input_file, line);
    
    // Get Almanac Maps
    std::vector<std::vector<AlmanacConversion>> almanac;

    for (unsigned long i = 0; i < 7; i++)
    {
        if (!std::getline(input_file, line))
        {
            break;
        }

        std::vector<AlmanacConversion> almanac_map;
        while (std::getline(input_file, line) && line != "")
        {
            auto almanac_elem = TextUtils::Split(line, " ");
            unsigned long output = std::stoul(almanac_elem[0]);
            unsigned long input = std::stoul(almanac_elem[1]);
            unsigned long range = std::stoul(almanac_elem[2]);
            
            almanac_map.push_back({input, input+range-1, output-input });
            
        }
        almanac.push_back(almanac_map);
    }

    std::cout << "All parsed, now computing solution" << std::endl;

    // Solve Part1
    unsigned long resultPart1 = std::numeric_limits<unsigned long>::max();
    for (int i = 1; i < seeds.size(); i++)
    {   
        unsigned long seed = std::stoul(seeds[i]);
        unsigned long location = seed;
        for (auto almanac_conversions : almanac)
        {
            auto it = std::find_if(almanac_conversions.cbegin(),almanac_conversions.cend(), [location](const AlmanacConversion& alma_elem){
                return (location>=alma_elem.input_min) && (location<=alma_elem.input_max);});

            if(it != almanac_conversions.cend())
            {
                location += it->shift;
            }
            
        }
        resultPart1 = std::min(resultPart1, location);
    }

    std::cout << "Part 1: " << resultPart1 << std::endl;

    return 0;
}
