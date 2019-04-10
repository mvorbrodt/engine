#pragma once

#include <ostream>
#include "types.hpp"
#include "vector.hpp"
#include "matrix.hpp"

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

inline std::ostream& operator << (std::ostream& os, const vec4& v)
{
	os << "[" << v[X] << ", " << v[Y] << ", " << v[Z] << ", " << v[W] << "]";
	return os;
}

inline mat4x4 operator * (const mat4x4& lhs, const mat4x4& rhs)
{
	return
	{
		lhs[R1C1] * rhs[R1C1] + lhs[R1C2] * rhs[R2C1] + lhs[R1C3] * rhs[R3C1] + lhs[R1C4] * rhs[R4C1],
		lhs[R1C1] * rhs[R1C2] + lhs[R1C2] * rhs[R2C2] + lhs[R1C3] * rhs[R3C2] + lhs[R1C4] * rhs[R4C2],
		lhs[R1C1] * rhs[R1C3] + lhs[R1C2] * rhs[R2C3] + lhs[R1C3] * rhs[R3C3] + lhs[R1C4] * rhs[R4C3],
		lhs[R1C1] * rhs[R1C4] + lhs[R1C2] * rhs[R2C4] + lhs[R1C3] * rhs[R3C4] + lhs[R1C4] * rhs[R4C4],
		lhs[R2C1] * rhs[R1C1] + lhs[R2C2] * rhs[R2C1] + lhs[R2C3] * rhs[R3C1] + lhs[R2C4] * rhs[R4C1],
		lhs[R2C1] * rhs[R1C2] + lhs[R2C2] * rhs[R2C2] + lhs[R2C3] * rhs[R3C2] + lhs[R2C4] * rhs[R4C2],
		lhs[R2C1] * rhs[R1C3] + lhs[R2C2] * rhs[R2C3] + lhs[R2C3] * rhs[R3C3] + lhs[R2C4] * rhs[R4C3],
		lhs[R2C1] * rhs[R1C4] + lhs[R2C2] * rhs[R2C4] + lhs[R2C3] * rhs[R3C4] + lhs[R2C4] * rhs[R4C4],
		lhs[R3C1] * rhs[R1C1] + lhs[R3C2] * rhs[R2C1] + lhs[R3C3] * rhs[R3C1] + lhs[R3C4] * rhs[R4C1],
		lhs[R3C1] * rhs[R1C2] + lhs[R3C2] * rhs[R2C2] + lhs[R3C3] * rhs[R3C2] + lhs[R3C4] * rhs[R4C2],
		lhs[R3C1] * rhs[R1C3] + lhs[R3C2] * rhs[R2C3] + lhs[R3C3] * rhs[R3C3] + lhs[R3C4] * rhs[R4C3],
		lhs[R3C1] * rhs[R1C4] + lhs[R3C2] * rhs[R2C4] + lhs[R3C3] * rhs[R3C4] + lhs[R3C4] * rhs[R4C4],
		lhs[R4C1] * rhs[R1C1] + lhs[R4C2] * rhs[R2C1] + lhs[R4C3] * rhs[R3C1] + lhs[R4C4] * rhs[R4C1],
		lhs[R4C1] * rhs[R1C2] + lhs[R4C2] * rhs[R2C2] + lhs[R4C3] * rhs[R3C2] + lhs[R4C4] * rhs[R4C2],
		lhs[R4C1] * rhs[R1C3] + lhs[R4C2] * rhs[R2C3] + lhs[R4C3] * rhs[R3C3] + lhs[R4C4] * rhs[R4C3],
		lhs[R4C1] * rhs[R1C4] + lhs[R4C2] * rhs[R2C4] + lhs[R4C3] * rhs[R3C4] + lhs[R4C4] * rhs[R4C4],
	};
}

inline std::ostream& operator << (std::ostream& os, const mat4x4& m)
{
	os << "[" << m[R1C1] << ", " << m[R1C2] << ", " << m[R1C3] << ", " << m[R1C4] << "]\n";
	os << "[" << m[R2C1] << ", " << m[R2C2] << ", " << m[R2C3] << ", " << m[R2C4] << "]\n";
	os << "[" << m[R3C1] << ", " << m[R3C2] << ", " << m[R3C3] << ", " << m[R3C4] << "]\n";
	os << "[" << m[R4C1] << ", " << m[R4C2] << ", " << m[R4C3] << ", " << m[R4C4] << "]\n";
	return os;
}

inline vec4 operator * (const mat4x4& m, const vec4& v)
{
	return
	{
		m[R1C1] * v[X] + m[R1C2] * v[Y] + m[R1C3] * v[Z] + m[R1C4] * v[W],
		m[R2C1] * v[X] + m[R2C2] * v[Y] + m[R2C3] * v[Z] + m[R2C4] * v[W],
		m[R3C1] * v[X] + m[R3C2] * v[Y] + m[R3C3] * v[Z] + m[R3C4] * v[W],
		m[R4C1] * v[X] + m[R4C2] * v[Y] + m[R4C3] * v[Z] + m[R4C4] * v[W]
	};
}
