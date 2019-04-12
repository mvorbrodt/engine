#pragma once

#include <ostream>
#include "types.hpp"
#include "vector.hpp"
#include "point.hpp"

namespace engine
{
	class matrix
	{
	public:
		matrix
		(
			real r1c1 = 1.0, real r1c2 = 0.0, real r1c3 = 0.0, real r1c4 = 0.0,
			real r2c1 = 0.0, real r2c2 = 1.0, real r2c3 = 0.0, real r2c4 = 0.0,
			real r3c1 = 0.0, real r3c2 = 0.0, real r3c3 = 1.0, real r3c4 = 0.0,
			real r4c1 = 0.0, real r4c2 = 0.0, real r4c3 = 0.0, real r4c4 = 1.0
		) : m_data
		{
			r1c1, r2c1, r3c1, r4c1,
			r1c2, r2c2, r3c2, r4c2,
			r1c3, r2c3, r3c3, r4c3,
			r1c4, r2c4, r3c4, r4c4
		} {}

		real* data()
		{
			return m_data;
		}

		const real* data() const
		{
			return m_data;
		}

		real& operator[](MC c)
		{
			return m_data[c];
		}

		const real& operator[](MC c) const
		{
			return m_data[c];
		}

	private:
		real m_data[16];
	};

	inline static const matrix IDENTITY_MATRIX =
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	inline matrix operator * (const matrix& lhs, const matrix& rhs)
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

	inline vector operator * (const matrix& m, const vector& v)
	{
		return
		{
			m[R1C1] * v[X] + m[R1C2] * v[Y] + m[R1C3] * v[Z],
			m[R2C1] * v[X] + m[R2C2] * v[Y] + m[R2C3] * v[Z],
			m[R3C1] * v[X] + m[R3C2] * v[Y] + m[R3C3] * v[Z]
		};
	}

	inline point operator * (const matrix& m, const point& p)
	{
		return
		{
			m[R1C1] * p[X] + m[R1C2] * p[Y] + m[R1C3] * p[Z] + m[R1C4],
			m[R2C1] * p[X] + m[R2C2] * p[Y] + m[R2C3] * p[Z] + m[R2C4],
			m[R3C1] * p[X] + m[R3C2] * p[Y] + m[R3C3] * p[Z] + m[R3C4]
		};
	}

	inline std::ostream& operator << (std::ostream& os, const matrix& m)
	{
		os << "[" << m[R1C1] << ", " << m[R1C2] << ", " << m[R1C3] << ", " << m[R1C4] << "]\n";
		os << "[" << m[R2C1] << ", " << m[R2C2] << ", " << m[R2C3] << ", " << m[R2C4] << "]\n";
		os << "[" << m[R3C1] << ", " << m[R3C2] << ", " << m[R3C3] << ", " << m[R3C4] << "]\n";
		os << "[" << m[R4C1] << ", " << m[R4C2] << ", " << m[R4C3] << ", " << m[R4C4] << "]\n";
		return os;
	}
}
