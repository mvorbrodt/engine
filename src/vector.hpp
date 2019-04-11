#pragma once

#include <cmath>
#include "types.hpp"

namespace engine
{
	class vector
	{
	public:
		vector(real x = 0, real y = 0, real z = 0)
		: m_data{ x, y, z } {}

		real length() const
		{
			return std::sqrt(lengthSq());
		}

		real lengthSq() const
		{
			return m_data[X] * m_data[X] + m_data[Y] * m_data[Y] + m_data[Z] * m_data[Z];
		}

		vector normal() const
		{
			real len = length();
			return { m_data[X] / len, m_data[Y] / len, m_data[Z] / len };
		}

		real* data()
		{
			return m_data;
		}

		const real* data() const
		{
			return m_data;
		}

		real& operator[](VC c)
		{
			return m_data[c];
		}

		const real& operator[](VC c) const
		{
			return m_data[c];
		}

	private:
		real m_data[3];
	};
}
