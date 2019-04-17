#pragma once

#include <tuple>
#include <memory>
#include <vector>
#include "types.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "texcoord.hpp"

namespace engine
{
	// <vertex, texture, normal, tangent, bitangent, index>
	using model_data = std::tuple<point_buffer, texcoord_buffer, vector_buffer, vector_buffer, vector_buffer, int_buffer>;
	using model_data_ptr = std::shared_ptr<model_data>;
	using model_data_array = std::vector<model_data_ptr>;

	model_data_array load_model(const char* model_file);
}
