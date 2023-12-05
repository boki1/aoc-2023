#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <mutex>
#include <ranges>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

static constexpr auto g_max_istream = std::numeric_limits<std::streamsize>::max();

struct entry {
	std::uint64_t dest_start;
	std::uint64_t src_start;
	std::uint64_t length;
};

struct seed_range {
	std::uint64_t start;
	std::uint64_t length;
};

std::vector<seed_range> read_seed_ranges(std::ifstream& ifs)
{
	std::vector<seed_range> seed_ranges;
	ifs.ignore(g_max_istream, ':');
	for (seed_range sr; ifs.peek() != '\n';) {
		ifs >> sr.start >> sr.length;
		seed_ranges.push_back(sr);
	}
	ifs.ignore(g_max_istream, '\n');
	return seed_ranges;
}

std::vector<std::vector<entry>> read_maps(std::ifstream& ifs)
{
	static constexpr auto maps_count = 7;
	std::vector<std::vector<entry>> maps;
	maps.reserve(maps_count);

	auto read_map = [&]() {
		std::string line;
		std::vector<entry> curr;
		ifs.ignore(g_max_istream, '\n');
		std::getline(ifs, line);
		assert(line.find("map") != std::string::npos);

		for (entry e;;) {
			ifs >> e.dest_start >> e.src_start >> e.length;
			curr.emplace_back(e);
			ifs.ignore(g_max_istream, '\n');
			if (ifs.eof() || !std::isdigit(ifs.peek()))
				break;
		}

		return curr;
	};

	for (int i = 0; i < maps_count; ++i) {
		maps.emplace_back(read_map());
		assert(!maps[i].empty());
		std::sort(std::begin(maps[i]), std::end(maps[i]), [](const entry& a, const entry& b) {
			return a.src_start < b.src_start;
		});
	}

	return maps;
}

std::uint64_t location_of_seed(std::uint64_t seed, const std::vector<std::vector<entry>>& maps)
{
	std::uint64_t acc = seed;
	for (const auto& curr : maps) {
		const auto it = std::find_if(std::cbegin(curr), std::cend(curr), [acc](const entry& en) {
			return en.src_start <= acc && acc < en.src_start + en.length;
		});
		if (it != std::cend(curr))
			acc += it->dest_start - it->src_start;
	}
	return acc;
}

int main(int argc, const char* argv[])
{
	assert(argc == 2 && "No input file provided!");
	std::ifstream ifs { argv[1], std::ios::in };
	const auto seed_ranges = read_seed_ranges(ifs);
	const auto maps = read_maps(ifs);

	std::vector<std::uint64_t> locations;
	std::vector<std::thread> ts;
	std::mutex locations_lock;
	for (const auto& sr : seed_ranges) {
		ts.emplace_back([&](const seed_range& sr) {
			std::uint64_t min_loc = -1;
			for (std::uint64_t seed = sr.start, end = sr.start + sr.length; seed < end; ++seed)
				if (const auto loc = location_of_seed(seed, maps); loc < min_loc)
					min_loc = loc;
			std::unique_lock<std::mutex> _ { locations_lock };
			locations.push_back(min_loc);
		},
			sr);
	}

	for (auto& t : ts)
		t.join();

	std::cout << *std::min_element(std::cbegin(locations), std::cend(locations)) << '\n';
	return 0;
}
