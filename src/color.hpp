#pragma once

#include <ostream>
#include "types.hpp"
#include "buffer.hpp"
#include "opengl.hpp"

namespace engine
{
	class color
	{
	public:
		color(real r = 0.0f, real g = 0.0f, real b = 0.0f, real a = 1.0f)
		: m_data{ r, g, b, a } {}

		real* data() { return m_data; }
		const real* data() const { return m_data; }

		real& operator[](ColorComponent c) { return m_data[c]; }
		const real& operator[](ColorComponent c) const { return m_data[c]; }

	private:
		real m_data[4];
	};

	using color_buffer = buffer_t<color, real, 4, k_color_attribute_name, GL_ARRAY_BUFFER, GL_FLOAT>;

	inline std::ostream& operator << (std::ostream& os, const color& c)
	{
		os << "[" << c[R] << ", " << c[G] << ", " << c[B] << ", " << c[A] << "]";
		return os;
	}
}
