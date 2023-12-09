#include <cassert>
#include <cstdint>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

std::vector<std::uint64_t> parse_line(const std::string &line)
{
	std::istringstream iss{line};
	std::istream_iterator<std::uint64_t> it{iss};
	std::vector<std::uint64_t> vec;
	std::copy(it, decltype(it){}, std::back_inserter(vec));
	return vec;
}

std::uint64_t extrapolate_next_value(std::vector<std::uint64_t>& progress)
{
	for (auto limit = std::prev(std::end(progress)); std::begin(progress) != limit; limit = std::prev(limit)) {
		if (std::all_of(std::begin(progress), limit, [](std::uint64_t n) { return !n; }))
			return std::accumulate(limit, std::end(progress), 0);
		for (auto it = std::begin(progress); it != limit; ++it)
			*it = *std::next(it) - *it;
	}

	assert(!"unreachable");
}

int main(int argc, const char* argv[])
{
	assert(argc == 2 && "No input file provided!");
	std::ifstream ifs { argv[1], std::ios::in };
	std::uint64_t total{0};
	for (std::string line; std::getline(ifs, line); ) {
		auto value_progress = parse_line(line);
		total += extrapolate_next_value(value_progress);
	}
	std::cout << total << '\n';
	return 0;
}
