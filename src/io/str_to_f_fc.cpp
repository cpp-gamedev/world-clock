#include <charconv>
#include "str_to_f.hpp"

namespace wc {
io::str2f_res io::str_to_f(char const* first, char const* last, float& out, bool hex) {
	auto [ptr, ec] = std::from_chars(first, last, out, hex ? std::chars_format::hex : std::chars_format::fixed);
	if (ec != std::errc()) { return {first, false}; }
	return {ptr, true};
}
} // namespace wc
