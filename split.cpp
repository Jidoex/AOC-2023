#include<string>
#include<vector>
#include<iostream>

std::vector<std::string> split_char(std::string s, char delimiter) {
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

size_t find_multiple(std::string s, std::string delimiters, size_t _start = 0)
{
    size_t min_index = s.size() + 1;
    for(auto delimiter : delimiters)
    {
        size_t c_index = s.find(delimiter, _start);
        if(delimiter != std::string::npos && min_index > c_index)
        {
            min_index = c_index;
        }
    }
    
    return (min_index > s.size()) ? std::string::npos : min_index;
}

std::vector<std::string> split_multiple_char(std::string s, std::string delimiter) {
    size_t pos_start = 0;
    size_t pos_end = 0;
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = find_multiple(s, delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + 1;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

int main() 
{
    std::string line = "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53";
    auto split_line = split_multiple_char(line, ":|");
    std::cout << split_line.at(0) << std::endl;
    std::cout << "Winning Numbers : " << split_line.at(1) << std::endl;
    std::cout << "My Numbers : " << split_line.at(2) << std::endl;
    std::cout << std::endl;
    return 0;
}
