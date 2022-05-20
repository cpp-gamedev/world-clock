#pragma once
#include <clock/world_hour.hpp>
#include <glm/vec2.hpp>
#include <ktl/fixed_vector.hpp>
#include <vulkify/context/frame.hpp>
#include <vulkify/core/rgba.hpp>
#include <vulkify/graphics/primitives/circle_shape.hpp>
#include <vulkify/graphics/primitives/quad_shape.hpp>

namespace wc {
struct info_t {
	static constexpr std::size_t max_hands_v = 8;

	glm::vec2 centre{};
	glm::vec2 hand_pivot{0.0f, 0.4f};
	glm::vec2 eye_npos{0.35f, 0.2f};
	vf::Rgba face_tint{vf::Rgba::make(0xccccbbff)};
	vf::Rgba marker_tint{vf::Rgba::make(0x555555ff)};
	vf::Rgba pupil_tint{vf::Rgba::make(0xa020f0ff)};
	float hand_height{250.0f};
	float marker_radius{10.0f};
	float iris_radius{50.0f};
	float face_pad{0.2f};
	float scale{1.0f};
};

struct face_t {
	vf::CircleShape body{};
	vf::CircleShape markers[8]{};

	static face_t make(vf::Context const& context, info_t const& info = {});

	void draw(vf::Frame const& frame) const {
		frame.draw(body);
		for (auto const& marker : markers) { frame.draw(marker); }
	}
};

struct hand_t {
	vf::QuadShape quad{};

	static hand_t make(vf::Context const& context, world_hour_t hour, vf::Rgba tint, info_t const& info = {});

	void draw(vf::Frame const& frame) const { frame.draw(quad); }
};

struct eye_t {
	vf::CircleShape iris{};
	vf::CircleShape pupil{};

	static eye_t make(vf::Context const& context, glm::vec2 position, info_t const& info = {});

	void draw(vf::Frame const& frame) const {
		frame.draw(iris);
		frame.draw(pupil);
	}
};

struct gui_t {
	face_t face{};
	eye_t eyes[2]{};
	ktl::fixed_vector<hand_t, info_t::max_hands_v> hands{};
};
} // namespace wc
