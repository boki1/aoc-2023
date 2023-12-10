#include <algorithm>  // for max, copy, fill_n
#include <cassert>    // for assert
#include <fstream> 	  // for ifstream
#include <cstdint>    // for uint64_t, int64_t
#include <cstdlib>    // for labs
#include <iostream>   // for operator<<, cout, basic_istream, istream, ifstream
#include <optional>   // for optional, make_optional
#include <queue>      // for queue
#include <string>     // for basic_string, getline, string
#include <utility>    // for swap
#include <vector>     // for vector

template <typename T>
T takeone(std::queue<T>& q)
{
	auto to_pop = q.front();
	q.pop();
	return to_pop;
}

struct maze {
	static constexpr auto ROWS = 140;
	static constexpr auto COLS = 140;

	struct coordinate {
		char curr;
		std::int64_t row;
		std::int64_t col;
		std::uint64_t depth;

		std::vector<coordinate> children(const maze& mz)
		{
			std::vector<coordinate> children_;
			auto sym_at = [&](std::uint64_t r, std::uint64_t c) {
				return mz.maze[r].at(c);
			};

			auto find_vertical = [&](int step) -> std::optional<coordinate> {
				std::int64_t r = row + step;
				for (; r > 0 && r <= ROWS && sym_at(r, col) == '|'; r += step)
					;
				if (r < 0 || r >= ROWS || sym_at(r, col) == '.')
					return {};
				return coordinate { sym_at(r, col), r, col, depth + std::labs((r - row)) };
			};

			auto find_horizontal = [&](int step) -> std::optional<coordinate> {
				std::int64_t c = col + step;
				for (; c > 0 && c <= COLS && sym_at(row, c) == '-'; c += step)
					;
				if (c < 0 || c >= COLS || sym_at(row, c) == '.')
					return {};
				return std::make_optional(coordinate { sym_at(row, c), row, c, depth + std::labs((c - col)) });
			};

			if (curr == 'L' || curr == 'J' || curr == 'S')
				if (auto v = find_vertical(-1); v)
					children_.push_back(*v);
			if (curr == '7' || curr == 'F' || curr == 'S')
				if (auto v = find_vertical(+1); v)
					children_.push_back(*v);
			if (curr == 'J' || curr == '7' || curr == 'S')
				if (auto v = find_horizontal(-1); v)
					children_.push_back(*v);
			if (curr == 'L' || curr == 'F' || curr == 'S')
				if (auto v = find_horizontal(+1); v)
					children_.push_back(*v);

			if (children_.size() == 2 && children_[0].depth > children_[1].depth)
				std::swap(children_[0], children_[1]);
			return children_;
		}
	};

	friend std::istream& operator>>(std::istream& is, maze& m)
	{
		std::int64_t i = 0;
		std::optional<coordinate> maybe_start;
		for (std::string line; std::getline(is, line); ++i) {
			assert(line.size() == COLS);
			m.maze.push_back(line);
			if (!maybe_start)
				if (const std::int64_t p = line.find('S'); p != std::string::npos)
					maybe_start.emplace(coordinate{'S', i, p, 0});
		}

		assert(m.maze.size() == ROWS);
		assert(maybe_start);
		m.start = maybe_start.value();
		return is;
	}

	std::uint64_t longest_path()
	{
		std::vector<bool> visited(ROWS * COLS, false);
		auto mark_visited = [&](const coordinate& c) {
			visited[c.row * COLS + c.col] = true;
		};
		auto is_visited = [&](const coordinate& c) {
			return visited[c.row * COLS + c.col];
		};
		auto show_visited = [&]() {
			for (std::uint64_t j = 0; j < ROWS; ++j) {
				for (std::uint64_t i = 0; i < COLS; ++i)
					std::cout << visited[j * COLS + i];
				std::cout << '\n';
			}
			std::cout << '\n';
		};

		std::queue<coordinate> wset;
		wset.push(start);

		std::uint64_t max_depth { 0 };
		for (coordinate curr; !wset.empty(); ) {
			curr = takeone(wset);
			if (is_visited(curr))
				continue;
			mark_visited(curr);
			// show_visited();
			max_depth = std::max(curr.depth, max_depth);
			const auto& children = curr.children(*this);
			for (const auto& c : children)
				if (!is_visited(c))
					wset.push(c);
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
