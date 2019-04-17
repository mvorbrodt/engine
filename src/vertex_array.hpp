#pragma once

#include <memory>
#include <vector>
#include "load_model.hpp"

namespace engine
{
	class vertex_array
	{
	public:
		vertex_array(const model_data_ptr& data);
		vertex_array(const vertex_array&) = delete;
		vertex_array(vertex_array&&) = delete;
		vertex_array& operator = (const vertex_array&) = delete;
		vertex_array& operator = (vertex_array&&) = delete;
		~vertex_array();

		void draw() const;

	private:
		unsigned int m_triangles = 0;
		unsigned int m_position_buffer_handle = 0;
		unsigned int m_texcoord_buffer_handle = 0;
		unsigned int m_normal_buffer_handle = 0;
		unsigned int m_tangent_buffer_handle = 0;
		unsigned int m_bitangent_buffer_handle = 0;
		unsigned int m_index_buffer_handle = 0;
		unsigned int m_vertex_array_handle = 0;
	};

	using vertex_array_ptr = std::shared_ptr<vertex_array>;
	using vertex_arrays = std::vector<vertex_array_ptr>;

	vertex_array_ptr make_vertex_array(const model_data_ptr& data);
}
