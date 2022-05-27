#pragma once
#include <app/input.hpp>
#include <clock/world_clock.hpp>
#include <gui/gui.hpp>
#include <ktl/not_null.hpp>

namespace wc {
using namespace std::chrono_literals;

class app_t {
  public:
	app_t(ktl::not_null<vf::Context const*> context, world_clock_t clock, info_t const& info = {});

	bool tick(vf::Frame const& frame);

	void draw(vf::Frame const& frame) const {
		m_gui.face.draw(frame);
		if (!flags.test(wc::app_t::flag::blink)) {
			for (auto const& eye : m_gui.eyes) { eye.draw(frame); }
		}
		for (auto const& hand : m_gui.hands) { hand.draw(frame); }
	}

	std::pair<vf::Time, vf::Time> blink_rate{5s, 20s};
	vf::Time blink_duration{0.12s};

  private:
	void free_rotate(vf::Time const dt);
	void reset(vf::Time const dt);
	void blink(vf::Time const dt);
	void eyes(glm::vec2 cursor);

	enum class flag { reset, blink };

	world_clock_t m_clock{};
	gui_t m_gui{};
	input_t m_input{};

	struct {
		vf::Time remain{};
		vf::Time next{};
	} m_blink;
	struct {
		vf::Radian current{};
		vf::Radian previous{};
	} m_offset{};
	ktl::enum_flags<flag> flags{};
	ktl::not_null<vf::Context const*> m_context;
};
} // namespace wc
