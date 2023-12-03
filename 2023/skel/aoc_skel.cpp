#include <string_view>
#include <array>
#include <algorithm>
#include <ranges>
using namespace std::literals;

constexpr std::string_view input {
R"()"sv
};
template<size_t size>
constexpr auto input_to_lines(std::string_view input) {
	std::array<std::string_view, size> lines{};
	std::ranges::transform(std::views::split(input, "\n"sv), lines.begin(), [](auto&& l) { return std::string_view(l); });
	return lines;
}

constexpr auto is_digit = [](char c) { return '0' <= c && c <= '9'; };

template<size_t N, bool part>
constexpr auto process_input_data(auto&& lines) {
}

constexpr size_t number_of_lines = std::ranges::count(input, '\n') + 1;
constexpr auto input_lines = input_to_lines<number_of_lines>(input);
