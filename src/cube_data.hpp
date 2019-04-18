#pragma once

#include "types.hpp"
#include "point.hpp"
#include "color.hpp"
#include "texcoord.hpp"

namespace engine
{
	point_buffer cube_vertices =
	{
		{ -0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },
		{  0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f, -0.5f },
		{ -0.5f,  0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f },

		{ -0.5f, -0.5f,  0.5f },
		{  0.5f, -0.5f,  0.5f },
		{  0.5f,  0.5f,  0.5f },
		{  0.5f,  0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },
		{ -0.5f, -0.5f,  0.5f },

		{ -0.5f,  0.5f,  0.5f },
		{ -0.5f,  0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f, -0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },

		{  0.5f,  0.5f,  0.5f },
		{  0.5f,  0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f,  0.5f },
		{  0.5f,  0.5f,  0.5f },

		{ -0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f,  0.5f },
		{  0.5f, -0.5f,  0.5f },
		{ -0.5f, -0.5f,  0.5f },
		{ -0.5f, -0.5f, -0.5f },

		{ -0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f,  0.5f },
		{  0.5f,  0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },
		{ -0.5f,  0.5f, -0.5f }
	};

	color_buffer cube_colors =
	{
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },

		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },

		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },

		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },

		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },

		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 }
	};

	texcoord_buffer cube_texcoords =
	{
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },

		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },

		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },

		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },

		{ 0.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 0.0f },
		{ 0.0f, 1.0f },

		{ 0.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 0.0f },
		{ 0.0f, 1.0f }
	};

	int_buffer cube_indices =
	{
		0, 1, 2,
		3, 4, 5,

		6, 7, 8,
		9, 10, 11,

		12, 13, 14,
		15, 16, 17,

		18, 19, 20,
		21, 22, 23,

		24, 25, 26,
		27, 28, 29,

		30, 31, 32,
		33, 34, 35
	};
}