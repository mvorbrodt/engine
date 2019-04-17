#pragma once

#include <vector>
#include <ostream>

namespace engine
{
	class texcoord
	{
	public:
		texcoord(real x = 0.0, real y = 0.0)
		: m_data{ x, y } {}

		real* data() { return m_data; }
		const real* data() const { return m_data; }

		real& operator[](VC c) { return m_data[c]; }
		const real& operator[](VC c) const { return m_data[c]; }

	private:
		real m_data[2];
	};

	using texcoord_buffer = std::vector<texcoord>;

	inline std::ostream& operator << (std::ostream& os, const texcoord& t)
	{
		os << "[" << t[X] << ", " << t[Y] << "]";
		return os;
	}
}
