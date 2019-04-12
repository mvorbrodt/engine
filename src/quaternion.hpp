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
		quaternion(real x = 0.0, real y = 0.0, real z = 0.0, real w = 1.0)
		: m_data{ x, y, z, w } {}

		quaternion(real angle, const vector& axis)
		{
			auto u = axis.normal();
			auto x = u[X];
			auto y = u[Y];
			auto z = u[Z];
			auto s = std::sin(angle * PI / (real)360);
			auto c = std::cos(angle * PI / (real)360);

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
			real len = 1.0 / length();
			return { m_data[X] * len, m_data[Y] * len, m_data[Z] * len, m_data[W] * len };
		}

		void normalize()
		{
			real len = 1.0 / length();
			m_data[X] *= len;
			m_data[Y] *= len;
			m_data[Z] *= len;
			m_data[W] *= len;
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

	inline static const quaternion IDENTITY_QUATERNION = { 0.0, 0.0, 0.0, 1.0 };

	inline quaternion operator + (const quaternion& q, const quaternion& r)
	{
		vector qv{ q[X], q[Y], q[Z] };
		vector rv{ r[X], r[Y], r[Z] };
		auto qw = q[W];
		auto rw = r[W];
		auto ri = qv + rv;
		auto rr = qw + rw;

		return { ri[X], ri[Y], ri[Z], rr };
	}

	inline quaternion& operator += (quaternion& q, const quaternion& r)
	{
		q = q + r;
		return q;
	}

	inline quaternion operator - (const quaternion& q)
	{
		return { -q[X], -q[Y], -q[Z], q[W] };
	}

	inline quaternion operator * (real s, const quaternion& q)
	{
		return { s * q[X], s * q[Y], s * q[Z], s * q[W] };
	}

	inline quaternion operator * (const quaternion& q, real s)
	{
		return { q[X] * s, q[Y] * s, q[Z] * s, q[W] * s };
	}

	inline quaternion operator * (const quaternion& q, const quaternion& r)
	{
		vector qv{ q[X], q[Y], q[Z] };
		vector rv{ r[X], r[Y], r[Z] };
		auto qw = q[W];
		auto rw = r[W];
		auto ri = (qv ^ rv) + (rw * qv) + (qw * rv);
		auto rr = (qw * rw) - (qv * rv);

		return { ri[X], ri[Y], ri[Z], rr };
	}

	inline quaternion& operator *= (quaternion& q, const quaternion& r)
	{
		q = q * r;
		return q;
	}

	inline std::ostream& operator << (std::ostream& os, const quaternion& q)
	{
		os << "[" << q[X] << ", " << q[Y] << ", " << q[Z] << ", " << q[W] << "]";
		return os;
	}
}
