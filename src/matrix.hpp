#pragma once

#include <cstring>
#include <ostream>
#include "types.hpp"
#include "vector.hpp"
#include "point.hpp"

namespace engine
{
	class matrix
	{
	public:
		matrix()
		{
			std::memset(m_data, 0, sizeof(m_data));
			m_data[R1C1] = 1.0;
			m_data[R2C2] = 1.0;
			m_data[R3C3] = 1.0;
			m_data[R4C4] = 1.0;
		}

		matrix
		(
			real r1c1, real r1c2, real r1c3,
			real r2c1, real r2c2, real r2c3,
			real r3c1, real r3c2, real r3c3
		) : m_data
		{
			r1c1, r2c1, r3c1, 0.0,
			r1c2, r2c2, r3c2, 0.0,
			r1c3, r2c3, r3c3, 0.0,
			 0.0,  0.0,  0.0, 1.0
		} {}

		matrix
		(
			real r1c1, real r1c2, real r1c3, real r1c4,
			real r2c1, real r2c2, real r2c3, real r2c4,
			real r3c1, real r3c2, real r3c3, real r3c4,
			real r4c1, real r4c2, real r4c3, real r4c4
		) : m_data
		{
			r1c1, r2c1, r3c1, r4c1,
			r1c2, r2c2, r3c2, r4c2,
			r1c3, r2c3, r3c3, r4c3,
			r1c4, r2c4, r3c4, r4c4
		} {}

		matrix(const vector& v) : matrix
		(
			1.0, 0.0, 0.0, v[X],
			0.0, 1.0, 0.0, v[Y],
			0.0, 0.0, 1.0, v[Z],
			0.0, 0.0, 0.0, 1.0
		) {}

		matrix(const point& w) : matrix
		(
			1.0, 0.0, 0.0, w[X],
			0.0, 1.0, 0.0, w[Y],
			0.0, 0.0, 1.0, w[Z],
			0.0, 0.0, 0.0, 1.0
		) {}

		matrix(const vector& x, const vector& y, const vector& z) : matrix
		(
			x[X], y[X], z[X],
			x[Y], y[Y], z[Y],
			x[Z], y[Z], z[Z]
		) {}

		matrix(const vector& x, const vector& y, const vector& z, const point& w) : matrix
		(
			x[X], y[X], z[X], w[X],
			x[Y], y[Y], z[Y], w[Y],
			x[Z], y[Z], z[Z], w[Z],
			 0.0,  0.0,  0.0,  1.0
		) {}

		matrix(real s) : matrix
		(
			  s, 0.0, 0.0,
			0.0,   s, 0.0,
			0.0, 0.0,   s
		) {}

		matrix normal() const
		{
			return matrix
			(
				m_data[R1C1], m_data[R1C2], m_data[R1C3],
				m_data[R2C1], m_data[R2C2], m_data[R2C3],
				m_data[R3C1], m_data[R3C2], m_data[R3C3]
			);
		}

		matrix transpose() const
		{
			return matrix
			(
				m_data[R1C1], m_data[R2C1], m_data[R3C1], m_data[R4C1],
				m_data[R1C2], m_data[R2C2], m_data[R3C2], m_data[R4C2],
				m_data[R1C3], m_data[R2C3], m_data[R3C3], m_data[R4C3],
				m_data[R1C4], m_data[R2C4], m_data[R3C4], m_data[R4C4]
			);
		}

		real* data() { return m_data; }
		const real* data() const { return m_data; }

		real& operator[](MatrixCoordinate c) { return m_data[c]; }
		const real& operator[](MatrixCoordinate c) const { return m_data[c]; }

		inline static const std::size_t k_component_count = 16;
		inline static std::size_t component_count() { return k_component_count; }

	private:
		real m_data[k_component_count];
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
		return matrix
		(
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
			lhs[R4C1] * rhs[R1C4] + lhs[R4C2] * rhs[R2C4] + lhs[R4C3] * rhs[R3C4] + lhs[R4C4] * rhs[R4C4]
		);
	}

	inline matrix& operator *= (matrix& lhs, const matrix& rhs)
	{
		lhs = lhs * rhs;
		return lhs;
	}

	inline vector operator * (const matrix& m, const vector& v)
	{
		return vector
		(
			m[R1C1] * v[X] + m[R1C2] * v[Y] + m[R1C3] * v[Z],
			m[R2C1] * v[X] + m[R2C2] * v[Y] + m[R2C3] * v[Z],
			m[R3C1] * v[X] + m[R3C2] * v[Y] + m[R3C3] * v[Z]
		);
	}

	inline vector& operator *= (vector& v, const matrix& m)
	{
		v = m * v;
		return v;
	}

	inline point operator * (const matrix& m, const point& p)
	{
		return point
		(
			m[R1C1] * p[X] + m[R1C2] * p[Y] + m[R1C3] * p[Z] + m[R1C4],
			m[R2C1] * p[X] + m[R2C2] * p[Y] + m[R2C3] * p[Z] + m[R2C4],
			m[R3C1] * p[X] + m[R3C2] * p[Y] + m[R3C3] * p[Z] + m[R3C4]
		);
	}

	inline point& operator *= (point& p, const matrix& m)
	{
		p = m * p;
		return p;
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
