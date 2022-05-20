#pragma once
#include <clock/world_hour.hpp>
#include <string>
#include <vector>

namespace wc {
class world_clock_t {
  public:
	struct entry_t {
		std::string name;
		world_hour_t hour;
		std::uint32_t colour{};
	};

	using storage_t = std::vector<entry_t>;
	using const_iterator = typename storage_t::const_iterator;

	void add(std::string name, std::uint32_t colour = 0xffffffff, hour_t offset = {});
	bool remove(std::string_view name) noexcept;

	world_clock_t& operator+=(hour_t rhs) noexcept;
	world_clock_t& operator-=(hour_t rhs) noexcept;

	std::size_t size() const { return m_entries.size(); }

	const_iterator begin() const noexcept { return m_entries.begin(); }
	const_iterator end() const noexcept { return m_entries.end(); }
	const_iterator cbegin() const noexcept { return m_entries.begin(); }
	const_iterator cend() const noexcept { return m_entries.end(); }

  private:
	storage_t m_entries;
};

// impl

inline void world_clock_t::add(std::string name, std::uint32_t const colour, hour_t const offset) {
	m_entries.push_back({std::move(name), world_hour_t(offset), colour});
}

inline bool world_clock_t::remove(std::string_view name) noexcept {
	for (auto it = m_entries.begin(); it != m_entries.end(); ++it) {
		if (it->name == name) {
			m_entries.erase(it);
			return true;
		}
	}
	return false;
}

inline world_clock_t& world_clock_t::operator+=(hour_t rhs) noexcept {
	for (auto& entry : m_entries) { entry.hour += rhs; }
	return *this;
}

inline world_clock_t& world_clock_t::operator-=(hour_t rhs) noexcept {
	for (auto& entry : m_entries) { entry.hour -= rhs; }
	return *this;
}
} // namespace wc
