#include <charconv>
#include <compare>
#include <cstdint>
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
	std::ranges::transform(std::views::split(input, "\n"sv), lines.begin(), [](auto&& l) static { return std::string_view(l); });
	return lines;
}

enum struct hand_kind : uint8_t {
	high_card,
	one_pair,
	two_pairs,
	three_of_a_kind,
	full_house,
	four_of_a_kind,
	five_of_a_kind,
};

struct hand {
	hand_kind kind;
	std::array<int, 5> hand;
	uint16_t score;
	constexpr auto operator<=>(const struct hand&) const = default;
};

template<bool part1>
constexpr int card_to_number(char c) {
	switch(c) {
		case 'A': return 14;
		case 'K': return 13;
		case 'Q': return 12;
		case 'J': return 11 - !part1*10;
		case 'T': return 10;
		 default: return c - '0';
	}
}

template<bool part1>
constexpr hand parse_line(std::string_view line) {
	hand h{};

	std::string_view score_sv = line.substr(line.find(' ') + 1);
	std::from_chars(score_sv.begin(), score_sv.end(), h.score);

	std::string_view hand = line.substr(0, line.find(' '));

	std::array<int, 15> cards_and_counts{};
	for(char c : hand) {
		if(part1 || c != 'J')
			cards_and_counts[card_to_number<part1>(c)]++;
	}
	auto max_it = std::ranges::max_element(cards_and_counts);
	if constexpr (!part1) {
		if(*max_it == 2) {
			max_it = std::ranges::find_last(cards_and_counts, 2).begin();
		}
		*max_it += std::ranges::count(hand, 'J');
	}

	auto pair_count = std::ranges::count(cards_and_counts, 2);

	switch(*max_it) {
		case 1: h.kind = hand_kind::high_card; break;
		case 5: h.kind = hand_kind::five_of_a_kind; break;
		case 4: h.kind = hand_kind::four_of_a_kind; break;
		case 3: h.kind = pair_count == 1 ? hand_kind::full_house : hand_kind::three_of_a_kind; break;
		case 2: h.kind = pair_count == 2 ? hand_kind::two_pairs  : hand_kind::one_pair; break;
	}
	std::ranges::copy(
		std::views::transform(hand, card_to_number<part1>),
		h.hand.begin()
	);
	return h;
}

template<size_t size>
constexpr auto sorted(std::array<hand, size> range) {
	std::ranges::sort(range);
	return range;
}

constexpr size_t number_of_lines = std::ranges::count(input, '\n') + 1;
constexpr auto input_lines = input_to_lines<number_of_lines>(input);

constexpr auto all_hands_p1 = std::ranges::fold_left(
	std::views::enumerate(input_lines | std::views::transform(parse_line<true>)),
	std::array<hand, number_of_lines>{},
	[](auto&& hands, auto&& counted_line) static -> decltype(auto) {
		auto&& [hands_index, line] = counted_line;
		hands[hands_index++] = line;
		return hands;
	});
constexpr auto all_hands_sorted_p1 = sorted(all_hands_p1);
constexpr size_t part1 = std::ranges::fold_left(
	std::views::zip_transform(
		std::multiplies{},
		std::views::iota(1uz, number_of_lines),
		all_hands_sorted_p1 | std::views::transform(&hand::score)
	),
	0uz,
	std::plus{});

constexpr auto all_hands_p2 = std::ranges::fold_left(
	std::views::enumerate(input_lines | std::views::transform(parse_line<false>)),
	std::array<hand, number_of_lines>{},
	[](auto&& hands, auto&& counted_line) static -> decltype(auto) {
		auto&& [hands_index, line] = counted_line;
		hands[hands_index++] = line;
		return hands;
	});
constexpr auto all_hands_sorted_p2 = sorted(all_hands_p2);
constexpr size_t part2 = std::ranges::fold_left(
	std::views::zip_transform(
		std::multiplies{},
		std::views::iota(1uz, number_of_lines),
		all_hands_sorted_p2 | std::views::transform(&hand::score)
	),
	0uz,
	std::plus{});

size_t r1 = part1;
size_t r2 = part2;
