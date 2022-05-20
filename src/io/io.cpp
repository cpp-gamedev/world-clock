#include <io/io.hpp>
#include <charconv>
#include <fstream>
#include <iostream>

namespace wc {
namespace {
struct bad_parse {
	std::string obtained{};
	std::string expected{};
};

struct parser_t {
	io::clock_t& out_clock;
	std::string_view text{};

	void operator()() {
		if (text.empty()) { return; }
		advance('{');
		if (peek() == '}') { return; }
		while (true) {
			out_clock.hands.push_back(parse_hand());
			if (peek() != ',') { break; }
			advance(',', true);
		}
		advance('}');
		if (!text.empty()) { error("<eof>"); }
	}

	char peek() const { return text.empty() ? '\0' : text[0]; }

	void error(std::string_view expected, std::string_view obtained = {}) const {
		throw bad_parse{obtained.empty() ? std::string(text.substr(0, text.find('\n'))) : std::string(obtained), std::string(expected)};
	}

	void expect(char ch) const {
		if (text.empty() || text[0] != ch) { error({&ch, 1}); }
	}

	void expect_not(char ch, std::string_view exp) const {
		if (!text.empty() && text[0] == ch) { error(exp); }
	}

	void skip_ws() {
		while (!text.empty() && std::isspace(static_cast<unsigned char>(text[0]))) { text = text.substr(1); }
	}

	void advance(char expect, bool skip_afer = true) {
		skip_ws();
		this->expect(expect);
		text = text.substr(1);
		if (skip_afer) { skip_ws(); }
	}

	io::hand_t parse_hand() {
		auto ret = io::hand_t{};
		ret.key = quoted(false);
		advance(':');
		advance('{');
		expect_not('}', "<key>");
		while (true) {
			auto key = quoted(false);
			advance(':', true);
			parse_value(ret, key);
			if (peek() != ',') { break; }
			advance(',');
		}
		advance('}');
		return ret;
	}

	std::string quoted(bool allow_empty) {
		advance('\"', false);
		auto end = text.find('\"');
		if (end == std::string_view::npos) { error("\""); }
		auto ret = std::string(text.substr(0, end));
		text = text.substr(end);
		advance('\"');
		if (!allow_empty && ret.empty()) { error("<string>"); }
		return ret;
	}

	void parse_value(io::hand_t& out_hand, std::string_view const key) {
		if (key == "offset") {
			parse_offset(out_hand);
		} else if (key == "colour" || key == "color") {
			parse_colour(out_hand);
		}
	}

	void parse_offset(io::hand_t& out_hand) {
		auto end = text.find_first_of(",}");
		if (end == std::string_view::npos) { error(", or }"); }
		auto [ptr, ec] = std::from_chars(text.data(), text.data() + end, out_hand.offset);
		if (ec != std::errc()) { error("<number>"); }
		text = text.substr(ptr - text.begin());
		skip_ws();
	}

	void parse_colour(io::hand_t& out_hand) {
		auto hex = quoted(false);
		if (hex.empty() || hex[0] != '#') { error("#<hex>", hex); }
		auto value = float{};
		auto [ptr, ec] = std::from_chars(hex.data() + 1, hex.data() + hex.size(), value, std::chars_format::hex);
		if (ec != std::errc()) { error("#<hex>"); }
		out_hand.colour_hex = value >= 1.0f ? static_cast<std::uint32_t>(value) - 1 : 0x333333ff;
	}
};
} // namespace

bool io::read_file(std::string& out_text, char const* path) {
	auto file = std::ifstream(path);
	if (!file) { return {}; }
	for (std::string line{}; std::getline(file, line); line.clear()) {
		out_text += std::move(line);
		out_text += '\n';
	}
	return true;
}

bool io::from_text(clock_t& out_clock, std::string_view text) {
	try {
		parser_t{out_clock, text}();
	} catch (bad_parse bp) {
		std::cerr << "JSON parse error (expected " << bp.expected << "):\n\n" << bp.obtained << "\n";
		return false;
	}
	return true;
}
} // namespace wc
