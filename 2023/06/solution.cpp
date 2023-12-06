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

constexpr double half_assed_sqrt(double n) {
	return std::ranges::fold_left(std::views::iota(1, 120), n/2, [n](double acc, int _) { return (acc*acc + n)/2/acc; });
}

constexpr size_t race_to_win_ranges(size_t time, size_t distance) {
	double x1 = (time - half_assed_sqrt(time*time - 4*distance))/2;
	double x2 = (time + half_assed_sqrt(time*time - 4*distance))/2;
	size_t min = std::floor(x1);
	size_t max = std::ceil(x2);
	return max - min - 1;
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
constexpr size_t timings_p2 = std::ranges::fold_left(input_lines[0].substr(input_lines[0].find_first_of("1234567890"sv)) | std::views::filter(is_digit), 0uz, [](size_t acc, char c) {
	return acc*10uz+c-'0';
});
constexpr size_t distances_p2 = std::ranges::fold_left(input_lines[1].substr(input_lines[1].find_first_of("1234567890"sv)) | std::views::filter(is_digit), 0uz, [](size_t acc, char c) {
	return acc*10uz+c-'0';
});
constexpr size_t part1 = std::ranges::fold_left(calc_pipeline, 1uz, std::multiplies{});
constexpr size_t part2 = race_to_win_ranges(timings_p2, distances_p2);
size_t r1 = part1;
size_t r2 = part2;
