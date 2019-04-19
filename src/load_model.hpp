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
	// <vertex, color, texture, normal, tangent, bitangent>
	using flat_model_data = std::tuple<point_buffer, color_buffer, texcoord_buffer, normal_buffer, tangent_buffer, bitangent_buffer>;
	using flat_model_data_ptr = std::shared_ptr<flat_model_data>;
	using flat_model_data_array = std::vector<flat_model_data_ptr>;

	flat_model_data_ptr make_flat_model_data(
		const point_buffer& point_buffer,
		const color_buffer& color_buffer,
		const texcoord_buffer& texcoord_buffer,
		const normal_buffer& normal_buffer,
		const tangent_buffer& tangent_buffer,
		const bitangent_buffer& bitangent_buffer);

	// <vertex, color, texture, normal, tangent, bitangent, index>
	using indexed_model_data = std::tuple<point_buffer, color_buffer, texcoord_buffer, normal_buffer, tangent_buffer, bitangent_buffer, index_buffer>;
	using indexed_model_data_ptr = std::shared_ptr<indexed_model_data>;
	using indexed_model_data_array = std::vector<indexed_model_data_ptr>;

	indexed_model_data_ptr make_indexed_model_data(
		const point_buffer& point_buffer,
		const color_buffer& color_buffer,
		const texcoord_buffer& texcoord_buffer,
		const normal_buffer& normal_buffer,
		const tangent_buffer& tangent_buffer,
		const bitangent_buffer& bitangent_buffer,
		const index_buffer& index_buffer);

	indexed_model_data_array load_indexed_model_data(const char* model_file);
}
