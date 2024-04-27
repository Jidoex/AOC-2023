#include "TextUtils.hpp"

#include <array>
#include <map>
#include <numeric>

#define EnablePart2

struct Card
{
	explicit operator int() const
	{
		if (('2' <= val) && (val <= '9'))
		{
			return val - '0';
		}
		switch (val)
		{
		case 'T':
			return 10;
			break;
		case 'J':
#ifdef EnablePart2
			return 1;
#else
			return 11;
#endif
		case 'Q':
			return 12;
		case 'K':
			return 13;
		case 'A':
			return 14;
		default:
			std::cerr << "Type of card undefined " << val << std::endl;
			return -1;
		}
	}

	bool operator<(const Card& p_other)
	{
		return static_cast<int>(*this) < static_cast<int>(p_other);
	}

	char val;
};

bool operator<(const Card& p_first, const Card& p_sec)
{
	return static_cast<int>(p_first) < static_cast<int>(p_sec);
}

struct Hand : public std::array<Card, 5>
{
	Hand(const Hand& p_other)
	{
		*this = p_other;
	}

	Hand(const std::string& p_input)
	{
		for (int i = 0; i < size(); i++)
		{
			at(i) = { p_input.at(i) };
		}
	}
};

bool operator<(const Hand& p_first, const Hand& p_sec)
{
	for (int i = 0; i < p_first.size(); i++)
	{
		int firstVal { p_first.at(i) };
		int secVal { p_sec.at(i) };
		if (firstVal != secVal)
		{
			return (firstVal < secVal);
		}
	}
	return false;
}

std::ostream& operator<<(std::ostream& p_stream, const Hand& p_hand)
{
	for (auto card : p_hand)
	{
		p_stream << card.val << "";
	}

	return p_stream;
}

using HandBetPair = std::pair<Hand, long>;

double score(HandBetPair p_hand)
{
	std::map<Card, double> countChar;

	for (Card c : p_hand.first)
	{
		countChar[c] = countChar[c] + 1;
	}

	std::vector<int> maxCounts;
	int              jokerVal = 0;
	for (auto count : countChar)
	{
#ifdef EnablePart2
		if (count.first.val == 'J')
		{
			jokerVal = count.second;
			continue;
		}
#endif
		maxCounts.push_back(count.second);
	}
	std::sort(maxCounts.begin(), maxCounts.end(), std::greater<>());

	if ((maxCounts.size() > 1) && (maxCounts.at(1) == 2))
	{
		return maxCounts.front() + jokerVal + 0.5;
	}
	return (maxCounts.size() > 0 ? maxCounts.front() : 0) + jokerVal;
}

bool operator<(const HandBetPair& p_firstHand, const HandBetPair& p_secondHand)
{
	double firstScore = score(p_firstHand);
	double secScore   = score(p_secondHand);

	if (firstScore != secScore)
	{
		return firstScore < secScore;
	}
	return p_firstHand.first < p_secondHand.first;
}

int main()
{
	std::fstream input_file;
	input_file.open("example7.txt", std::ios::in);

	std::cout << std::boolalpha;
	std::cout << "Day 7, lets go!" << std::endl;

	// Get times and distances
	std::vector<HandBetPair> handAndBetPairs;

	std::string currentLine;
	while (std::getline(input_file, currentLine))
	{
		auto                  splitRes = TextUtils::Split(currentLine, " ");
		std::pair<Hand, long> pair     = { splitRes.at(0), std::stol(splitRes.at(1)) };

		handAndBetPairs.push_back(pair);
	}

	std::sort(handAndBetPairs.begin(), handAndBetPairs.end());

	long score = 0;
	for (long i = 0; i < handAndBetPairs.size(); i++)
	{
		score += (i + 1) * handAndBetPairs.at(i).second;
	}

	std::cout << "Score is " << score;

	return 0;
}