#pragma once

#include <ostream>
#include "types.hpp"
#include "vector.hpp"

namespace engine
{
	class point
	{
	public:
		point(real x = 0.0, real y = 0.0, real z = 0.0)
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

	inline static const point ORIGIN = { 0.0, 0.0, 0.0 };

	inline point operator + (const point& p, const vector& v)
	{
		return point(p[X] + v[X], p[Y] + v[Y], p[Z] + v[Z]);
	}

	inline point& operator += (point& p, const vector& v)
	{
		p = p + v;
		return p;
	}

	inline point operator - (const point& p, const vector& v)
	{
		return point(p[X] - v[X], p[Y] - v[Y], p[Z] - v[Z]);
	}

	inline point& operator -= (point& p, const vector& v)
	{
		p = p - v;
		return p;
	}

	inline vector operator - (const point& lhs, const point& rhs)
	{
		return vector(lhs[X] - rhs[X], lhs[Y] - rhs[Y], lhs[Z] - rhs[Z]);
	}

	inline std::ostream& operator << (std::ostream& os, const point& p)
	{
		os << "[" << p[X] << ", " << p[Y] << ", " << p[Z] << "]";
		return os;
	}
}
