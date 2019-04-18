#pragma once

#include <tuple>
#include <memory>
#include <vector>
#include "buffer.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "color.hpp"
#include "texcoord.hpp"

namespace engine
{
	// <vertex, color, texture, normal, tangent, bitangent, index>
	using model_data = std::tuple<point_buffer, color_buffer, texcoord_buffer, vector_buffer, vector_buffer, vector_buffer, index_buffer>;
	using model_data_ptr = std::shared_ptr<model_data>;
	using model_data_array = std::vector<model_data_ptr>;

	model_data_ptr make_model_data(
		const point_buffer& point_buffer,
		const color_buffer& color_buffer,
		const texcoord_buffer& texcoord_buffer,
		const vector_buffer& normal_buffer,
		const vector_buffer& tangent_buffer,
		const vector_buffer& bitangent_buffer,
		const index_buffer& index_buffer);

	model_data_array load_model(const char* model_file);
}
