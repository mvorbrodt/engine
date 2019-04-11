#pragma once

#include "types.hpp"

namespace engine
{
	class mat4x4
	{
	public:
		mat4x4
		(
			real r1c1 = 1, real r1c2 = 0, real r1c3 = 0, real r1c4 = 0,
			real r2c1 = 0, real r2c2 = 1, real r2c3 = 0, real r2c4 = 0,
			real r3c1 = 0, real r3c2 = 0, real r3c3 = 1, real r3c4 = 0,
			real r4c1 = 0, real r4c2 = 0, real r4c3 = 0, real r4c4 = 1
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
}
