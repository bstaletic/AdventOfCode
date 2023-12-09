#include <charconv>
#include <functional>
#include <iterator>
#include <numeric>
#include <string_view>
#include <array>
#include <algorithm>
#include <ranges>
using namespace std::literals;

constexpr char raw_input[] {
#include "input.h"
};

constexpr std::string_view input(raw_input, sizeof(raw_input) - 1); // Strip trailing new line.

template<bool part1>
constexpr auto parse_input() {
	constexpr size_t N = std::ranges::count(input, '\n') + 1;
	constexpr size_t M = std::ranges::distance(input.substr(0, input.find('\n')) | std::views::split(" "sv));
	std::array<std::array<ptrdiff_t, M>, N> out{};
	std::ranges::transform(std::views::split(input, "\n"sv), out.begin(), [](auto&& l) static {
		std::string_view line(l);
		std::array<ptrdiff_t, M> result;
		std::ranges::transform(std::views::split(line, " "sv), result.begin(), [](auto&& r) static {
			ptrdiff_t n{};
			std::from_chars(r.begin(), r.end(), n);
			return n;
		});
		if constexpr (!part1) std::ranges::reverse(result);
		return result;
	});
	return out;
}

template<size_t N>
constexpr ptrdiff_t get_next(std::array<ptrdiff_t, N> const& single) {
	std::array<std::array<ptrdiff_t, N>, N> single_with_diffs{single};
	for(ptrdiff_t offset = 0z; auto&& tup : single_with_diffs | std::views::adjacent<2>) {
		auto& [first, second] = tup;
		std::adjacent_difference(first.begin(), first.end(), second.begin());
		auto second_check_start = second.begin() + ++offset;
		if(std::ranges::all_of(second_check_start, second.end(), std::bind_front(std::equal_to{}, *second_check_start))) break;
	}
	return std::ranges::fold_left(
		single_with_diffs | std::views::transform([](std::array<ptrdiff_t, N> const& arr) static { return arr.back(); }),
		0uz,
		std::plus{});
}

template<size_t N, size_t M>
constexpr auto get_all_nexts(std::array<std::array<ptrdiff_t, M>, N> const& data) {
	std::array<ptrdiff_t, N> nexts;
	std::ranges::transform(data, nexts.begin(), get_next<M>);
	return nexts;
}

constexpr auto data_p1 = parse_input<true>();
constexpr auto nexts_p1 = get_all_nexts(data_p1);
constexpr ptrdiff_t part1 = std::ranges::fold_left(nexts_p1, 0z, std::plus{});
ptrdiff_t r1 = part1;

constexpr auto data_p2 = parse_input<false>();
constexpr auto nexts_p2 = get_all_nexts(data_p2);
constexpr ptrdiff_t part2 = std::ranges::fold_left(nexts_p2, 0z, std::plus{});
ptrdiff_t r2 = part2;
