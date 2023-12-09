#include <charconv>
#include <iterator>
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

struct parsed_map {
	size_t input_start;
	size_t input_end;
	size_t output_start;
};

constexpr auto is_digit = [](char c) { return '0' <= c && c <= '9'; };

template<size_t seed_count, bool part1>
constexpr auto parse_seeds(std::string_view line) {
	std::array<std::pair<size_t, size_t>, seed_count> arr;
	line.remove_prefix(7);
	if constexpr (part1) {
		std::ranges::transform(std::views::split(line, " "sv), arr.begin(), [](auto&& n) {
			std::string_view sv(n);
			size_t num{};
			std::from_chars(sv.begin(), sv.end(), num);
			return std::pair{num, 1};
		});
	} else {
		std::ranges::transform(std::views::split(line, " "sv) | std::views::chunk(2), arr.begin(), [](auto&& chunk) {
			auto it = chunk.begin();
			auto start_r = *it++;
			auto range_r = *it;
			std::string_view start_sv(start_r);
			std::string_view range_sv(range_r);
			size_t start_number = 0uz;
			std::from_chars(start_sv.begin(), start_sv.end(), start_number);
			size_t range_number = 0uz;
			std::from_chars(range_sv.begin(), range_sv.end(), range_number);
			return std::pair{start_number, range_number};
			
		});
	}
	return arr;
}

template<bool part1>
constexpr auto count_seeds(std::string_view line) {
	line.remove_prefix(7);
	if constexpr (part1) {
		return std::ranges::distance(std::views::split(line, " "sv));
	} else {
		return std::ranges::distance(std::views::split(line, " "sv)) / 2;
	}
}

constexpr auto count_outputs(auto&& inputs, auto&& map) {
	size_t total = 0uz;
	for(auto const& [start, extent] : inputs) {
		for(auto i = start; i < start+extent;++i) {
			++total;
			auto it = std::ranges::find_if(map, [i](parsed_map pm) {
				return pm.input_start <= i && i <= pm.input_end;
			});
			if(it != map.end()) {
				i = std::min(start + extent - 1, it->input_end); // up to the end of mapped range
			} else if(i > std::ranges::max(map, {}, &parsed_map::input_end).input_end) {
				i = start + extent - 1; // end
			} else {
				// up to the start of next mapped range
				i = std::ranges::min(map | std::views::filter([i](parsed_map pm) { return i < pm.input_start; }), {}, &parsed_map::input_start).input_start - 1;
			}
		}
	}
	return total;
}

constexpr auto find_map(auto&& lines, std::string_view map_name) {
	auto start = std::ranges::find(lines, map_name) + 1;
	auto end = std::ranges::find_if(start, lines.end(), std::ranges::empty);
	return std::pair{start, end};
}

template<size_t map_size>
constexpr auto parse_map(auto&& lines, std::string_view map_name) {
	std::array<parsed_map, map_size> arr;
	auto [start, end] = find_map(lines, map_name);
	std::ranges::transform(start, end, arr.begin(), [](auto&& line) {
		parsed_map result;
		auto it = std::from_chars(line.begin(), line.end(), result.output_start).ptr + 1;
		it = std::from_chars(it, line.end(), result.input_start).ptr + 1;
		size_t range;
		std::from_chars(it, line.end(), range);
		result.input_end = result.input_start + range - 1;
		return result;

	});
	return arr;
}

constexpr auto calculate_map_size(auto&& lines, std::string_view map_name) {
	auto [start, end] = find_map(lines, map_name);
	return end - start;
}

