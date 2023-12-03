#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <ranges>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class engine_map {

	struct part {
		std::int64_t number;
		std::uint64_t lineoff;
		std::uint32_t number_length;
	};

	void parse_line(const std::string& line, std::uint64_t current_linenum)
	{
		std::vector<part> line_parts;
		std::vector<std::uint64_t> line_symbols;

		std::uint64_t lineoff = 0;
		for (std::istringstream iss { line }; !iss.eof() && iss.peek() > 0;) {
			if (iss.peek() == '.') {
				++lineoff;
				iss.get();
				continue;
			}

			if (!std::isalnum(iss.peek())) {
				if (char c = iss.get(); c == '*')
					line_symbols.push_back(lineoff);
				++lineoff;
				continue;
			}

			if (std::isdigit(iss.peek())) {
				std::uint64_t number;
				iss >> number;
				line_parts.emplace_back(number, lineoff, std::to_string(number).size());
				lineoff += line_parts.back().number_length;
			}
		}

		m_parts.emplace_back(std::move(line_parts));
		assert(std::ranges::is_sorted(line_symbols));
		m_symbols.emplace_back(std::move(line_symbols));
	}

public:
	explicit engine_map(const char* fname)
	{
		std::ifstream ifs { fname, std::ios::in };
		std::uint64_t current_linenum { 0 };
		for (std::string line; std::getline(ifs, line); ++current_linenum)
			parse_line(line, current_linenum);
		assert(m_symbols.size() == m_parts.size());
	}

	std::int64_t sum_gear_ratios() {
		std::uint64_t sum{0};

		auto nearby_parts = [&](std::uint64_t star_index, std::uint64_t linenum) {
			std::vector<std::uint64_t> partnumbers;
			for (int i = -1; i <= 1; ++i) {
				if (linenum + i < 0) continue;
				if (linenum + i >= m_symbols.size()) break;
				for (const auto &part : m_parts[linenum + i])
					if (part.lineoff <= star_index + 1
					 && star_index <= part.lineoff + part.number_length)
						partnumbers.push_back(part.number);
			}
			return partnumbers;
		};

		for (std::uint64_t linenum = 0; linenum < m_symbols.size(); ++linenum)
			for (const auto &star_index : m_symbols[linenum]) {
				auto nearby_numbers = nearby_parts(star_index, linenum);
				if (nearby_numbers.size() == 2)
					sum += nearby_numbers[0] * nearby_numbers[1];
			}

		return sum;
	}

	std::int64_t sum_parts() {
		std::uint64_t sum{0};

		auto check_part_on_line = [&](const part &part, std::uint64_t linenum) -> bool {
			const auto &syms = m_symbols[linenum];
			if (syms.empty())
				return false;
			auto lower = std::ranges::lower_bound(syms, part.lineoff);
			if (lower != std::cend(syms)
			 && std::abs(static_cast<std::int64_t>(*lower - part.lineoff)) <= part.number_length)
				return true;
			else if (lower != std::cbegin(syms)
			 && *--lower >= part.lineoff - 1)
				return true;
			return false;
		};

		for (std::uint64_t linenum = 0; linenum < m_symbols.size(); ++linenum)
			for (const auto &part : m_parts[linenum])
				if (check_part_on_line(part, linenum)
				 || (linenum > 0 && check_part_on_line(part, linenum - 1))
				 || (linenum < m_symbols.size() && check_part_on_line(part, linenum + 1))) {
					std::cout << "taking " << part.number << '\n';
					sum += part.number;
				}

		return sum;
	}

private:
	std::vector<std::vector<part>> m_parts;
	std::vector<std::vector<std::uint64_t>> m_symbols;
};

int main(int argc, const char* argv[])
{
	assert(argc == 2 && "No input file provided!");
	engine_map em { argv[1] };
	// std::cout << em.sum_parts() << '\n';
	std::cout << em.sum_gear_ratios() << '\n';
	return 0;
}
