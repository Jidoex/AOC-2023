#include "TextUtils.hpp"
#include <map>
#include <limits>
#include <numeric>
struct AlmanacConversion
{
    using Vect = std::vector<AlmanacConversion>;
    using VectOfVect = std::vector<Vect>;
    unsigned long input_min;
    unsigned long input_max;
    long long shift;
};

struct SeedRange{
    using Vect = std::vector<SeedRange>;
    unsigned long seed_min;
    unsigned long seed_max;
};


bool hasIntersection(const AlmanacConversion& p_almanac, const SeedRange& p_seedRange)
{
    return (p_seedRange.seed_max>=p_almanac.input_min) && (p_seedRange.seed_min<=p_almanac.input_max);
}

void cleanifySeedRanges(SeedRange::Vect& p_seedRanges)
{
    SeedRange::Vect cleanedRanges = p_seedRanges;
    
    for(auto it = cleanedRanges.begin(); it < (cleanedRanges.end());)
    {
        if(it->seed_max < it->seed_min)
        {
            it = cleanedRanges.erase(it);
            continue;
        }
        it++;
    }

    std::sort(cleanedRanges.begin(),cleanedRanges.end(), [](const SeedRange& p_a, const SeedRange& p_b){return p_a.seed_min < p_b.seed_min;});

    for(auto it = cleanedRanges.begin(); it < (cleanedRanges.end()-1);)
    {
        if(it->seed_max >= (it+1)->seed_min)
        {
            (it+1)->seed_min = std::min(it->seed_min, (it+1)->seed_min);
            (it+1)->seed_max = std::max(it->seed_max, (it+1)->seed_max);
            it = cleanedRanges.erase(it);
            continue;
        }
        it++;
    }

    p_seedRanges = cleanedRanges;
}

unsigned long minimalSeed(const SeedRange::Vect& p_seedRanges)
{
    unsigned long minimalVal = std::numeric_limits<unsigned long>::max();

    for(const auto& seed: p_seedRanges)
    {
        minimalVal = std::min(minimalVal, seed.seed_min);
    }
    return minimalVal;
}

unsigned long applyAlmanac(const AlmanacConversion::VectOfVect& p_almanac, const SeedRange::Vect& p_seedRanges)
{
    SeedRange::Vect result = std::accumulate(p_almanac.cbegin(), p_almanac.cend(), p_seedRanges, []( const SeedRange::Vect& p_inputSeed, const AlmanacConversion::Vect& p_eachAlmanac)
            {
                std::cout << "Number of seed ranges " << p_inputSeed.size() << " and minimal val " << minimalSeed(p_inputSeed) << std::endl;
                SeedRange::Vect outputRanges;
                for( const SeedRange& p_seed : p_inputSeed)
                {
                    bool wasFound=false;
                    for(const AlmanacConversion& eachAlmanac : p_eachAlmanac)
                    {
                        if( hasIntersection(eachAlmanac,p_seed))
                        {
                            SeedRange outputRange;
                            outputRange.seed_min = std::max(eachAlmanac.input_min,p_seed.seed_min) + eachAlmanac.shift;
                            outputRange.seed_max = std::min(eachAlmanac.input_max,p_seed.seed_max) + eachAlmanac.shift;

                            outputRanges.push_back(outputRange);
                            outputRanges.push_back({p_seed.seed_min, std::max(eachAlmanac.input_min,p_seed.seed_min)-1});
                            outputRanges.push_back({std::min(eachAlmanac.input_max,p_seed.seed_max)+1, p_seed.seed_max});
                            wasFound=true;
                            break;
                        }
                    }
                    if(!wasFound) {
                        outputRanges.push_back(p_seed);
                    }

                }
                cleanifySeedRanges(outputRanges);
                return outputRanges;
            });

    return minimalSeed(result);
}

int main()
{
    std::fstream input_file;
    input_file.open("input_day5.txt", std::ios::in);

    std::cout << std::boolalpha;
    std::cout << "Day 5, lets go!" << std::endl;

    std::string line;

    // Get Seeds
    std::getline(input_file, line);
    std::vector<std::string> seedsString = TextUtils::Split(line, " ");


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
            
            almanac_map.push_back({input, input+range-1, static_cast<long long>(output-input) });
            
        }
        almanac.push_back(almanac_map);
    }

    std::cout << "All parsed, now computing solution" << std::endl;

    // Solve Part1
    unsigned long resultPart1 = std::numeric_limits<unsigned long>::max();
    for (int i = 1; i < seedsString.size(); i++)
    {   
        unsigned long seed = std::stoul(seedsString[i]);
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


    std::vector<SeedRange> seedRanges;
    std::vector<SeedRange> seedsRangePart1;

    unsigned long resultPart2 = std::numeric_limits<unsigned long>::max();

    // Create the seed ranges from the input
    for (int i = 1; i < (seedsString.size() -1 ); i=i+2)
    {
        SeedRange seedRange;
        seedRange.seed_min = std::stoul(seedsString[i]);
        seedRange.seed_max=seedRange.seed_min + (std::stoul(seedsString[i+1])-1);
        seedRanges.push_back(seedRange);
    }

    std::cout << "Part 2: " << std::endl << applyAlmanac(almanac,seedRanges) << std::endl;

    return 0;
}
