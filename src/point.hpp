#pragma once

#include <ostream>
#include "types.hpp"
#include "buffer.hpp"
#include "vector.hpp"
#include "opengl.hpp"

namespace engine
{
	class point
	{
	public:
		point(real x = 0.0f, real y = 0.0f, real z = 0.0f)
		: m_data{ x, y, z } {}

		real* data() { return m_data; }
		const real* data() const { return m_data; }

		real& operator[](Coordinate c) { return m_data[c]; }
		const real& operator[](Coordinate c) const { return m_data[c]; }

	private:
		real m_data[3];
	};

	using point_buffer = buffer_t<point, real, 3, k_point_attribute_name, GL_ARRAY_BUFFER, GL_FLOAT>;

	inline static const point ORIGIN = { 0.0f, 0.0f, 0.0f };

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

	inline point operator - (const point& p)
	{
		return point(-p[X], -p[Y], -p[Z]);
	}

	inline point operator * (real r, const point& p)
	{
		return point(r * p[X], r * p[Y], r * p[Z]);
	}

	inline point operator * (const point& p, real r)
	{
		return point(p[X] * r, p[Y] * r, p[Z] * r);
	}

	inline point& operator *= (point& p, real r)
	{
		p = p * r;
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
