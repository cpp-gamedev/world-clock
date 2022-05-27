#include <app/app.hpp>
#include <vulkify/context/context.hpp>
#include <random>

namespace wc {
namespace {
float random_range(float min, float max) {
	static auto eng = std::default_random_engine(std::random_device{}());
	return std::uniform_real_distribution<float>{min, max}(eng);
}
} // namespace

app_t::app_t(ktl::not_null<vf::Context const*> context, world_clock_t clock, info_t const& info) : m_clock(std::move(clock)), m_context(context) {
	m_gui.face = wc::face_t::make(*context, info);
	for (auto const& hour : m_clock) {
		if (!m_gui.hands.has_space()) { break; }
		m_gui.hands.push_back(wc::hand_t::make(*context, hour.hour, vf::Rgba::make(hour.colour), info));
	}

	auto const face_radius = m_gui.face.body.diameter() * 0.5f;
	auto const eye_y = info.centre.y + face_radius * info.eye_npos.y;
	auto const eye_dx = face_radius * info.eye_npos.x;
	m_gui.eyes[0] = wc::eye_t::make(*context, {info.centre.x - eye_dx, eye_y}, info);
	m_gui.eyes[1] = wc::eye_t::make(*context, {info.centre.x + eye_dx, eye_y}, info);

	m_blink.next = vf::Time(random_range(blink_rate.first.count(), blink_rate.second.count()));
}

bool app_t::tick(vf::Frame const& frame) {
	m_input.update(frame.poll().events);
	if (m_input[button_t::escape]) { return false; }

	if (flags.test(flag::reset)) {
		reset(frame.dt());
	} else {
		free_rotate(frame.dt());
	}
	blink(frame.dt());

	eyes(m_context->unproject(m_context->cursorPosition()));
	return true;
}

void app_t::free_rotate(vf::Time const dt) {
	auto dr = vf::Radian{};
	if (m_input[button_t::left]) { dr = vf::Degree{dt.count() * 180.0f}; }
	if (m_input[button_t::right]) { dr = vf::Degree{-dt.count() * 180.0f}; }
	if (m_input[button_t::space]) {
		m_offset.previous = m_offset.current;
		flags.set(flag::reset);
		return;
	}

	if (dr != 0.0f) {
		m_offset.current.value += dr;
		for (auto& hand : m_gui.hands) { hand.quad.transform().orientation.rotate(dr); }
	}
}

void app_t::reset(vf::Time const dt) {
	vf::Radian const drad = vf::Degree{dt.count() * 360.0f * 2.0f}; // 2 rev/s
	auto const coeff = m_offset.previous < 0.0f ? 1.0f : -1.0f;
	auto const dr = coeff * std::min(drad.value, std::abs(m_offset.current));
	m_offset.current.value += dr;
	for (auto& hand : m_gui.hands) { hand.quad.transform().orientation.rotate(vf::Radian{dr}); }
	if (vf::FloatEq{}(m_offset.current, 0.0f)) { flags.reset(flag::reset); }
}

void app_t::blink(vf::Time const dt) {
	if (m_blink.remain > 0s) {
		m_blink.remain -= dt;
		if (m_blink.remain <= 0s) { flags.reset(flag::blink); }
		return;
	}
	m_blink.next -= dt;
	if (m_blink.next <= 0s) {
		m_blink.next = vf::Time(random_range(blink_rate.first.count(), blink_rate.second.count()));
		m_blink.remain = blink_duration;
		flags.set(flag::blink);
	}
}

void app_t::eyes(glm::vec2 const cursor) {
	for (auto& eye : m_gui.eyes) {
		auto const diff = cursor - eye.iris.transform().position;
		auto const dir = vf::nvec2(diff);
		auto const diam = eye.iris.diameter() * 0.25f;
		auto const offset = dir.value() * (eye.iris.diameter() - diam) * 0.80f * 0.5f; // 20% padding
		eye.pupil.transform().position = eye.iris.transform().position + offset;
	}
}
} // namespace wc
