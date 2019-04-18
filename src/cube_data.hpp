#pragma once

#include "buffer.hpp"
#include "point.hpp"
#include "color.hpp"
#include "texcoord.hpp"

namespace engine
{
	point_buffer cube_vertices =
	{
		{ -0.5, -0.5, -0.5 },
		{  0.5, -0.5, -0.5 },
		{  0.5,  0.5, -0.5 },
		{  0.5,  0.5, -0.5 },
		{ -0.5,  0.5, -0.5 },
		{ -0.5, -0.5, -0.5 },

		{ -0.5, -0.5,  0.5 },
		{  0.5, -0.5,  0.5 },
		{  0.5,  0.5,  0.5 },
		{  0.5,  0.5,  0.5 },
		{ -0.5,  0.5,  0.5 },
		{ -0.5, -0.5,  0.5 },

		{ -0.5,  0.5,  0.5 },
		{ -0.5,  0.5, -0.5 },
		{ -0.5, -0.5, -0.5 },
		{ -0.5, -0.5, -0.5 },
		{ -0.5, -0.5,  0.5 },
		{ -0.5,  0.5,  0.5 },

		{  0.5,  0.5,  0.5 },
		{  0.5,  0.5, -0.5 },
		{  0.5, -0.5, -0.5 },
		{  0.5, -0.5, -0.5 },
		{  0.5, -0.5,  0.5 },
		{  0.5,  0.5,  0.5 },

		{ -0.5, -0.5, -0.5 },
		{  0.5, -0.5, -0.5 },
		{  0.5, -0.5,  0.5 },
		{  0.5, -0.5,  0.5 },
		{ -0.5, -0.5,  0.5 },
		{ -0.5, -0.5, -0.5 },

		{ -0.5,  0.5, -0.5 },
		{  0.5,  0.5, -0.5 },
		{  0.5,  0.5,  0.5 },
		{  0.5,  0.5,  0.5 },
		{ -0.5,  0.5,  0.5 },
		{ -0.5,  0.5, -0.5 }
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
		{ 0.0, 0.0 },
		{ 1.0, 0.0 },
		{ 1.0, 1.0 },
		{ 1.0, 1.0 },
		{ 0.0, 1.0 },
		{ 0.0, 0.0 },

		{ 0.0, 0.0 },
		{ 1.0, 0.0 },
		{ 1.0, 1.0 },
		{ 1.0, 1.0 },
		{ 0.0, 1.0 },
		{ 0.0, 0.0 },

		{ 1.0, 0.0 },
		{ 1.0, 1.0 },
		{ 0.0, 1.0 },
		{ 0.0, 1.0 },
		{ 0.0, 0.0 },
		{ 1.0, 0.0 },

		{ 1.0, 0.0 },
		{ 1.0, 1.0 },
		{ 0.0, 1.0 },
		{ 0.0, 1.0 },
		{ 0.0, 0.0 },
		{ 1.0, 0.0 },

		{ 0.0, 1.0 },
		{ 1.0, 1.0 },
		{ 1.0, 0.0 },
		{ 1.0, 0.0 },
		{ 0.0, 0.0 },
		{ 0.0, 1.0 },

		{ 0.0, 1.0 },
		{ 1.0, 1.0 },
		{ 1.0, 0.0 },
		{ 1.0, 0.0 },
		{ 0.0, 0.0 },
		{ 0.0, 1.0 }
	};

	point_buffer cube_positions[] =
	{
		{  0.0,  0.0,   0.0 },
		{  2.0,  5.0, -15.0 },
		{ -1.5, -2.2,  -2.5 },
		{ -3.8, -2.0, -12.3 },
		{  2.4, -0.4,  -3.5 },
		{ -1.7,  3.0,  -7.5 },
		{  1.3, -2.0,  -2.5 },
		{  1.5,  2.0,  -2.5 },
		{  1.5,  0.2,  -1.5 },
		{ -1.3,  1.0,  -1.5 }
	};
}
