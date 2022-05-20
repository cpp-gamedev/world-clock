#pragma once
#include <ktl/fixed_vector.hpp>
#include <string>

namespace wc::io {
struct hand_t {
	std::string key{};
	std::uint32_t colour_hex{};
	float offset{};
};

struct clock_t {
	ktl::fixed_vector<hand_t, 16> hands{};
};

bool read_file(std::string& out_text, char const* path);
bool from_text(clock_t& out_clock, std::string_view text);
} // namespace wc::io
