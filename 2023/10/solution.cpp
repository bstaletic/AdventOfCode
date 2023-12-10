#include <string_view>
#include <array>
#include <algorithm>
#include <ranges>
#include <tuple>
#include <utility>
using namespace std::literals;

constexpr char raw_input[] {
#include "input.h"
};

constexpr std::string_view input(raw_input, sizeof(raw_input) - 1);

template<size_t size>
constexpr auto input_to_lines(std::string_view input) {
	std::array<std::string_view, size> lines{};
	std::ranges::transform(std::views::split(input, "\n"sv), lines.begin(), [](auto&& l) { return std::string_view(l); });
	return lines;
}

constexpr std::pair<size_t, size_t> find_start(auto&& grid) {
	size_t start_column = -1uz;
	auto line_it = std::ranges::find_if(grid, [&start_column](std::string_view line) {
		start_column = line.find('S');
		return start_column != std::string::npos;
	});
	return {line_it - grid.begin(), start_column};
}

struct grid_position {
	size_t row;
	size_t column;
	char incoming_direction;
	constexpr auto operator==(const grid_position& other) const {
		return std::tie(row, column) == std::tie(other.row, other.column);
	}
	constexpr auto operator<=>(const grid_position& other) const {
		return std::tie(row, column) <=> std::tie(other.row, other.column);
	}
};

constexpr auto update_position(auto&& grid, grid_position current_position) {
	switch(grid[current_position.row][current_position.column]) {
		case 'F':
			if(current_position.incoming_direction == 'E') {
				current_position.row += 1;
				current_position.incoming_direction = 'N';
			} else {
				current_position.column += 1;
				current_position.incoming_direction = 'W';
			}
			break;
		case '7':
			if(current_position.incoming_direction == 'W') {
				current_position.row += 1;
				current_position.incoming_direction = 'N';
			} else {
				current_position.column -= 1;
				current_position.incoming_direction = 'E';
			}
			break;
		case 'J':
			if(current_position.incoming_direction == 'W') {
				current_position.row -= 1;
				current_position.incoming_direction = 'S';
			} else {
				current_position.column -= 1;
				current_position.incoming_direction = 'E';
			}
			break;
		case 'L':
			if(current_position.incoming_direction == 'E') {
				current_position.row -= 1;
				current_position.incoming_direction = 'S';
			} else {
				current_position.column += 1;
				current_position.incoming_direction = 'W';
			}
			break;
		case '-':
			if(current_position.incoming_direction == 'E') {
				current_position.column -= 1;
			} else {
				current_position.column += 1;
			}
			break;
		case '|':
			if(current_position.incoming_direction == 'N') {
				current_position.row += 1;
			} else {
				current_position.row -= 1;
			}
			break;
	}
	return current_position;
}

constexpr std::array<grid_position, 2> get_start_connections(auto&& grid, std::pair<size_t, size_t> start_coordinates) {
	std::array<grid_position, 2> result{};
	auto result_it = result.begin();
	if(start_coordinates.second < grid[start_coordinates.first].size() - 1 && "J7-"sv.find(grid[start_coordinates.first][start_coordinates.second + 1]) != std::string_view::npos) {
		*result_it++ = grid_position{.row = start_coordinates.first, .column = start_coordinates.second + 1, .incoming_direction = 'W'};
	}
	if(start_coordinates.second > 0 && "FL-"sv.find(grid[start_coordinates.first][start_coordinates.second - 1]) != std::string_view::npos) {
		*result_it++ = grid_position{.row = start_coordinates.first, .column = start_coordinates.second - 1, .incoming_direction = 'E'};
	}
	if(start_coordinates.first > 0 && "|7F"sv.find(grid[start_coordinates.first - 1][start_coordinates.second]) != std::string_view::npos) {
		*result_it++ = grid_position{.row = start_coordinates.first - 1, .column = start_coordinates.second, .incoming_direction = 'S'};
	}
	if(start_coordinates.first < grid.size() - 1 && "JL|"sv.find(grid[start_coordinates.first + 1][start_coordinates.second]) != std::string_view::npos) {
		*result_it++ = grid_position{.row = start_coordinates.first + 1, .column = start_coordinates.second, .incoming_direction = 'N'};
	}
	return result;
}

constexpr auto walk_loop(auto&& grid) {
	std::pair<size_t, size_t> start_coordinates = find_start(grid);
	auto [previous_coordinates, next_coordinates] = get_start_connections(grid, start_coordinates);

	size_t steps = 1;

	do {
		previous_coordinates = update_position(grid, previous_coordinates);
		next_coordinates = update_position(grid, next_coordinates);
		steps++;
	} while(next_coordinates != previous_coordinates);
	return steps;
}

constexpr char starting_pipe(auto&& grid) {
	auto start = find_start(grid);
	auto [previous_coordinates, next_coordinates] = get_start_connections(grid, start);
	switch(previous_coordinates.incoming_direction) {
		case 'W':
			return next_coordinates.incoming_direction == 'E' ? '-' :
				next_coordinates.incoming_direction == 'S' ? 'L' :
										'F';
		case 'E':
			return next_coordinates.incoming_direction == 'S' ? 'J' : '7';
		case 'S':
			return '|';
	}
}

constexpr size_t number_of_lines = std::ranges::count(input, '\n') + 1;
constexpr auto input_lines = input_to_lines<number_of_lines>(input);
constexpr size_t part1 = walk_loop(input_lines);
