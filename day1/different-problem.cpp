/*
 * It seems that I have solved a different problem with this one.
 * However, I really liked the new fold_left function, so I will keep this :).
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string_view>
using namespace std::string_view_literals;

inline std::uint64_t concat_int(char a, char b) {
  const char cc[] = {a, b, '\0'};
  return std::atoi(cc);
}

std::string reduce(const std::string &str) {
  static const std::array digits_in_text{
      "zero"sv, "one"sv, "two"sv,   "three"sv, "four"sv,
      "five"sv, "six"sv, "seven"sv, "eight"sv, "nine"sv};

	return std::ranges::fold_left(str, std::string {}, [](std::string reduced, char c) {
			reduced.push_back(c);
			for (const auto& [digit, text] : std::views::enumerate(digits_in_text)) {
				if (reduced.ends_with(text)) {
					reduced.replace(reduced.size() - text.size(), text.size(), std::to_string(digit));
					return reduced;
				}
			}

		});
}

int main(int argc, const char *argv[]) {
  assert(argc == 2);
  std::ifstream ifs{argv[1], std::ios::in};
  std::uint64_t total{0};

  auto isdigit = [](const char s) { return std::isdigit(s); };

  for (std::string line; std::getline(ifs, line);) {
	std::cout << line << " -> ";
    const auto reduced = reduce(line);
	// std::cout << reduced << " -> ";
    const auto first_it= std::ranges::find_if(reduced, isdigit);
    const auto last_it = std::ranges::find_last_if(reduced, isdigit).begin();
	assert(first_it != std::cend(reduced) && last_it != std::cend(reduced));
    const auto curr = concat_int(*first_it, *last_it);
	std::cout << curr << '\n';
	total += curr;
  }

  // std::cout << "Total: " << total << '\n';
  return 0;
}
