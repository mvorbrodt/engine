#pragma once

#include <cmath>
#include "types.hpp"

class vec4
{
public:
	vec4(real x = 0.0, real y = 0.0, real z = 0.0, real w = 0.0)
	: m_data{ x, y, z, w } {}

	real length() const { return std::sqrt(lengthSq()); }
	real lengthSq() const { return m_data[X] * m_data[X] + m_data[Y] * m_data[Y] + m_data[Z] * m_data[Z] + m_data[W] * m_data[W]; }

	real* data() { return m_data; }
	const real* data() const { return m_data; }

	real& operator[](VC c) { return m_data[c]; }
	const real& operator[](VC c) const { return m_data[c]; }

private:
	real m_data[4];
};

inline vec4 operator + (const vec4& lhs, const vec4& rhs)
{
	return { lhs[X] + rhs[X], lhs[Y] + rhs[Y], lhs[Z] + rhs[Z], lhs[W] + rhs[W] };
}

inline vec4 operator - (const vec4& lhs, const vec4& rhs)
{
	return { lhs[X] - rhs[X], lhs[Y] - rhs[Y], lhs[Z] - rhs[Z], lhs[W] - rhs[W] };
}

inline vec4 operator * (real r, const vec4& v)
{
	return { r * v[X], r * v[Y], r * v[Z], r * v[W] };
}

inline vec4 operator * (const vec4& v, real r)
{
	return { v[X] * r, v[Y] * r, v[Z] * r, v[W] * r };
}

inline vec4 operator / (const vec4& v, real r)
{
	return { v[X] / r, v[Y] / r, v[Z] / r, v[W] / r };
}

inline real operator * (const vec4& lhs, const vec4& rhs)
{
	return lhs[X] * rhs[X] + lhs[Y] * rhs[Y] + lhs[Z] * rhs[Z] + lhs[W] * rhs[W];
}

inline vec4 operator ^ (const vec4& lhs, const vec4& rhs)
{
	return
	{
		lhs[Y] * rhs[Z] - lhs[Z] * rhs[Y],
		lhs[Z] * rhs[X] - lhs[X] * rhs[Z],
		lhs[X] * rhs[Y] - lhs[Y] * rhs[X],
		0.0
	};
}