template<size_t input_size, bool part1>
constexpr auto execute_map(auto&& map, auto&& inputs) {
	std::array<std::pair<size_t, size_t>, input_size> outputs;
	auto arr_it = outputs.begin();
	for(auto const& [start, extent] : inputs) {
		for(auto i = start; i < start+extent; ++i) {
			auto it = std::ranges::find_if(map, [i](parsed_map pm) {
				return pm.input_start <= i && i <= pm.input_end;
			});
			if(it != map.end()) {
				arr_it->first = i - it->input_start + it->output_start;
				i = std::min(start + extent - 1, it->input_end); // up to the end of mapped range
				arr_it++->second = part1 ? 1 : i - it->input_start + it->output_start;
			} else if(i > std::ranges::max(map, {}, &parsed_map::input_end).input_end) {
				arr_it->first = i;
				i = start + extent - 1; // end
				arr_it++->second = part1 ? 1 : i;
			} else {
				arr_it->first = i;
				i = std::ranges::min(map | std::views::filter([i](parsed_map pm) { return i < pm.input_start; }), {}, &parsed_map::input_start).input_start - 1;
				arr_it++->second = part1 ? 1 : i;
			}
		}
	}
	return outputs;
}


constexpr size_t number_of_lines = std::ranges::count(input, '\n') + 1;
constexpr auto input_lines = input_to_lines<number_of_lines>(input);
constexpr size_t seed_to_soil_size = calculate_map_size(input_lines, "seed-to-soil map:"sv);
constexpr size_t soil_to_fertilizer_size = calculate_map_size(input_lines, "soil-to-fertilizer map:"sv);
constexpr size_t fertilizer_to_water_size = calculate_map_size(input_lines, "fertilizer-to-water map:"sv);
constexpr size_t water_to_light_size = calculate_map_size(input_lines, "water-to-light map:"sv);
constexpr size_t light_to_temperature_size = calculate_map_size(input_lines, "light-to-temperature map:"sv);
constexpr size_t temperature_to_humidity_size = calculate_map_size(input_lines, "temperature-to-humidity map:"sv);
constexpr size_t humidity_to_location_size = calculate_map_size(input_lines, "humidity-to-location map:"sv);
constexpr auto seed_to_soil = parse_map<seed_to_soil_size>(input_lines, "seed-to-soil map:"sv);
constexpr auto soil_to_fertilizer = parse_map<soil_to_fertilizer_size>(input_lines, "soil-to-fertilizer map:"sv);
constexpr auto fertilizer_to_water = parse_map<fertilizer_to_water_size>(input_lines, "fertilizer-to-water map:"sv);
constexpr auto water_to_light = parse_map<water_to_light_size>(input_lines, "water-to-light map:"sv);
constexpr auto light_to_temperature = parse_map<light_to_temperature_size>(input_lines, "light-to-temperature map:"sv);
constexpr auto temperature_to_humidity = parse_map<temperature_to_humidity_size>(input_lines, "temperature-to-humidity map:"sv);
constexpr auto humidity_to_location = parse_map<humidity_to_location_size>(input_lines, "humidity-to-location map:"sv);
constexpr size_t size_of_seeds = count_seeds<false>(input_lines[0]);
constexpr auto seeds = parse_seeds<size_of_seeds, false>(input_lines[0]);
constexpr size_t size_of_soils = count_outputs(seeds, seed_to_soil);
constexpr auto soils = execute_map<size_of_soils, false>(seed_to_soil, seeds);
constexpr size_t size_of_fertilizers = count_outputs(soils, soil_to_fertilizer);
constexpr auto fertilizers = execute_map<size_of_fertilizers, false>(soil_to_fertilizer, soils);
constexpr size_t size_of_waters = count_outputs(fertilizers, fertilizer_to_water);
constexpr auto waters = execute_map<size_of_waters, false>(fertilizer_to_water, fertilizers);
constexpr size_t size_of_lights = count_outputs(waters, water_to_light);
constexpr auto lights = execute_map<size_of_lights, false>(water_to_light, waters);
constexpr size_t size_of_temperatures = count_outputs(lights, light_to_temperature);
constexpr auto temperatures = execute_map<size_of_temperatures, false>(light_to_temperature, lights);
constexpr size_t size_of_humidities = count_outputs(temperatures, temperature_to_humidity);
constexpr auto humidities = execute_map<size_of_humidities, false>(temperature_to_humidity, temperatures);
constexpr size_t size_of_locations = count_outputs(humidities, humidity_to_location);
constexpr auto locations = execute_map<size_of_locations, false>(humidity_to_location, humidities);
constexpr auto min_location = std::ranges::min(locations, {}, &std::pair<size_t, size_t>::first).first;
size_t r1 = min_location;
