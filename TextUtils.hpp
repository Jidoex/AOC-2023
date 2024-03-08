#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

struct TextUtils 
{   
    static std::vector<std::string> Split(const std::string& s, std::string delimiter);
    static std::vector<std::string> Split(const std::string& s, char delimiter)
    {
        return Split(s, std::string{delimiter});
    };
    static std::vector<std::string> WIP_Split(std::string s, std::string delimiter);
};
