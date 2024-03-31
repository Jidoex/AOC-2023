#include "TextUtils.hpp"

struct Race
{
	double duration;
	double distance;
};

int main()
{
	std::fstream input_file;
	input_file.open("example6.txt", std::ios::in);

	std::cout << std::boolalpha;
	std::cout << "Day 6, lets go!" << std::endl;

	std::string timeLine;
	std::string distanceLine;

	// Get times and distances
	std::getline(input_file, timeLine);
	std::getline(input_file, distanceLine);

	std::vector<std::string> timesStr     = TextUtils::Split(timeLine, " ");
	std::vector<std::string> distancesStr = TextUtils::Split(distanceLine, " ");
	std::vector<Race>        racesVect;

	// Remove empty elements
	timesStr.erase(std::remove_if(timesStr.begin(),
	                              timesStr.end(),
	                              [](const std::string& p_str)
	                              {
		                              return p_str.empty() || p_str == "";
	                              }),
	               timesStr.end());

	distancesStr.erase(std::remove_if(distancesStr.begin(),
	                                  distancesStr.end(),
	                                  [](const std::string& p_str)
	                                  {
		                                  return p_str.empty() || p_str == "";
	                                  }),
	                   distancesStr.end());

	if (timesStr.size() != distancesStr.size())
	{
		std::cerr << "Time and distance vectors should have the same size" << std::endl;
		return -1;
	}

	for (int i = 1; i < timesStr.size(); i++)
	{
		racesVect.push_back({ std::stod(timesStr.at(i)), std::stod(distancesStr.at(i)) });
	}

	// The product of solutions for each race
	long productOfSolutions = 1;

	for (const Race& race : racesVect)
	{

		// We want  d_boat > best_distance
		//          (v_boat * t_boat) > best_distance
		//          ((t_pressed)_v * t_total-t_pressed) > best_distance
		//          - t_pressed^2 + (t_total * t_pressed) - best_distance > 0
		//          -X^2 + t_total * X - best_distance = 0

		long   currNumberOfSolutions;
		double squaredTotalTime = std::pow(race.duration, 2);
		double delta            = squaredTotalTime - 4 * race.distance;

		if (delta < 0)
		{
			return -1;
		}

		// Root solutions
		double minPressedTimeSol = (race.duration - std::sqrt(delta)) / 2.0;
		double maxPressedTimeSol = (race.duration + std::sqrt(delta)) / 2.0;

		std::cout << "Root solutions " << minPressedTimeSol << " and " << maxPressedTimeSol << std::endl;

		// We want to count the integers between the root solutions
		currNumberOfSolutions = std::ceil(maxPressedTimeSol) - std::floor(minPressedTimeSol) - 1;
		productOfSolutions *= currNumberOfSolutions;
	}

	std::cout << std::endl << "Production of solutions " << productOfSolutions << std::endl;

	return 0;
}