#pragma once

#include <memory>
#include <vector>
#include "opengl.hpp"
#include "load_model.hpp"

namespace engine
{
	class vertex_array
	{
	public:
		vertex_array(const model_data_ptr& data);

		void draw() const;

	private:
		std::size_t m_indices = 0;

		opengl_buffer_handles m_buffer_handles;
		opengl_vertex_array_handle m_vertex_array_handle;
	};

	using vertex_array_ptr = std::shared_ptr<vertex_array>;
	using vertex_arrays = std::vector<vertex_array_ptr>;

	vertex_array_ptr make_vertex_array(const model_data_ptr& data);
}
