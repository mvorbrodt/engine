#pragma once

#include <ostream>
#include "types.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "transforms.hpp"

namespace engine
{
	class axis
	{
	public:
		axis(const vector& x = UNIT_X, const vector& y = UNIT_Y, const vector& z = UNIT_Z)
		: m_axis{ x, y, z }
		{
			m_axis[X].normalize();
			m_axis[Y].normalize();
			m_axis[Z].normalize();
		}

		void rotate(real angle, const vector& axis)
		{
			auto m = engine::rotate(angle, axis);
			m_axis[X] *= m;
			m_axis[Y] *= m;
			m_axis[Z] *= m;
		}

		void rotate(const quaternion& q)
		{
			auto m = engine::rotate(q);
			m_axis[X] *= m;
			m_axis[Y] *= m;
			m_axis[Z] *= m;
		}

		matrix to_local() const
		{
			return matrix
			(
				m_axis[X][X], m_axis[X][Y], m_axis[X][Z], 0.0,
				m_axis[Y][X], m_axis[Y][Y], m_axis[Y][Z], 0.0,
				m_axis[Z][X], m_axis[Z][Y], m_axis[Z][Z], 0.0,
				0.0, 0.0, 0.0, 1.0
			);
		}

		matrix to_global() const
		{
			return matrix
			(
				m_axis[X][X], m_axis[Y][X], m_axis[Z][X], 0.0,
				m_axis[X][Y], m_axis[Y][Y], m_axis[Z][Y], 0.0,
				m_axis[X][Z], m_axis[Y][Z], m_axis[Z][Z], 0.0,
				0.0, 0.0, 0.0, 1.0
			);
		}

		vector& operator[](Coordinate c) { return m_axis[c]; }
		const vector& operator[](Coordinate c) const { return m_axis[c]; }

	private:
		vector m_axis[3];
	};

	inline static const axis IDENTITY_AXIS = { UNIT_X, UNIT_Y, UNIT_Z };

	inline axis operator * (const matrix& m, const axis& a)
	{
		return axis(m * a[X], m * a[Y], m * a[Z]);
	}

	inline axis& operator *= (axis& a, const matrix& m)
	{
		a = m * a;
		return a;
	}
}
