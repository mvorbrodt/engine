#pragma once

#include <cmath>
#include "types.hpp"
#include "consts.hpp"
#include "vector.hpp"

namespace engine
{
	class quaternion
	{
	public:
		quaternion(real x = 0, real y = 0, real z = 0, real w = 1)
		: m_data{ x, y, z, w } {}

		quaternion(real angle, const vector& axis)
		{
			auto u = axis.normal();
			auto x = u[X];
			auto y = u[Y];
			auto z = u[Z];
			auto s = std::sin(angle * PI / 180) / 2;
			auto c = std::cos(angle * PI / 180) / 2;

			m_data[X] = x * s;
			m_data[Y] = y * s;
			m_data[Z] = z * s;
			m_data[W] = c;
		}

		real length() const
		{
			return std::sqrt(lengthSq());
		}

		real lengthSq() const
		{
			return m_data[X] * m_data[X] + m_data[Y] * m_data[Y] + m_data[Z] * m_data[Z] + m_data[W] * m_data[W];
		}

		quaternion normal() const
		{
			real len = length();
			return { m_data[X] / len, m_data[Y] / len, m_data[Z] / len, m_data[W] / len };
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
		real m_data[4];
	};
}
