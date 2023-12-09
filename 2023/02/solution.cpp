#include <algorithm>
#include <charconv>
#include <iterator>
#include <string_view>
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

struct game_status {
	size_t power;
	size_t id;
	bool valid;
};

constexpr game_status line_to_game_status(std::string_view line) {
	constexpr std::string_view delimiters = ",;"sv;
	constexpr size_t available_blues = 14;
	constexpr size_t available_reds = 12;
	constexpr size_t available_greens = 13;
	size_t current_blue = 0;
	size_t current_red = 0;
	size_t current_green = 0;
	size_t max_blue = 0;
	size_t max_green = 0;
	size_t max_red = 0;
	size_t game_id;
	auto it = std::from_chars(line.begin() + 5, line.end(), game_id).ptr + 2;
	for(; it != line.end(); std::ranges::advance(it, 2, line.end())) {
		size_t n;
		it = std::from_chars(it, line.end(), n).ptr + 1;
		switch(*it) {
			case 'b': current_blue += n; break;
			case 'r': current_red += n; break;
			case 'g': current_green += n; break;
		}
		it = std::ranges::find_first_of(it, line.end(), delimiters.begin(), delimiters.end());
		if(it == line.end() || *it == ';') {
			max_blue = std::max(max_blue, std::exchange(current_blue, 0));
			max_green = std::max(max_green, std::exchange(current_green, 0));
			max_red = std::max(max_red, std::exchange(current_red, 0));
		}
	}
	return {.power = max_blue * max_green * max_red,.id = game_id, .valid = max_blue <= available_blues && max_red <= available_reds && max_green <= available_greens};
}

template<typename T, size_t N>
constexpr auto eval_pipeline(auto&& pipeline) {
	std::array<T, N> result;
	std::ranges::copy(pipeline, result.begin());
	return result;
}


constexpr size_t number_of_lines = std::ranges::count(input, '\n') + 1;
constexpr auto input_lines = input_to_lines<number_of_lines>(input);
constexpr auto all_game_statuses = input_lines | std::views::transform(line_to_game_status);
constexpr auto game_statuses = eval_pipeline<game_status, number_of_lines>(all_game_statuses);
constexpr auto result_p1 = std::ranges::fold_left(game_statuses, std::pair<size_t, size_t>{}, [](auto&& acc, game_status gs) {
	acc.first += gs.id*gs.valid;
	acc.second += gs.power;
	return acc;
});
auto id_sum = result_p1.first;
auto power_sum = result_p1.second;
