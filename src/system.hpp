#pragma once

#include <ostream>
#include "types.hpp"
#include "point.hpp"
#include "matrix.hpp"
#include "axis.hpp"
#include "transforms.hpp"

namespace engine
{
	class system
	{
	public:
		system(const axis& a = IDENTITY_AXIS, const point& o = ORIGIN)
		: m_axis{ a }, m_origin{ o } {}

		matrix to_local() const
		{
			auto r = m_axis.to_local();
			auto t = translate({ -m_origin[X], -m_origin[Y], -m_origin[Z] });
			return r * t;
		}

		matrix to_global() const
		{
			auto r = m_axis.to_global();
			auto t = translate({ m_origin[X], m_origin[Y], m_origin[Z] });
			return t * r;
		}

		class axis& axis()
		{
			return m_axis;
		}

		const class axis& axis() const
		{
			return m_axis;
		}

		point& origin()
		{
			return m_origin;
		}

		const point& origin() const
		{
			return m_origin;
		}

	private:
		class axis m_axis;
		point m_origin;
	};

	inline static const system IDENTITY_SYSTEM = { IDENTITY_AXIS, ORIGIN };

	inline system operator * (const matrix& m, const system& s)
	{
		return { m * s.axis(), m * s.origin() };
	}

	inline system operator *= (system& s, const matrix& m)
	{
		s = m * s;
		return s;
	}
}
