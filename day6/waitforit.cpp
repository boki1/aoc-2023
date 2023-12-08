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

std::vector<competition> parse(std::ifstream& ifs)
{
	static constexpr auto max_istream = std::numeric_limits<std::streamsize>::max();
	auto read_number_line = [](std::ifstream &ifs) {
		std::string line;
		if (!std::getline(ifs, line))
			throw std::invalid_argument{"bad line"};
		std::istringstream iss{line};
		iss.ignore(max_istream, ':');
		std::vector<std::uint64_t> vec;
		std::istream_iterator<std::uint64_t> it { iss };
		std::copy(it, decltype(it){}, std::back_inserter(vec));
		return vec;
	};

	const auto durations = read_number_line(ifs);
	const auto records = read_number_line(ifs);
	std::vector<competition> competitions;
	competitions.reserve(durations.size());
	for (const auto [d, r] : std::views::zip(durations, records))
		competitions.emplace_back(d, r);
	return competitions;
}

int main(int argc, const char* argv[])
{
	assert(argc == 2 && "No input file provided!");
	std::ifstream ifs { argv[1], std::ios::in };
	const auto competitions = parse(ifs);
	std::cout << std::accumulate(std::cbegin(competitions), std::cend(competitions), 1, [&](std::uint64_t acc, const competition& comp) {
		return acc * comp.count_winning_strategies();
	}) << '\n';
	return 0;
}
