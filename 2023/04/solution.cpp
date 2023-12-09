#include <charconv>
#include <cmath>
#include <iterator>
#include <optional>
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

template<size_t number_of_plays, size_t numbers_in_play>
constexpr auto get_winning_numbers(auto&& lines, char start_indicator, std::optional<char> end_indicator = std::nullopt) {
	std::array<std::array<char, numbers_in_play>, number_of_plays> winning_numbers;
	std::ranges::transform(lines, winning_numbers.begin(), [start_indicator, end_indicator](std::string_view card) {
		auto start = card.find(start_indicator) + 2;
		if(!is_digit(card[start])) ++start;
		if(end_indicator.has_value()) {
			auto end = card.find(*end_indicator) - 1;
			card = card.substr(start, end - start);
		} else {
			card.remove_prefix(start);
		}
		std::array<char, numbers_in_play> win;
		auto win_it = win.begin();
		std::ranges::transform(card | std::views::split(" "sv) | std::views::filter(std::ranges::size), win.begin(), [](auto&& num) {
			char n{};
			std::string_view num_sv(num);
			std::from_chars(num.begin(), num.end(), n);
			return n;
		});
		std::ranges::sort(win);
		return win;
	});
	return winning_numbers;
}

constexpr ptrdiff_t count_winning_numbers_in_hand(auto&& win, auto&& hand) {
	std::array<char, 25> holder;
	auto number_of_wins = std::ranges::set_intersection(win, hand, holder.begin()).out - holder.begin();
	return number_of_wins;
}

template<size_t size>
constexpr auto count_winning_numbers_in_hand_all_games(auto&& wins, auto&& hands) {
	std::array<ptrdiff_t, size> arr{};
	std::ranges::transform(std::views::zip(wins, hands), arr.begin(), [](auto&& p) {
		auto&& [w, h] = p;
		return count_winning_numbers_in_hand(w, h);
	});
	return arr;
}

template<size_t size>
constexpr auto calculate_points(auto&& good_guesses) {
	std::array<size_t, size> arr{};
	std::ranges::transform(good_guesses, arr.begin(), [](ptrdiff_t n) -> size_t {
		return std::pow(2, n-1);
	});
	return arr;
}
constexpr auto get_guess_and_draws_count(std::string_view line) {
	auto drawn_start = line.find(':') + 2;
	auto drawn_end = line.find('|') - 1;
	auto played_start = line.find('|') + 2;
	auto played_end = line.size() - 1;
	auto drawn = line.substr(drawn_start, drawn_end - drawn_start);
	auto played = line.substr(played_start, played_end - played_start);
	auto drawn_amount = std::ranges::distance(std::views::split(drawn, " "sv) | std::views::filter(std::ranges::size));
	auto played_amount = std::ranges::distance(std::views::split(played, " "sv) | std::views::filter(std::ranges::size));
	return std::pair{played_amount, drawn_amount};
}

template<size_t size>
constexpr auto calculate_rewarded_cards(auto&& good_guesses) {
	std::array<size_t, size> arr{};
	std::ranges::fill(arr, 1);
	std::ranges::for_each(std::views::enumerate(good_guesses), [&arr](auto&& p) {
		auto&& [i, guess] = p;
		for(auto j = i+1; j < i+1+guess; j++) arr[j] += arr[i];
	});
	return arr;
}

constexpr size_t number_of_lines = std::ranges::count(input, '\n') + 1;
constexpr auto input_lines = input_to_lines<number_of_lines>(input);
constexpr auto guesses_and_draws_per_play = get_guess_and_draws_count(input_lines[0]);
constexpr auto winning_numbers = get_winning_numbers<number_of_lines, guesses_and_draws_per_play.second>(input_lines, ':', '|');
constexpr auto hands = get_winning_numbers<number_of_lines, guesses_and_draws_per_play.first>(input_lines, '|');
constexpr auto good_guesses = count_winning_numbers_in_hand_all_games<number_of_lines>(winning_numbers, hands);
constexpr auto points_per_card = calculate_points<number_of_lines>(good_guesses);
constexpr auto rewarded_cards = calculate_rewarded_cards<number_of_lines>(good_guesses);
constexpr size_t result_p1 = std::ranges::fold_left(points_per_card, 0uz, std::plus{});
constexpr size_t result_p2 = std::ranges::fold_left(rewarded_cards, 0uz, std::plus{});
size_t r1 = result_p1;
size_t r2 = result_p2;
