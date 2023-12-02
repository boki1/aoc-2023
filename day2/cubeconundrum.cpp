#include <cassert>
#include <cstdint>
#include <algorithm>
#include <tuple>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <limits>

static constexpr auto red_limit{12};
static constexpr auto green_limit{13};
static constexpr auto blue_limit{14};

bool check_game(const std::string& line, std::uint64_t& out_identifier, std::uint64_t &out_power) {
	std::istringstream iss{line};
	static constexpr auto max_size = std::numeric_limits<std::streamsize>::max();
	iss.ignore(max_size,' ');
	iss >> out_identifier;
	iss.ignore(max_size, ' ');

	auto elf_get_next= [&]() -> std::tuple<int, int, int> {
		std::uint64_t value;
		std::uint64_t r{0}, g{0}, b{0};

		for (std::string color; !iss.eof(); iss.ignore(max_size, ' ')) {
			iss >> value;
			iss >> color;

			if (color.starts_with("red") && value > r)
				r = value;
			else if (color.starts_with("green") && value > g)
				g = value;
			else if (color.starts_with("blue") && value > b)
				b = value;

			if (color.back() == ';')
				break;
		}

		return std::make_tuple(r, g, b);
	};

	std::uint64_t mr{0}, mg{0}, mb{0};
	bool valid = true;
	for ( ; !iss.eof(); ) {
		auto [r, g, b] = elf_get_next();

		if (r > mr) mr = r;
		if (g > mg) mg = g;
		if (b > mb) mb = b;

		if (r > red_limit
		 || g > green_limit
		 || b > blue_limit)
			valid = false;
		iss.ignore(max_size, ' ');
	}

	out_power = mr * mg * mb;
	return valid;
}

int main(int argc, const char* argv[])
{
	assert(argc == 2 && "No input file provided!");
	std::ifstream ifs { argv[1], std::ios::in };
	std::uint64_t sum { 0 };
	for (std::string line; std::getline(ifs, line);) {
		std::uint64_t identifier;
		std::uint64_t power;
		(void) check_game(line, identifier, power);
		std::cout << "Game " << identifier << " has power " << power << '\n';
		sum += power;
	}

	std::cout << "Sum = " << sum << '\n';

	return 0;
}
