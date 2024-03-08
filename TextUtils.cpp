
#include "TextUtils.hpp" 

std::vector<size_t> FindAll(const std::string& s, char c)
{   
    size_t pos_start = 0;
    size_t pos_end = 0;
    std::vector<size_t> res;

    while ((pos_end = s.find(c, pos_start)) != std::string::npos) {
        pos_start = pos_end + 1;
        res.push_back (pos_end);
    }

    return res;
}

std::vector<std::string> TextUtils::Split(const std::string& s, std::string delimiter) 
{
    std::string token;
    std::vector<std::string> res;
    
    std::vector<size_t> delimiter_indices;

    for(auto c : delimiter)
    {
        auto result = FindAll(s, c);
        delimiter_indices.insert(delimiter_indices.end(), result.begin(), result.end());
    }
    std::sort(delimiter_indices.begin(), delimiter_indices.end());

    size_t pos_start = 0;
    size_t delim_len = 1;
    for(auto i : delimiter_indices)
    {   
        token = s.substr (pos_start, i - pos_start);
        res.push_back (token);

        pos_start = i + delim_len;
    }

    res.push_back (s.substr (pos_start));
    return res;
} 

std::vector<std::string> TextUtils::WIP_Split(std::string s, std::string delimiter) {
    
    std::vector<std::string> res;
    std::vector<std::string> residues = {s};

    int i = 0;
    while(!residues.empty())
    {   
        for(auto part : residues){
            for(auto c : part){
                if(delimiter.find(c))
                {
                    auto result = Split(s, c);
                    res.push_back(result[0]);
                }
            }
        }
    }

    return res;
} 
