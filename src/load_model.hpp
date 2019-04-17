#pragma once

#include <tuple>
#include <memory>
#include "types.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "texcoord.hpp"

namespace engine
{
	// <vertex, normal, tangent, texture, index>
	using model_data = std::tuple<point_buffer, vector_buffer, vector_buffer, texcoord_buffer, int_buffer>;
	using model_data_ptr = std::shared_ptr<model_data>;

	model_data_ptr load_model(const char* model_file);
}
