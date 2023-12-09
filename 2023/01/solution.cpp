#include <string_view>
#include <ranges>
#include <algorithm>

using namespace std::literals;

constexpr char raw_input[] {
#include "input.h"
};

constexpr std::string_view input(raw_input, sizeof(raw_input) - 1); // Strip trailing new line.

template<size_t word_count>
constexpr size_t get_calibration_digit(auto&& line, auto&& words) {
	std::array<decltype(line.begin()), word_count> iters{};
	for(size_t i = 0; auto&& word : words) {
		iters[i++] = std::ranges::search(line, word).begin();
	}
	auto index = std::ranges::distance(iters.begin(), std::ranges::min_element(iters));
	return index > 8 ? (index - 8 + 1) : index + 1;
}

constexpr size_t get_calibration_number_p2(std::string_view line) {
	constexpr std::array spellings{"1"sv, "2"sv, "3"sv, "4"sv, "5"sv, "6"sv, "7"sv, "8"sv, "9"sv, "one"sv, "two"sv, "three"sv, "four"sv, "five"sv, "six"sv, "seven"sv, "eight"sv, "nine"sv};
	constexpr std::array reverse_spellings{"1"sv, "2"sv, "3"sv, "4"sv, "5"sv, "6"sv, "7"sv, "8"sv, "9"sv, "eno"sv, "owt"sv, "eerht"sv, "ruof"sv, "evif"sv, "xis"sv, "neves"sv, "thgie"sv, "enin"sv};
	size_t first_digit = get_calibration_digit<spellings.size()>(line, spellings);
	size_t last_digit = get_calibration_digit<reverse_spellings.size()>(line | std::views::reverse, reverse_spellings);
	return 10uz*first_digit + last_digit;
}

constexpr size_t get_calibration_number_p1(auto&& input_line) {
	auto first_digit = *std::ranges::find_if(input_line, [](char c) { return '0' <= c && c <= '9'; });
	auto last_digit = std::ranges::find_last_if(input_line, [](char c) { return '0' <= c && c <= '9'; }).front();
	return 10*(first_digit - '0') + (last_digit - '0');
}

template<bool part>
consteval auto lines_to_calibration_numbers(auto&& lines) {
	return std::views::transform(lines, [](std::string_view line) {
		if constexpr (part)
			return get_calibration_number_p2(line);
		else
			return get_calibration_number_p1(line);
	});
}

template<size_t size>
constexpr auto split_into_lines() {
	// auto lines = input | std::views::split("\n"sv) | std::ranges::to<std::array<std::string_view, size>>();
	std::array<std::string_view, size> lines{};
	std::ranges::copy(input | std::views::split("\n"sv) | std::views::transform([](auto&& l) { return std::string_view(l); }), lines.begin());
	return lines;
}

template<size_t pipeline_size>
constexpr auto eval_calibration_numbers_pipeline(auto&& pipeline) {
	std::array<size_t, pipeline_size> calib{};
	std::ranges::copy(pipeline, calib.begin());
	return calib;
}

constexpr size_t number_of_lines = std::ranges::count(input, '\n') + 1;
constexpr auto lines = split_into_lines<number_of_lines>();
constexpr auto calibration_numbers_pipeline_p1 = lines_to_calibration_numbers<false>(lines);
constexpr auto calibration_numbers_pipeline_p2 = lines_to_calibration_numbers<true>(lines);
constexpr auto calibration_numbers_p1 = eval_calibration_numbers_pipeline<number_of_lines>(calibration_numbers_pipeline_p1);
constexpr auto calibration_numbers_p2 = eval_calibration_numbers_pipeline<number_of_lines>(calibration_numbers_pipeline_p2);
constexpr auto result_p1 = std::ranges::fold_left(calibration_numbers_p1, 0uz, std::plus{});
constexpr auto result_p2 = std::ranges::fold_left(calibration_numbers_p2, 0uz, std::plus{});
size_t result = result_p2;
