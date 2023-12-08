#include <cassert>
#include <fstream>
#include <cstdint>
#include <limits>
#include <iostream>
#include <unordered_map>
#include <vector>

struct navigation_map {
	enum class step : std::uint8_t { Left, Right };

	friend std::istream &operator>>(std::istream &is, navigation_map &nmap) {
		std::string line;
		std::getline(is, line);
		nmap.steps.reserve(line.size());
		for (const char c : line) {
			assert(c == 'L' || c == 'R');
			nmap.steps.push_back(c == 'L' ? step::Left : step::Right);
		}

		static constexpr auto max_istream = std::numeric_limits<std::streamsize>::max();
		is.ignore(max_istream, '\n');
		for (std::string key, leftturn, rightturn; !is.eof() && is.peek() != -1; ) {
			is >> key;
			is.ignore(max_istream, '(');
			is >> leftturn;
			assert(leftturn.back() == ',');
			leftturn.pop_back();
			is >> rightturn;
			assert(rightturn.back() == ')');
			rightturn.pop_back();
			nmap.turnpoints[key] = std::make_pair(leftturn, rightturn);
			is.ignore(max_istream, '\n');
		}

		return is;
	}

	std::uint64_t follow_steps() {
		const std::string *curr{&src};
		for (std::uint64_t total = 0;;) {
			for (const step s : steps) {
				if (*curr == dest)
					return total;
				assert(turnpoints.find(*curr) != turnpoints.cend());
				const auto &turns = turnpoints[*curr];
				curr = s == step::Left ? &turns.first : &turns.second;
				++total;
			}
		}
	}

	std::vector<step> steps;
	std::unordered_map<std::string, std::pair<std::string, std::string>> turnpoints;
	const std::string dest{"ZZZ"};
	const std::string src{"AAA"};
};

int main(int argc, const char* argv[])
{
	assert(argc == 2 && "No input file provided!");
	std::ifstream ifs { argv[1], std::ios::in };
	navigation_map nmap;
	ifs >> nmap;
	const std::uint64_t total_steps = nmap.follow_steps();
	std::cout << total_steps << '\n';
	return 0;
}
