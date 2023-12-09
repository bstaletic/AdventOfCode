#include <functional>
#include <iostream>
#include <string_view>
#include <array>
#include <algorithm>
#include <ranges>
using namespace std::literals;

constexpr char raw_input[] {
#include "input.h"
};

constexpr std::string_view input(raw_input, sizeof(raw_input) - 1); // Strip trailing new line.
template<size_t size>
constexpr auto input_to_lines(std::string_view input) {
	std::array<std::string_view, size> lines{};
	std::ranges::transform(std::views::split(input, "\n"sv), lines.begin(), [](auto&& l) { return std::string_view(l); });
	return lines;
}

constexpr auto is_digit = [](char c) { return '0' <= c && c <= '9'; };

struct map {
	std::string_view head;
	std::string_view left;
	std::string_view right;
};

template<typename T, size_t N>
constexpr auto parse_input_data(auto&& lines) {
	std::array<T, N> parsed_input;
	std::ranges::transform(lines, parsed_input.begin(), [](std::string_view l) {
		return map{
			.head = l.substr(0, 3),
			.left = l.substr(l.find('(') + 1, 3),
			.right= l.substr(l.rfind(' ') + 1, 3)
		};
	});
	std::ranges::sort(parsed_input, {}, &map::head);
	return parsed_input;
}

constexpr size_t needed_steps(auto&& input, std::string_view directions) {
	std::string_view current_head = "AAA"sv;
	size_t count = 0;
	auto direction_it = directions.begin();
	do {
		auto new_position = std::ranges::lower_bound(input, current_head, {}, &map::head);
		current_head = *direction_it == 'L' ? new_position->left : new_position->right;
		++direction_it;
		if(direction_it == directions.end()) direction_it = directions.begin();
		count++;
	} while(current_head != "ZZZ"sv);
	return count;
}

constexpr size_t needed_steps_p2(auto starting_positions, auto&& input, std::string_view directions) {
	size_t count = 0;
	auto direction_it = directions.begin();
	do {
		std::ranges::transform(starting_positions, starting_positions.begin(), [&input, &direction_it, directions](std::string_view current_head) mutable {
			auto new_position = std::ranges::lower_bound(input, current_head, {}, &map::head);
			current_head = *direction_it == 'L' ? new_position->left : new_position->right;
			return current_head;
		});
		count++;
		++direction_it;
		if(direction_it == directions.end()) direction_it = directions.begin();
	} while(std::ranges::any_of(starting_positions, [](std::string_view head) { return head.back() != 'Z'; }));
	return count;
}

template<size_t N>
constexpr auto get_starts(auto&& data) {
	std::array<std::string_view, N> arr{};
	std::ranges::transform(
		data
			| std::views::filter([](map m) { return m.head.back() == 'A'; })
			| std::views::transform(&map::head),
		arr.begin(),
		std::identity{}
	);
	return arr;
}

constexpr size_t number_of_lines = std::ranges::count(input, '\n') + 1;
constexpr auto input_lines = input_to_lines<number_of_lines>(input);
constexpr auto data = parse_input_data<map, number_of_lines - 2>(input_lines | std::views::drop(2));
//constexpr size_t part1 = needed_steps(data, input_lines[0]);
//size_t r1 = part1;
constexpr size_t number_of_starts = std::ranges::count_if(data, [](std::string_view head) { return head.back() == 'A'; }, &map::head);
constexpr auto starts = get_starts<number_of_starts>(data);
constexpr size_t part2 = needed_steps_p2(starts, data, input_lines[0]);
size_t r2 = part2;
int main(){ std::cout << r2; }
