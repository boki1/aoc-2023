#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <cstdint>
#include <unordered_set>
#include <sstream>

std::uint64_t eval_points_of(const std::string &game) {
	std::uint64_t amount = 0;
	std::unordered_set<std::uint64_t> winning_numbers;
	std::istringstream iss{game};
	iss.ignore(game.size(), ':');
	for (std::uint64_t wn; iss.peek() != '|'; iss.ignore(game.size(), ' ')) {
		iss >> wn;
		winning_numbers.insert(wn);
	}
	iss.ignore(game.size(), ' ');
	for (std::uint64_t wn; !iss.eof() && iss.peek() != -1; iss.ignore(game.size(), ' ')) {
		iss >> wn;
		if (winning_numbers.find(wn) != std::cend(winning_numbers))
			++amount;
	}

	return amount
		? 1 << (amount - 1)
		: 0;
}

int main(int argc, const char* argv[])
{
	assert(argc == 2 && "No input file provided!");
	std::ifstream ifs{ argv[1], std::ios::in };
	std::uint64_t sum{0}, game_i{0};
	for (std::string game; std::getline(ifs, game); ) {
		const auto points = eval_points_of(game);
		std::cout << "Game " << game_i++ << " makes " << points << '\n';
		sum += points;
	}
	std::cout << "Total points: " << sum << '\n';
	return 0;
}
