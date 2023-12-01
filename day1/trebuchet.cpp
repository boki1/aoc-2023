#include <algorithm>
#include <array>
#include <utility>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string_view>
#include <list>
using namespace std::string_view_literals;

inline std::uint64_t concat_int(char a, char b) {
  const char cc[] = {a, b, '\0'};
  return std::atoi(cc);
}

template <bool IsReversed, typename It>
uint64_t lrmost(It begin, It end) {
   static const std::array digits_in_text{
       "zero"sv, "one"sv, "two"sv,   "three"sv, "four"sv,
       "five"sv, "six"sv, "seven"sv, "eight"sv, "nine"sv};

    std::string acc;
	for ( ; begin != end; ++begin) {
		if (std::isdigit(*begin))
			return *begin - '0';
		if constexpr (IsReversed)	acc.insert(acc.begin(), *begin);
		else acc.insert(acc.end(), *begin);
		for (const auto& [digit, text] : std::views::enumerate(digits_in_text))
			if (acc.contains(text))
				return digit;
	}

	assert(!"Input contains bad line!");
	std::unreachable();
}

int main(int argc, const char *argv[]) {
  assert(argc == 2 && "No input file provided!");
  std::ifstream ifs{argv[1], std::ios::in};
  std::uint64_t total{0};

  for (std::string line; std::getline(ifs, line);) {
	const std::uint64_t left = lrmost<false>(std::cbegin(line), std::cend(line));
	const std::uint64_t right = lrmost<true>(std::crbegin(line), std::crend(line));
	total += left * 10 + right;
  }

  std::cout << "Total: " << total << '\n';
  return 0;
}
