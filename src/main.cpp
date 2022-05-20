#include <app/app.hpp>
#include <io/io.hpp>
#include <vulkify/vulkify.hpp>
#include <iostream>

namespace {
std::ostream& operator<<(std::ostream& out, wc::world_hour_t wt) { return out << wt.hour(); }

std::ostream& operator<<(std::ostream& out, wc::world_clock_t const& clock) {
	for (auto const& entry : clock) { out << entry.name << ": " << entry.hour << '\n'; }
	return out;
}

wc::world_clock_t make_clock() {
	auto ret = wc::world_clock_t{};
	auto text = std::string{};
	if (wc::io::read_file(text, "world_clock.json")) {
		auto clock_data = wc::io::clock_t{};
		if (!wc::io::from_text(clock_data, text)) { return {}; }
		for (auto& hand : clock_data.hands) { ret.add(std::move(hand.key), hand.colour_hex, hand.offset); }
	} else {
		ret.add("PDT", 0xff8800ff, -7.0f);
		ret.add("CET", 0x11ee44ff, 2.0f);
	}
	return ret;
}

void run(vf::Context ctx, wc::world_clock_t clock) {
	std::cout << "\nclock:\n" << clock << '\n';

	auto info = wc::info_t{};
	auto app = wc::app_t(ctx, std::move(clock), info);
	ctx.show();

	while (!ctx.closing()) {
		auto frame = ctx.frame();
		if (!app.tick(frame)) { return; }
		app.draw(frame);
	}
}
} // namespace

int main() {
	auto clock = make_clock();
	if (clock.size() == 0) { return EXIT_FAILURE; }

	auto ctx = vf::Builder{}.setTitle("world clock").build();
	if (!ctx) {
		std::cerr << "Failed to create vulkify context\n";
		return EXIT_FAILURE;
	}

	std::cout << "using GPU: " << ctx->gpu().name << '\n';
	run(std::move(*ctx), std::move(clock));
}
