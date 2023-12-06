#include <iostream>
#include <charconv>
#include <cmath>
#include <string_view>
#include <array>
#include <algorithm>
#include <ranges>
using namespace std::literals;

constexpr std::string_view input {
R"(Time:        53     71     78     80
Distance:   275   1181   1215   1524)"sv
};
template<size_t size>
constexpr auto input_to_lines(std::string_view input) {
	std::array<std::string_view, size> lines{};
	std::ranges::transform(std::views::split(input, "\n"sv), lines.begin(), [](auto&& l) { return std::string_view(l); });
	return lines;
}

constexpr auto is_digit = [](char c) { return '0' <= c && c <= '9'; };

constexpr size_t get_number_of_races(std::string_view line) {
	line.remove_prefix(line.find_first_of("123456789"));
	return std::ranges::distance(std::views::split(line, " "sv) | std::views::filter(std::ranges::size));
}

size_t race_to_win_ranges(size_t time, size_t distance) {
	double x1 = (time - std::sqrt(time*time - 4*distance))/2;
	double x2 = (time + std::sqrt(time*time - 4*distance))/2;
	double min = std::ceil(std::min(x1, x2));
	double max = std::floor(std::max(x1, x2));
	size_t lower_bound = min;
	size_t upper_bound = max;
	if(lower_bound == std::min(x1, x2)) lower_bound++;
	if(upper_bound == std::max(x1, x2)) upper_bound--;
	return upper_bound - lower_bound + 1;
}

template<size_t races>
constexpr auto get_data(std::string_view line) {
	std::array<size_t, races> data;
	line.remove_prefix(line.find_first_of("123456789"));
	std::ranges::transform(std::views::split(line, " "sv) | std::views::filter(std::ranges::size), data.begin(), [](auto&& r) {
		std::string_view sv(r);
		size_t d = 0uz;
		std::from_chars(sv.begin(), sv.end(), d);
		return d;
	});
	return data;
}

constexpr size_t number_of_lines = std::ranges::count(input, '\n') + 1;
constexpr auto input_lines = input_to_lines<number_of_lines>(input);
constexpr size_t number_of_races = get_number_of_races(input_lines[0]);
constexpr auto timings_p1 = get_data<number_of_races>(input_lines[0]);
constexpr auto distances_p1 = get_data<number_of_races>(input_lines[1]);
constexpr auto calc_pipeline = std::views::zip_transform(
		race_to_win_ranges,
		timings_p1,
		distances_p1);
constexpr auto timings_p2 = std::ranges::fold_right(input_lines[0] | std::views::filter(is_digit), 0uz, [](size_t acc, char c) {
	return acc*10uz+c-'0';
});
constexpr auto distances_p2 = std::ranges::fold_right(input_lines[1] | std::views::filter(is_digit), 0uz, [](size_t acc, char c) {
	return acc*10uz+c-'0';
});
size_t part1 = std::ranges::fold_left(calc_pipeline, 1uz, std::multiplies{});
size_t part2 = race_to_win_ranges(timings_p2, distances_p2);
