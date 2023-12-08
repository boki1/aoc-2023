#include <algorithm>
#include <cassert>
#include <sstream>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <ranges>
#include <unordered_map>
#include <vector>

struct competition {
	const std::uint64_t duration;
	const std::uint64_t record;

	std::uint64_t count_winning_strategies() const
	{
		std::uint64_t winning_strategies{0};
		for (std::uint64_t i = 1; i < duration; ++i)
			if ((duration - i) * i > record)
				++winning_strategies;
		return winning_strategies;
	}
};

competition parse(std::ifstream& ifs)
{
	static constexpr auto max_istream = std::numeric_limits<std::streamsize>::max();
	auto read_number_skipws = [](std::ifstream &ifs) {
		std::string line;
		if (!std::getline(ifs, line))
			throw std::invalid_argument{"bad line"};
		std::istringstream iss{line};
		iss.ignore(max_istream, ':');
		std::uint64_t number{0};
		while (!iss.eof() && iss.peek() > 0) {
			if (std::isspace(iss.peek())) {
				iss.get();
				continue;
			}

			std::uint64_t digit = iss.get() - '0';
			number = number * 10 + digit;
		}

		return number;
	};

	const auto duration = read_number_skipws(ifs);
	const auto record = read_number_skipws(ifs);
	return competition {
		.duration = duration,
		.record = record
	};
}

int main(int argc, const char* argv[])
{
	assert(argc == 2 && "No input file provided!");
	std::ifstream ifs { argv[1], std::ios::in };
	const auto competition = parse(ifs);
	std::cout << competition.count_winning_strategies() << '\n';
	return 0;
}
