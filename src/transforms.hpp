#pragma once

#include <cmath>
#include "types.hpp"
#include "consts.hpp"
#include "vector.hpp"
#include "quaternion.hpp"
#include "matrix.hpp"

namespace engine
{
	inline mat4x4 translate(const vector& v)
	{
		return
		{
			1.0, 0.0, 0.0, v[X],
			0.0, 1.0, 0.0, v[Y],
			0.0, 0.0, 1.0, v[Z],
			0.0, 0.0, 0.0, 1.0
		};
	}

	inline mat4x4 rotate(real angle, const vector& axis)
	{
		auto u = axis.normal();
		auto x = u[X];
		auto y = u[Y];
		auto z = u[Z];
		auto s = std::sin(angle * PI / (real)180);
		auto c = std::cos(angle * PI / (real)180);

		return mat4x4
		(
			c + x * x * (1.0 - c), x * y * (1.0 - c) - z * s, x * z * (1.0 - c) + y * s, 0.0,
			y * x * (1.0 - c) + z * s, c + y * y * (1.0 - c), y * z * (1.0 - c) - x * s, 0.0,
			z * x * (1.0 - c) - y * s, z * y * (1.0 - c) + x * s, c + z * z * (1.0 - c), 0.0,
			0.0, 0.0, 0.0, 1.0
		);
	}

	inline mat4x4 rotate(const quaternion& q)
	{
		auto n = q.normal();
		auto x = n[X];
		auto y = n[Y];
		auto z = n[Z];
		auto w = n[W];

		return mat4x4
		(
			1.0 - 2.0 * (y * y + z * z), 2.0 * (x * y - w * z), 2.0 * (x * z + w * y), 0.0,
			2.0 * (x * y + w * z), 1.0 - 2.0 * (x * x + z * z), 2.0 * (y * z - w * x), 0.0,
			2.0 * (x * z - w * y), 2.0 * (y * z + w * x), 1.0 - 2.0 * (x * x + y * y), 0.0,
			0.0, 0.0, 0.0, 1.0
		);
	}

	inline mat4x4 scale(real xs, real ys, real zs)
	{
		return mat4x4
		(
			 xs, 0.0, 0.0, 0.0,
			0.0,  ys, 0.0, 0.0,
			0.0, 0.0,  zs, 0.0,
			0.0, 0.0, 0.0, 1.0
		);
	}

	inline mat4x4 scale(real s)
	{
		return scale(s, s, s);
	}

	inline mat4x4 projection(real l, real r, real t, real b, real n, real f)
	{
		return mat4x4
		(
			2.0 * n / (r - l), 0.0, (r + l) / (r - l), 0.0,
			0.0, 2.0 * n / (t - b), (t + b) / (t - b), 0.0,
			0.0, 0.0, -(f + n) / (f - n), -2.0 * f * n / (f - n),
			0.0, 0.0, -1.0, 0.0
		);
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
