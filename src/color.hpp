#pragma once

#include <vector>
#include <ostream>
#include "types.hpp"

namespace engine
{
	class color
	{
	public:
		color(real r = 0.0, real g = 0.0, real b = 0.0, real a = 1.0)
		: m_data{ r, g, b, a } {}

		real* data() { return m_data; }
		const real* data() const { return m_data; }

		real& operator[](ColorComponent c) { return m_data[c]; }
		const real& operator[](ColorComponent c) const { return m_data[c]; }

	private:
		real m_data[4];
	};

	using color_buffer = std::vector<color>;

	inline std::ostream& operator << (std::ostream& os, const color& c)
	{
		os << "[" << c[R] << ", " << c[G] << ", " << c[B] << ", " << c[A] << "]";
		return os;
	}
}
