#pragma once

#include <cassert>
#include "types.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "transforms.hpp"

#define MIN_FOV 10.0
#define MAX_FOV 90.0

namespace engine
{
	class pov
	{
	public:
		pov(int w, int h, real fov = 60.0, real near = 1.0, real far = 100.0, const point& origin = ORIGIN, const vector& direction = -UNIT_Z, const vector& up = UNIT_Y)
		: m_width{ w }, m_height{ h }, m_fov{ fov }, m_near{ near }, m_far{ far }, m_origin{ origin }, m_direction{ direction }, m_up{ up }
		{
			assert(m_width > 1 && m_height > 1);
			assert(m_fov >= MIN_FOV && m_fov <= MAX_FOV);
			assert(m_near >= 1.0 && m_far >= 1.0 && m_near < m_far);
			m_direction.normalize();
			m_up.normalize();
		}

		real get_fov() const
		{
			return m_fov;
		}

		void set_fov(real fov)
		{
			if(fov < MIN_FOV) fov = MIN_FOV;
			if(fov > MAX_FOV) fov = MAX_FOV;
			m_fov = fov;
		}

		int get_width() const
		{
			return m_width;
		}

		void set_width(int w)
		{
			if(w < 1) w = 1;
			m_width = w;
		}

		int get_height() const
		{
			return m_height;
		}

		void set_height(int h)
		{
			if(h < 1) h = 1;
			m_height = h;
		}

		void move(real forward, real side)
		{
			auto v = (m_up ^ m_direction).normal();
			m_origin += forward * m_direction + side * v;
		}

		void turn(real angle)
		{
			auto m = engine::rotate(angle, m_up);
			m_direction *= m;
		}

		void look(real angle)
		{
			auto side = (m_up ^ m_direction).normal();
			auto m = engine::rotate(angle, side);
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

		matrix projection_matrix()
		{
			return projection(m_fov, (real)m_width / (real)m_height, m_near, m_far);
		}

	private:
		int m_width;
		int m_height;
		real m_fov;
		real m_near;
		real m_far;
		point m_origin;
		vector m_direction;
		vector m_up;
	};
}
