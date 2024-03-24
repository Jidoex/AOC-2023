#include "TextUtils.hpp"

#include <limits>
#include <map>
#include <numeric>
#include <vector>

struct AlmanacConversion
{
	using Vect       = std::vector<AlmanacConversion>;
	using VectOfVect = std::vector<Vect>;
	long long input_min;
	long long input_max;
	long long shift;
};

struct SeedRange
{
	using Vect = std::vector<SeedRange>;
	long long seed_min;
	long long seed_max;
};

bool hasIntersection(const AlmanacConversion& p_almanac, const SeedRange& p_seedRange)
{
	return (p_seedRange.seed_max >= p_almanac.input_min) && (p_seedRange.seed_min <= p_almanac.input_max);
}

void cleanifySeedRanges(SeedRange::Vect& p_seedRanges)
{
	SeedRange::Vect cleanedRanges = p_seedRanges;

	std::sort(cleanedRanges.begin(),
	          cleanedRanges.end(),
	          [](const SeedRange& p_a, const SeedRange& p_b)
	          {
		          return p_a.seed_min < p_b.seed_min;
	          });

	if (p_seedRanges.size() > 1)
	{
		for (auto it = cleanedRanges.begin(); it < (cleanedRanges.end() - 1);)
		{
			if (it->seed_max >= (it + 1)->seed_min)
			{
				SeedRange& rangeToBeMerged  = *it;
				SeedRange& rangeToMergeInto = *(it + 1);

				rangeToMergeInto.seed_min = rangeToBeMerged.seed_min;
				rangeToMergeInto.seed_max = std::max(rangeToBeMerged.seed_max, rangeToMergeInto.seed_max);
				it                        = cleanedRanges.erase(it);
				continue;
			}
			it++;
		}
	}
	p_seedRanges = cleanedRanges;
}

long long minimalSeed(const SeedRange::Vect& p_seedRanges)
{
	long long minimalVal = std::numeric_limits<long long>::max();

	for (const auto& seed : p_seedRanges)
	{
		minimalVal = std::min(minimalVal, seed.seed_min);
	}
	return minimalVal;
}

long long applyAlmanac(const AlmanacConversion::VectOfVect& p_almanac, const SeedRange::Vect& p_seedRanges)
{
	auto accumulateLambda = [](const SeedRange::Vect& p_inputSeed, const AlmanacConversion::Vect& p_eachAlmanac)
	{
		std::cout << "Number of seed ranges " << p_inputSeed.size() << " and minimal val " << minimalSeed(p_inputSeed) << std::endl;
		SeedRange::Vect outputRanges;
		for (const SeedRange& p_seed : p_inputSeed)
		{
			bool            wasSeedUpdated = true;
			SeedRange::Vect seedToProcess  = { p_seed };
			while ((seedToProcess.size() != 0) && wasSeedUpdated)
			{
				SeedRange seed = seedToProcess.back();

				wasSeedUpdated = false;
				for (const AlmanacConversion& eachAlmanac : p_eachAlmanac)
				{
					if (hasIntersection(eachAlmanac, seed))
					{
						seedToProcess.pop_back();
						SeedRange outputRange;

						outputRange.seed_min = std::max(eachAlmanac.input_min, seed.seed_min);
						outputRange.seed_max = std::min(eachAlmanac.input_max, seed.seed_max);

						SeedRange seedInf { seed.seed_min, outputRange.seed_min - 1 };
						SeedRange seedUp { outputRange.seed_max + 1, seed.seed_max };

						if (seedInf.seed_min <= seedInf.seed_max)
						{
							seedToProcess.push_back(seedInf);
						}
						if (seedUp.seed_min <= seedUp.seed_max)
						{
							seedToProcess.push_back(seedUp);
						}

						// Updating with shift
						outputRange.seed_min += eachAlmanac.shift;
						outputRange.seed_max += eachAlmanac.shift;
						outputRanges.push_back(outputRange);

						wasSeedUpdated = true;
						break;
					}
				}
			}

			for (const auto& seed : seedToProcess)
			{
				outputRanges.push_back(seed);
			}
		}
		cleanifySeedRanges(outputRanges);
		return outputRanges;
	};

	SeedRange::Vect result = std::accumulate(p_almanac.cbegin(), p_almanac.cend(), p_seedRanges, accumulateLambda);

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

	for (long long i = 0; i < 7; i++)
	{
		if (!std::getline(input_file, line))
		{
			break;
		}

		std::vector<AlmanacConversion> almanac_map;
		while (std::getline(input_file, line) && line != "")
		{
			auto      almanac_elem = TextUtils::Split(line, " ");
			long long output       = std::stoul(almanac_elem[0]);
			long long input        = std::stoul(almanac_elem[1]);
			long long range        = std::stoul(almanac_elem[2]);

			almanac_map.push_back({ input, input + range - 1, output - input });
		}
		almanac.push_back(almanac_map);
	}

	std::cout << "All parsed, now computing solution" << std::endl;

	// Solve Part1
	long long resultPart1 = std::numeric_limits<long long>::max();
	for (int i = 1; i < seedsString.size(); i++)
	{
		long long seed     = std::stoul(seedsString[i]);
		long long location = seed;
		for (auto almanac_conversions : almanac)
		{
			auto it = std::find_if(almanac_conversions.cbegin(),
			                       almanac_conversions.cend(),
			                       [location](const AlmanacConversion& alma_elem)
			                       {
				                       return (location >= alma_elem.input_min) && (location <= alma_elem.input_max);
			                       });

			if (it != almanac_conversions.cend())
			{
				location += it->shift;
			}
		}
		resultPart1 = std::min(resultPart1, location);
	}

	std::cout << "Part 1: " << resultPart1 << std::endl;

	std::vector<SeedRange> seedRanges;
	std::vector<SeedRange> seedsRangePart1;

	long long resultPart2 = std::numeric_limits<long long>::max();

	// Create the seed ranges from the input
	for (int i = 1; i < (seedsString.size() - 1); i = i + 2)
	{
		SeedRange seedRange;
		seedRange.seed_min = std::stoul(seedsString[i]);
		seedRange.seed_max = seedRange.seed_min + (std::stoul(seedsString[i + 1]) - 1);
		seedRanges.push_back(seedRange);
	}

	std::cout << "Part 2: " << std::endl << applyAlmanac(almanac, seedRanges) << std::endl;

	return 0;
}
