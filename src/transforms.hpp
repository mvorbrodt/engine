#pragma once

#include <cmath>
#include "types.hpp"
#include "consts.hpp"
#include "vector.hpp"
#include "matrix.hpp"

namespace engine
{
	inline mat4x4 translate(const vector& v)
	{
		return
		{
			1, 0, 0, v[X],
			0, 1, 0, v[Y],
			0, 0, 1, v[Z],
			0, 0, 0, 1
		};
	}

	inline mat4x4 rotate(real angle, const vector& axis)
	{
		auto u = axis.normal();
		auto x = u[X];
		auto y = u[Y];
		auto z = u[Z];
		auto s = std::sin(angle * PI / 180);
		auto c = std::cos(angle * PI / 180);

		return
		{
			c + x * x * (1 - c), x * y * (1 - c) - z * s, x * z * (1 - c) + y * s, 0,
			y * x * (1 - c) + z * s, c + y * y * (1 - c), y * z * (1 - c) - x * s, 0,
			z * x * (1 - c) - y * s, z * y * (1 - c) + x * s, c + z * z * (1 - c), 0,
			0,                       0,                       0,                   1
		};
	}

	inline mat4x4 scale(real xs, real ys, real zs)
	{
		return
		{
			xs,  0,   0, 0,
			0,  ys,   0, 0,
			0,   0,  zs, 0,
			0,   0,   0, 1
		};
	}

	inline mat4x4 scale(real s)
	{
		return scale(s, s, s);
	}

	inline mat4x4 projection(real l, real r, real t, real b, real n, real f)
	{
		return
		{
			2 * n / (r - l), 0,               (r + l) / (r - l),                    0,
			0,               2 * n / (t - b), (t + b) / (t - b),                    0,
			0,               0,              -(f + n) / (f - n), -2 * f * n / (f - n),
			0,               0,                              -1,                    0
		};
	}

	inline mat4x4 projection(real fov, real aspect, real near, real far)
	{
		auto top = std::tan(fov * PI / 360) * near;
		auto bottom = -top;
		auto right = top * aspect;
		auto left = -top * aspect;

		return projection(left, right, top, bottom, near, far);
	}
}
