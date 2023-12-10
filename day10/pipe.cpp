#include <cassert>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <optional>
#include <queue>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>

template <typename T>
T takeone(std::queue<T> &q) {
	auto to_pop = q.front();
	q.pop();
	return to_pop;
}

struct maze {
	static constexpr auto ROWS = 5;
	static constexpr auto COLS = 5;

	struct coordinate {
		char curr;
		std::uint64_t row;
		std::uint64_t col;
		std::uint64_t depth;

		std::vector<coordinate> children(const maze &mz) {
			std::vector<coordinate> children_;
			auto sym_at = [&](std::uint64_t r, std::uint64_t c) {
				return mz.maze[r].at(c);
			};

			switch (curr) {
				break; case 'L': // North-East
					children_.emplace_back(sym_at(row - 2, col), row - 2, col, depth + 2); // North
					children_.emplace_back(sym_at(row, col + 2), row, col + 2, depth + 2); // East
				break; case 'J': // North-West
					children_.emplace_back(sym_at(row - 2, col), row - 2, col, depth + 2); // North
					children_.emplace_back(sym_at(row, col - 2), row, col - 2, depth + 2); // West
				break; case '7': // South-West
					children_.emplace_back(sym_at(row + 2, col), row + 2, col, depth + 2); // South
					children_.emplace_back(sym_at(row, col - 2), row, col - 2, depth + 2); // West
				break; case 'F': // South-East
					children_.emplace_back(sym_at(row + 2, col), row + 2, col, depth + 2); // South
					children_.emplace_back(sym_at(row, col + 2), row, col + 2, depth + 2); // East
				break; case 'S':
					if (sym_at(row + 1, col) == '|')
						children_.emplace_back(sym_at(row + 2, col), row + 2, col, depth + 2);
					if (sym_at(row - 1, col) == '|')
						children_.emplace_back(sym_at(row - 2, col), row - 2, col, depth + 2);
					if (sym_at(row, col + 1) == '-')
						children_.emplace_back(sym_at(row, col + 2), row, col + 2, depth + 2);
					if (sym_at(row, col - 1) == '-')
						children_.emplace_back(sym_at(row, col - 2), row, col - 2, depth + 2);
				break; case '.': // nothing
				default:
					__builtin_unreachable();
			}

			assert(children_.size() == 2);
			return children_;
		}

	};

	friend std::istream &operator>>(std::istream &is, maze &m) {
		std::uint64_t i = 0;
		std::optional<coordinate> maybe_start;
		for (std::string line; std::getline(is, line); ++i) {
			assert(line.size() == COLS);
			m.maze.push_back(line);
			if (!maybe_start)
				if (const auto p = line.find('S'); p != std::string::npos)
					maybe_start.emplace('S', i, p, 0);
		}

		assert(m.maze.size() == ROWS);
		assert(maybe_start);
		m.start = maybe_start.value();
		return is;
	}

	std::uint64_t longest_path() {
		std::vector<bool> visited(ROWS * COLS, false);
		auto mark_visited = [&](const coordinate &c) {
			visited[c.col * ROWS + c.row] = true;
		};
		auto is_visited = [&](const coordinate &c) {
			return visited[c.col * ROWS + c.row];
		};

		std::queue<coordinate> wset;
		wset.push(start);
		mark_visited(start);

		std::uint64_t max_depth{0};
		while (!wset.empty()) {
			auto curr = takeone(wset);
			max_depth = std::max(curr.depth, max_depth);
			const auto &children = curr.children(*this);
			for (const auto &c : children) {
				if (is_visited(c))
					continue;
				wset.push(c);
				mark_visited(c);
			}
		}
		return max_depth;
	}

	coordinate start;
	std::vector<std::string> maze;
};

int main(int argc, const char* argv[])
{
	assert(argc == 2 && "No input file provided!");
	std::ifstream ifs { argv[1], std::ios::in };
	maze m;
	ifs >> m;
	const std::uint64_t l = m.longest_path();
	std::cout << l << '\n';
	return 0;
}
