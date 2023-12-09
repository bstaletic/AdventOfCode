#include <charconv>
#include <optional>
#include <string_view>
#include <array>
#include <algorithm>
#include <ranges>
#include <utility>
using namespace std::literals;

constexpr char raw_input[] {
#include "input.h"
};

constexpr std::string_view input(raw_input, sizeof(raw_input) - 1); // Strip trailing new line.
template<size_t size>
constexpr auto input_to_lines(std::string_view input) {
	std::array<std::string_view, size> lines{};
	std::ranges::copy(input | std::views::split("\n"sv) | std::views::transform([](auto&& l) { return std::string_view(l); }), lines.begin());
	return lines;
}

constexpr auto is_digit = [](char c) { return '0' <= c && c <= '9'; };

constexpr size_t row_part_numbers(std::optional<std::string_view> previous_opt, std::string_view current, std::optional<std::string_view> next_opt) {
	size_t result = 0uz;
	for(auto it = std::ranges::find_if(current, is_digit); it != current.end(); it = std::ranges::find_if(it, current.end(), is_digit)) {
		size_t n = 0uz;
		auto adjacent_begin_index = std::max(0z, it - current.begin() - 1);
		auto old_it = it;
		it = std::from_chars(it, current.end(), n).ptr;
		if(old_it != current.begin() && *(old_it - 1) != '.') {
			result += n;
			continue;
		}
		if(it != current.end() && *it != '.') {
			result += n;
			continue;
		}
		auto adjacent_end_index = it - current.begin() + 1;

		auto has_part_symbol = [adjacent_begin_index, adjacent_end_index](std::string_view str) {
			auto first = str.begin() + adjacent_begin_index;
			auto last = std::ranges::next(str.begin(), adjacent_end_index, str.end());
			if(!std::ranges::all_of(first, last, [](char c) { return c == '.'; })) {
				return true;
			}
			return false;
		};
		if(next_opt.has_value() && has_part_symbol(*next_opt)) {
			result += n;
			continue;
		}

		if(previous_opt.has_value() && has_part_symbol(*previous_opt)) {
			result += n;
			continue;
		}
	}
	return result;
}

constexpr size_t row_gear_ratios(std::optional<std::string_view> previous_opt, std::string_view current, std::optional<std::string_view> next_opt) {
	size_t result = 0uz;
	std::array<size_t, 2> gears{};
	for(auto it = std::ranges::find(current, '*'); it != current.end(); it = std::ranges::find(it + 1, current.end(), '*')) {
		auto it_index = std::distance(current.begin(), it);
		size_t gear_index = 0uz;
		if(it != current.begin() && is_digit(*(it - 1))) {
			auto first = std::ranges::find_if_not(current.rend() - it_index, current.rend(), is_digit).base();
			std::from_chars(first, current.end(), gears[gear_index++]);
		}
		if(it < current.end() - 1 && is_digit(it[1])) {
			std::from_chars(it+1, current.end(), gears[gear_index++]);
		}

		if(next_opt.has_value()) {
			auto next = *next_opt;
			if(it_index > 0 && is_digit(next[it_index - 1])) {
				auto first = std::ranges::find_if_not(next.rend() - it_index, next.rend(), is_digit).base();
				std::from_chars(first, next.end(), gears[gear_index++]);
				if(!is_digit(next[it_index]) && it_index < next.size() - 1 && is_digit(next[it_index + 1])) {
					std::from_chars(next.begin() + it_index + 1, next.end(), gears[gear_index++]);
				}
			} else if(is_digit(next[it_index])) {
				std::from_chars(next.begin() + it_index, next.end(), gears[gear_index++]);
			} else if(it_index < next.size() - 1 && is_digit(next[it_index + 1])) {
				std::from_chars(next.begin() + it_index + 1, next.end(), gears[gear_index++]);
			}
		}

		if(previous_opt.has_value()) {
			auto previous = *previous_opt;
			if(it_index > 0 && is_digit(previous[it_index - 1])) {
				auto first = std::ranges::find_if_not(previous.rend() - it_index, previous.rend(), is_digit).base();
				std::from_chars(first, previous.end(), gears[gear_index++]);
				if(!is_digit(previous[it_index]) && it_index < previous.size() - 1 && is_digit(previous[it_index + 1])) {
					std::from_chars(previous.begin() + it_index + 1, previous.end(), gears[gear_index++]);
				}
			} else if(is_digit(previous[it_index])) {
				std::from_chars(previous.begin() + it_index, previous.end(), gears[gear_index++]);
			} else if(it_index < previous.size() - 1 && is_digit(previous[it_index + 1])) {
				std::from_chars(previous.begin() + it_index + 1, previous.end(), gears[gear_index++]);
			}
		}
		result += std::exchange(gears[0], 0) * std::exchange(gears[1], 0);
	}
	return result;
}

template<size_t N, bool part>
constexpr auto process_input_data(auto&& lines) {
	std::array<size_t, N> part_numbers{};
	std::ranges::transform(std::views::enumerate(lines), part_numbers.begin(), [&lines](auto&& p) {
		auto&& [i, current] = p;
		auto prev = i == 0 ? std::nullopt : std::optional<std::string_view>(lines[i-1]);
		auto next = i == lines.size() - 1 ? std::nullopt : std::optional<std::string_view>(lines[i+1]);
		if constexpr (part) return row_gear_ratios(prev, current, next);
		else                return row_part_numbers(prev, current, next);
	});
	return part_numbers;
}

constexpr size_t number_of_lines = std::ranges::count(input, '\n') + 1;
constexpr auto input_lines = input_to_lines<number_of_lines>(input);
constexpr auto part_numbers = process_input_data<number_of_lines, false>(input_lines);
constexpr auto gear_ratios = process_input_data<number_of_lines, true>(input_lines);
constexpr auto result_p1 = std::ranges::fold_left(part_numbers, 0uz, std::plus{});
constexpr auto result_p2 = std::ranges::fold_left(gear_ratios, 0uz, std::plus{});
auto r1 = result_p1;
auto r2 = result_p2;
