#pragma once

#include <ostream>
#include <cmath>
#include "types.hpp"
#include "buffer.hpp"
#include "opengl.hpp"

namespace engine
{
	class vector
	{
	public:
		vector(real x = 0.0f, real y = 0.0f, real z = 0.0f)
		: m_data{ x, y, z } {}

		real length() const { return std::sqrt(lengthSq()); }
		real lengthSq() const
		{
			return m_data[X] * m_data[X] + m_data[Y] * m_data[Y] + m_data[Z] * m_data[Z];
		}

		vector normal() const
		{
			real len = 1.0f / length();
			return vector(m_data[X] * len, m_data[Y] * len, m_data[Z] * len);
		}

		void normalize()
		{
			real len = 1.0f / length();
			m_data[X] *= len;
			m_data[Y] *= len;
			m_data[Z] *= len;
		}

		real* data() { return m_data; }
		const real* data() const { return m_data; }

		real& operator[](Coordinate c) { return m_data[c]; }
		const real& operator[](Coordinate c) const { return m_data[c]; }

	private:
		real m_data[3];
	};

	using normal_buffer = buffer_t<vector, real, 3, k_normal_attribute_name, GL_ARRAY_BUFFER, GL_FLOAT>;
	using tangent_buffer = buffer_t<vector, real, 3, k_tangent_attribute_name, GL_ARRAY_BUFFER, GL_FLOAT>;
	using bitangent_buffer = buffer_t<vector, real, 3, k_bitangent_attribute_name, GL_ARRAY_BUFFER, GL_FLOAT>;

	inline static const vector UNIT_X = { 1.0f, 0.0f, 0.0f };
	inline static const vector UNIT_Y = { 0.0f, 1.0f, 0.0f };
	inline static const vector UNIT_Z = { 0.0f, 0.0f, 1.0f };

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
