#include <iostream>
#include <fstream>
#include <limits>
#include <numeric>
#include <ranges>
#include <vector>
#include <cassert>
#include <string>
#include <cstdint>
#include <set>
#include <sstream>

std::uint64_t count_matches(const std::string &game) {
	static constexpr auto issmax = std::numeric_limits<std::streamsize>::max();
	std::uint64_t amount = 0;
	std::set<std::uint64_t> winning_numbers;
	std::istringstream iss{game};
	iss.ignore(issmax, ':');
	for (std::uint64_t wn; iss.peek() != '|'; iss.ignore(issmax, ' ')) {
		iss >> wn;
		winning_numbers.insert(wn);
	}
	iss.ignore(issmax, ' ');
	for (std::uint64_t wn; !iss.eof() && iss.peek() != -1; iss.ignore(issmax, ' ')) {
		iss >> wn;
		if (winning_numbers.find(wn) != std::cend(winning_numbers))
			++amount;
	}

	return amount;
}

std::uint64_t mark(const std::string &game, std::uint64_t card_curr, std::vector<std::uint64_t> &acc) {
	const auto matches = count_matches(game);
	++acc[card_curr];
	for (std::uint64_t card_won = card_curr + 1;
			card_won <= card_curr + matches;
			++card_won)
		acc[card_won] += acc[card_curr];
	return acc[card_curr];
}

int main(int argc, const char* argv[])
{
	assert(argc == 2 && "No input file provided!");
	std::ifstream ifs{ argv[1], std::ios::in };
	std::uint64_t sum{0}, game_i{0};
	std::vector<std::uint64_t> cards(196, 0);
	for (std::string game; std::getline(ifs, game); ++game_i)
		sum += mark(game, game_i, cards);
	std::cout << "Total cards: " << sum << '\n';
	return 0;
}
