#include <iostream>
#include <fstream>
#include <string>
#include <optional>
#include <vector>
#include <sstream>
#include <map>

// Line of the input format:
// Game {Game number} : [{number of balls} {color},];

std::vector<std::string> split(std::string s, char delimiter) {
    size_t pos_start = 0, pos_end, delim_len = 1;
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

bool solveDay2Part1(const std::string& p_line)
{       
    static std::map<std::string, int> color_to_threshold {{"red", 12}, {"green", 13}, {"blue", 14}};

    std::vector<std::string> parsed_line = split(p_line, ':');
    for(std::string round : split(parsed_line.at(1), ';'))
    {
        for(std::string result : split(round, ','))
        {   
            //result " number color"
            std::vector<std::string> pair = split(result, ' ');
            std::string color = pair.at(2);
            int number = std::stoi(pair.at(1));

            //std::cout <<  color << " " << number << ", ";

            if(number > color_to_threshold.at(color))
            {   
                //std::cout << std::endl << std::endl;
                return false;
            }
        }
        //std::cout << "; ";
    }
    //std::cout << std::endl << std::endl;
    return true;
}

int solveDay2Part2(const std::string& p_line)
{       
    std::map<std::string, int> color_to_threshold {{"red", 0}, {"green", 0}, {"blue", 0}};

    std::vector<std::string> parsed_line = split(p_line, ':');
    for(std::string round : split(parsed_line.at(1), ';'))
    {
        for(std::string result : split(round, ','))
        {   
            std::vector<std::string> pair = split(result, ' ');
            std::string color = pair.at(2);
            int number = std::stoi(pair.at(1));

            if(number > color_to_threshold.at(color))
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
    int index = 1;
    while(std::getline(input_file, line))
    {   
        //std::cout << index;
        if(solveDay2Part1(line))
        {
            resultPart1Sum += index;
        }
        index ++;

        resultPart2Sum += solveDay2Part2(line);
    }

    std::cout << "Part 1 result: " << resultPart1Sum << std::endl;
    std::cout << "Part 2 result: " << resultPart2Sum << std::endl;

    input_file.close();

    return 0;
};
