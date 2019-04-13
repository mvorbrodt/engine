#pragma once

#include <cmath>
#include "types.hpp"
#include "consts.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "quaternion.hpp"
#include "matrix.hpp"

namespace engine
{
	inline real degrees_to_radians(real degrees)
	{
		return degrees * PI / 180.0;
	}

	inline real radians_to_degrees(real radians)
	{
		return radians * 180.0 / PI;
	}

	template<typename T>
	T interpolate(real i, const T& s, const T& e)
	{
		return (1.0 - i) * s + (e * i);
	}

	inline matrix translate(const vector& v)
	{
		return matrix
		(
			1.0, 0.0, 0.0, v[X],
			0.0, 1.0, 0.0, v[Y],
			0.0, 0.0, 1.0, v[Z],
			0.0, 0.0, 0.0, 1.0
		);
	}

	inline matrix rotate(real angle, const vector& axis)
	{
		auto u = axis.normal();
		auto x = u[X];
		auto y = u[Y];
		auto z = u[Z];
		auto s = std::sin(degrees_to_radians(angle));
		auto c = std::cos(degrees_to_radians(angle));

		return matrix
		(
			c + x * x * (1.0 - c), x * y * (1.0 - c) - z * s, x * z * (1.0 - c) + y * s, 0.0,
			y * x * (1.0 - c) + z * s, c + y * y * (1.0 - c), y * z * (1.0 - c) - x * s, 0.0,
			z * x * (1.0 - c) - y * s, z * y * (1.0 - c) + x * s, c + z * z * (1.0 - c), 0.0,
			0.0, 0.0, 0.0, 1.0
		);
	}

	inline matrix rotate(const quaternion& q)
	{
		auto n = q.normal();
		auto x = n[X];
		auto y = n[Y];
		auto z = n[Z];
		auto w = n[W];

		return matrix
		(
			1.0 - 2.0 * (y * y + z * z), 2.0 * (x * y - w * z), 2.0 * (x * z + w * y), 0.0,
			2.0 * (x * y + w * z), 1.0 - 2.0 * (x * x + z * z), 2.0 * (y * z - w * x), 0.0,
			2.0 * (x * z - w * y), 2.0 * (y * z + w * x), 1.0 - 2.0 * (x * x + y * y), 0.0,
			0.0, 0.0, 0.0, 1.0
		);
	}

	inline matrix scale(real xs, real ys, real zs)
	{
		return matrix
		(
			 xs, 0.0, 0.0, 0.0,
			0.0,  ys, 0.0, 0.0,
			0.0, 0.0,  zs, 0.0,
			0.0, 0.0, 0.0, 1.0
		);
	}

	inline matrix scale(real s)
	{
		return scale(s, s, s);
	}

	inline matrix projection(real l, real r, real t, real b, real n, real f)
	{
		return matrix
		(
			2.0 * n / (r - l), 0.0, (r + l) / (r - l), 0.0,
			0.0, 2.0 * n / (t - b), (t + b) / (t - b), 0.0,
			0.0, 0.0, -(f + n) / (f - n), -2.0 * f * n / (f - n),
			0.0, 0.0, -1.0, 0.0
		);
	}

	inline matrix projection(real fov, real aspect, real near, real far)
	{
		auto top = std::tan(degrees_to_radians(fov) / 2.0) * near;
		auto bottom = -top;
		auto right = top * aspect;
		auto left = -top * aspect;

		return projection(left, right, top, bottom, near, far);
	}

	inline matrix look_at(const point& eye, const point& at, const vector& up)
	{
		auto z = (eye - at).normal();
		auto x = (up ^ z).normal();
		auto y = (z ^ x).normal();

		matrix r =
		{
			x[X], x[Y], x[Z], 0.0,
			y[X], y[Y], y[Z], 0.0,
			z[X], z[Y], z[Z], 0.0,
			 0.0,  0.0,  0.0, 1.0
		};

		matrix t =
		{
			1.0, 0.0, 0.0, -eye[X],
			0.0, 1.0, 0.0, -eye[Y],
			0.0, 0.0, 1.0, -eye[Z],
			0.0, 0.0, 0.0, 1.0
		};

		return r * t;
	}
}
