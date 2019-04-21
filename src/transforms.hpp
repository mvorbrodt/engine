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
	template<typename T>
	T interpolate(real i, const T& s, const T& e)
	{
		return (1.0f - i) * s + (e * i);
	}

	inline matrix translate(const vector& v)
	{
		return matrix(v);
	}

	inline matrix rotate(real angle, const vector& axis)
	{
		auto u = axis.normal();
		auto x = u[X];
		auto y = u[Y];
		auto z = u[Z];
		auto s = std::sin(angle * PI / 180.0f);
		auto c = std::cos(angle * PI / 180.0f);

		return matrix
		(
			c + x * x * (1.0f - c), x * y * (1.0f - c) - z * s, x * z * (1.0f - c) + y * s,
			y * x * (1.0f - c) + z * s, c + y * y * (1.0f - c), y * z * (1.0f - c) - x * s,
			z * x * (1.0f - c) - y * s, z * y * (1.0f - c) + x * s, c + z * z * (1.0f - c)
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
			1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y - w * z), 2.0f * (x * z + w * y),
			2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z - w * x),
			2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 1.0f - 2.0f * (x * x + y * y)
		);
	}

	inline matrix scale(real s)
	{
		return matrix(s);
	}

	inline matrix scale(real sx, real sy, real sz)
	{
		return matrix(sx, sy, sz);
	}

	inline matrix projection(real l, real r, real t, real b, real n, real f)
	{
		return matrix
		(
			2.0f * n / (r - l), 0.0f, (r + l) / (r - l), 0.0f,
			0.0f, 2.0f * n / (t - b), (t + b) / (t - b), 0.0f,
			0.0f, 0.0f, -(f + n) / (f - n), -2.0f * f * n / (f - n),
			0.0f, 0.0f, -1.0f, 0.0f
		);
	}

	inline matrix projection(real fov, real aspect, real _near, real _far)
	{
		auto top = std::tan(fov * PI / 360.0f) * _near;
		auto bottom = -top;
		auto right = top * aspect;
		auto left = -top * aspect;

		return projection(left, right, top, bottom, _near, _far);
	}

	inline matrix look_at(const point& eye, const point& at, const vector& up)
	{
		auto z = (eye - at).normal();
		auto x = (up ^ z).normal();
		auto y = (z ^ x).normal();

		matrix r = matrix(x, y, z).transpose();
		matrix t = matrix(-eye);

		return r * t;
	}
}
