#pragma once

namespace wc::io {
struct str2f_res {
	char const* next{};
	bool success{};
};

str2f_res str_to_f(char const* first, char const* last, float& out, bool hex);
} // namespace wc::io
