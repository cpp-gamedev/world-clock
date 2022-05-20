#include <gui/gui.hpp>
#include <ktl/enumerate.hpp>

namespace wc {
namespace {
constexpr auto max_rotation = vf::Degree{360.0f};

vf::Radian rotation(hour_t hour) noexcept { return vf::Degree{max_rotation * hour / total_hours}; }
} // namespace

face_t face_t::make(vf::Context const& context, info_t const& info) {
	auto ret = face_t{};
	auto const diameter = 2.0f * info.hand_height * (1.0f + info.face_pad);
	ret.body = vf::CircleShape(context, "face_body", {diameter});
	ret.body.tint() = info.face_tint.linear();
	for (auto [marker, index] : ktl::enumerate(ret.markers)) {
		marker = vf::CircleShape(context, "face_marker_" + std::to_string(index), {info.marker_radius * 2.0f});
		auto const hour = total_hours * static_cast<float>(index) / static_cast<float>(std::size(ret.markers));
		auto dir = vf::nvec2();
		dir.rotate(rotation(world_hour_t(hour).hour()));
		marker.transform().position = info.centre + dir.value() * diameter * 0.9f * 0.5f; // 10% padding
		marker.tint() = info.marker_tint.linear();
	}
	return ret;
}

hand_t hand_t::make(vf::Context const& context, world_hour_t hour, vf::Rgba tint, info_t const& info) {
	auto ret = hand_t{};
	auto qci = vf::QuadCreateInfo{};
	qci.size = glm::vec2(info.hand_height * 0.08f, info.hand_height) * info.scale;
	qci.origin = qci.size * info.hand_pivot;
	ret.quad = vf::QuadShape(context, "hand", qci);
	ret.quad.tint() = tint.linear();
	ret.quad.transform().position = info.centre;
	ret.quad.transform().orientation.rotate(rotation(-hour.hour()));
	return ret;
}

eye_t eye_t::make(vf::Context const& context, glm::vec2 const position, info_t const& info) {
	auto ret = eye_t{};
	ret.iris = vf::CircleShape(context, "iris", {info.iris_radius * info.scale * 2.0f});
	ret.pupil = vf::CircleShape(context, "pupil", {info.iris_radius * 0.25f * info.scale * 2.0f});
	ret.pupil.tint() = info.pupil_tint;
	ret.iris.transform().position = ret.pupil.transform().position = position;
	return ret;
}
} // namespace wc
