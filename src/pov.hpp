#pragma once

#include "types.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "transforms.hpp"

namespace engine
{
	class pov
	{
	public:
		pov(const point& origin = ORIGIN, const vector& direction = -UNIT_Z, const vector& up = UNIT_Y)
		: m_origin{ origin }, m_direction{ direction }, m_up{ up }
		{
			m_direction.normalize();
			m_up.normalize();
		}

		void move(real forward, real size)
		{
			auto v = (m_up ^ m_direction).normal();
			m_origin += forward * m_direction + size * v;
		}

		void turn(real angle)
		{
			auto m = engine::rotate(angle, m_up);
			m_direction *= m;
		}

		void look(real angle)
		{
			auto size = (m_up ^ m_direction).normal();
			auto m = engine::rotate(angle, size);
			m_direction *= m;
		}

		void roll(real angle)
		{
			auto m = engine::rotate(angle, m_direction);
			m_up *= m;
		}

		matrix view_matrix() const
		{
			auto at = m_origin + m_direction;
			return look_at(m_origin, at, m_up);
		}

	private:
		point m_origin;
		vector m_direction;
		vector m_up;
	};
}
