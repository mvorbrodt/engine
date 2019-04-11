#pragma once

#include "types.hpp"

namespace engine
{
	class point
	{
	public:
		point(real x = 0, real y = 0, real z = 0)
		: m_data{ x, y, z } {}

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
