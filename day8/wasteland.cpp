#include <algorithm>
#include <cassert>
#include <numeric>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <ranges>
#include <unordered_map>
#include <vector>

struct navigation_map {
	enum class step : std::uint8_t { Left,
		Right };

	friend std::istream& operator>>(std::istream& is, navigation_map& nmap)
	{
		std::string line;
		std::getline(is, line);
		nmap.steps.reserve(line.size());
		for (const char c : line) {
			assert(c == 'L' || c == 'R');
			nmap.steps.push_back(c == 'L' ? step::Left : step::Right);
		}

		static constexpr auto max_istream = std::numeric_limits<std::streamsize>::max();
		is.ignore(max_istream, '\n');
		for (std::string key, leftturn, rightturn; !is.eof() && is.peek() != -1;) {
			is >> key;
			if (key.back() == 'A')
				nmap.srcs.push_back(key);
			is.ignore(max_istream, '(');
			is >> leftturn;
			assert(leftturn.back() == ',');
			leftturn.pop_back();
			is >> rightturn;
			assert(rightturn.back() == ')');
			rightturn.pop_back();
			nmap.turnpoints[std::move(key)] = std::make_pair(std::move(leftturn), std::move(rightturn));
			is.ignore(max_istream, '\n');
		}

		return is;
	}

	const std::string* follow_step_from(const std::string& src, const step st)
	{
		assert(turnpoints.find(src) != turnpoints.cend());
		const auto& turns = turnpoints[src];
		return st == step::Left ? &turns.first : &turns.second;
	}

	std::uint64_t follow_steps()
	{
		std::uint64_t lcm_count{1};

		std::vector<const std::string*> dests;
		dests.reserve(srcs.size());
		std::transform(std::cbegin(srcs), std::cend(srcs), std::back_inserter(dests), [](const std::string& s) { return &s; });

		for (std::uint64_t step_index { 0 }, count{0}; !dests.empty(); ++step_index, ++count) {
			if (step_index >= steps.size())
				step_index = 0;
			for (auto it = dests.begin(); it != dests.end(); ) {
				*it = follow_step_from(**it, steps[step_index]);
				if ((*it)->back() == 'Z') {
					lcm_count = std::lcm(lcm_count, count + 1);
					dests.erase(it);
				} else ++it;
			}
		}

		return lcm_count;
	}

	std::vector<step> steps;
	std::unordered_map<std::string, std::pair<std::string, std::string>> turnpoints;
	std::vector<std::string> srcs;
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
