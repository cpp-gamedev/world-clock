#pragma once
#include <vulkify/instance/event.hpp>
#include <span>

namespace wc {
enum class button_t { left, right, space, enter, escape, count_ };
enum class btn_state_t { none, press, hold, release };

struct buttons_t {
	bool pressed[static_cast<std::size_t>(button_t::count_)]{};

	bool& operator[](button_t btn) { return pressed[static_cast<std::size_t>(btn)]; }
	bool operator[](button_t btn) const { return pressed[static_cast<std::size_t>(btn)]; }
};

struct input_t {
	buttons_t current{};
	buttons_t previous{};

	void on_key(vf::KeyEvent const& key) {
		auto const value = key.action != vf::Action::eRelease;
		switch (key.key) {
		case vf::Key::eLeft:
		case vf::Key::eA: current[button_t::left] = value; break;
		case vf::Key::eRight:
		case vf::Key::eD: current[button_t::right] = value; break;
		case vf::Key::eSpace: current[button_t::space] = value; break;
		case vf::Key::eEnter: current[button_t::enter] = value; break;
		case vf::Key::eEscape: current[button_t::escape] = value; break;
		default: break;
		}
	}

	void update(std::span<vf::Event const> events) {
		previous = current;
		for (auto const& event : events) {
			switch (event.type) {
			case vf::EventType::eKey: on_key(event.get<vf::EventType::eKey>()); break;
			default: break;
			}
		}
	}

	bool operator[](button_t btn) const { return current[btn]; }
};
} // namespace wc
