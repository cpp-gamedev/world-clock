#include <io/str_to_f.hpp>
#include <cstdlib>
#include <string>

namespace wc {
io::str2f_res io::str_to_f(char const* first, char const*, float& out, bool hex) {
	auto buf = std::string{};
	if (hex) {
		buf.reserve(32);
		buf += "0x";
		buf += first;
	}
	char* next{};
	out = std::strtof(hex ? buf.data() : first, &next);
	if (next == first) { return {first, false}; }
	return {next, true};
}
} // namespace wc
