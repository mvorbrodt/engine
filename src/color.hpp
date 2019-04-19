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
		color(real r = 0.0, real g = 0.0, real b = 0.0, real a = 1.0)
		: m_data{ r, g, b, a } {}

		real* data() { return m_data; }
		const real* data() const { return m_data; }

		real& operator[](ColorComponent c) { return m_data[c]; }
		const real& operator[](ColorComponent c) const { return m_data[c]; }

		inline static const std::size_t k_component_count = 4;
		inline static std::size_t component_count() { return k_component_count; }

	private:
		real m_data[k_component_count];
	};

	using color_buffer = buffer_t<color, k_color_buffer_name, color::k_component_count, GL_ARRAY_BUFFER, GL_FLOAT>;

	inline std::ostream& operator << (std::ostream& os, const color& c)
	{
		os << "[" << c[R] << ", " << c[G] << ", " << c[B] << ", " << c[A] << "]";
		return os;
	}
}
