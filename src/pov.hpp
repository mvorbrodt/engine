#pragma once

#include <cassert>
#include "types.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "transforms.hpp"

#define MIN_FOV 10.0f
#define MAX_FOV 90.0f

namespace engine
{
	class pov
	{
	public:
		pov(int width, int height, real fov = 60.0f, real _near = 1.0f, real _far = 100.0f, const point& origin = ORIGIN, const vector& direction = -UNIT_Z, const vector& up = UNIT_Y)
		: m_width{ width }, m_height{ height }, m_fov{ fov }, m_near{ _near }, m_far{ _far }, m_origin{ origin }, m_direction{ direction }, m_up{ up }
		{
			assert(m_width > 1 && m_height > 1);
			assert(m_fov >= MIN_FOV && m_fov <= MAX_FOV);
			assert(m_near >= 1.0f && m_far >= 1.0f && m_near < m_far);
			m_direction.normalize();
			m_up.normalize();
		}

		int get_width() const { return m_width; }
		void set_width(int width)
		{
			if(width < 1) width = 1;
			m_width = width;
		}

		int get_height() const { return m_height; }
		void set_height(int height)
		{
			if(height < 1) height = 1;
			m_height = height;
		}

		real get_fov() const { return m_fov; }
		void set_fov(real fov)
		{
			if(fov < MIN_FOV) fov = MIN_FOV;
			if(fov > MAX_FOV) fov = MAX_FOV;
			m_fov = fov;
		}

		real get_near() const { return m_near; }
		void set_near(real _near)
		{
			if(_near < 1.0f) _near = 1.0f;
			if(_near > m_far) _near = m_far - 1.0f;
			m_near = _near;
		}

		real get_far() const { return m_far; }
		void set_far(real _far)
		{
			if(_far < 1.0f) _far = 1.0f;
			if(_far < m_near) _far = m_near + 1.0f;
			m_far = _far;
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
