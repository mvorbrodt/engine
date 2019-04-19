#pragma once

#include <ostream>
#include "types.hpp"
#include "buffer.hpp"
#include "opengl.hpp"

namespace engine
{
	class texcoord
	{
	public:
		texcoord(real x = 0.0f, real y = 0.0f)
		: m_data{ x, y } {}

		real* data() { return m_data; }
		const real* data() const { return m_data; }

		real& operator[](Coordinate c) { return m_data[c]; }
		const real& operator[](Coordinate c) const { return m_data[c]; }

	private:
		real m_data[2];
	};

	using texcoord_buffer = buffer_t<texcoord, real, 2, k_texcoord_attribute_name, GL_ARRAY_BUFFER, GL_FLOAT>;

	inline std::ostream& operator << (std::ostream& os, const texcoord& t)
	{
		os << "[" << t[X] << ", " << t[Y] << "]";
		return os;
	}
}
