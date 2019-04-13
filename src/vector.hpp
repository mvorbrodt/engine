#pragma once

#include <ostream>
#include <cmath>
#include "types.hpp"

namespace engine
{
	class vector
	{
	public:
		vector(real x = 0.0, real y = 0.0, real z = 0.0)
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
			real len = 1.0 / length();
			return vector(m_data[X] * len, m_data[Y] * len, m_data[Z] * len);
		}

		void normalize()
		{
			real len = 1.0 / length();
			m_data[X] *= len;
			m_data[Y] *= len;
			m_data[Z] *= len;
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

	inline static const vector UNIT_X = { 1.0, 0.0, 0.0 };
	inline static const vector UNIT_Y = { 0.0, 1.0, 0.0 };
	inline static const vector UNIT_Z = { 0.0, 0.0, 1.0 };

	inline vector operator + (const vector& lhs, const vector& rhs)
	{
		return vector(lhs[X] + rhs[X], lhs[Y] + rhs[Y], lhs[Z] + rhs[Z]);
	}

	inline vector& operator += (vector& lhs, const vector& rhs)
	{
		lhs = lhs + rhs;
		return lhs;
	}

	inline vector operator - (const vector& lhs, const vector& rhs)
	{
		return vector(lhs[X] - rhs[X], lhs[Y] - rhs[Y], lhs[Z] - rhs[Z]);
	}

	inline vector& operator -= (vector& lhs, const vector& rhs)
	{
		lhs = lhs - rhs;
		return lhs;
	}

	inline vector operator - (const vector& v)
	{
		return vector(-v[X], -v[Y], -v[Z]);
	}

	inline vector operator * (real r, const vector& v)
	{
		return vector(r * v[X], r * v[Y], r * v[Z]);
	}

	inline vector operator * (const vector& v, real r)
	{
		return vector(v[X] * r, v[Y] * r, v[Z] * r);
	}

	inline vector& operator *= (vector& v, real r)
	{
		v = v * r;
		return v;
	}

	inline vector operator / (const vector& v, real r)
	{
		return vector(v[X] / r, v[Y] / r, v[Z] / r);
	}

	inline vector& operator /= (vector& v, real r)
	{
		v = v / r;
		return v;
	}

	inline real operator * (const vector& lhs, const vector& rhs)
	{
		return lhs[X] * rhs[X] + lhs[Y] * rhs[Y] + lhs[Z] * rhs[Z];
	}

	inline vector operator ^ (const vector& lhs, const vector& rhs)
	{
		return vector
		(
			lhs[Y] * rhs[Z] - lhs[Z] * rhs[Y],
			lhs[Z] * rhs[X] - lhs[X] * rhs[Z],
			lhs[X] * rhs[Y] - lhs[Y] * rhs[X]
		);
	}

	inline std::ostream& operator << (std::ostream& os, const vector& v)
	{
		os << "[" << v[X] << ", " << v[Y] << ", " << v[Z] << "]";
		return os;
	}
}
