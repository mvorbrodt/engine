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
		texcoord(real x = 0.0, real y = 0.0)
		: m_data{ x, y } {}

		real* data() { return m_data; }
		const real* data() const { return m_data; }

		real& operator[](Coordinate c) { return m_data[c]; }
		const real& operator[](Coordinate c) const { return m_data[c]; }

		inline static const std::size_t k_component_count = 2;
		inline static std::size_t component_count() { return k_component_count; }

	private:
		real m_data[k_component_count];
	};

	using texcoord_buffer = buffer_t<texcoord, texcoord::k_component_count, GL_ARRAY_BUFFER, GL_FLOAT>;

	inline std::ostream& operator << (std::ostream& os, const texcoord& t)
	{
		os << "[" << t[X] << ", " << t[Y] << "]";
		return os;
	}
}
