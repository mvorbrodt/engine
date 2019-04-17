#pragma once

#include <memory>
#include <vector>
#include <tuple>
#include "types.hpp"

namespace engine
{
	class vertex_array
	{
	public:
		vertex_array();
		vertex_array(const vertex_array&) = delete;
		vertex_array(vertex_array&&) = delete;
		vertex_array& operator = (const vertex_array&) = delete;
		vertex_array& operator = (vertex_array&&) = delete;
		~vertex_array();

		void bind() const;
		void draw() const;

	private:
		unsigned int m_vertex_array_handle;
	};

	using vertex_array_ptr = std::shared_ptr<vertex_array>;
	using vertex_arrays = std::vector<vertex_array_ptr>;

	using model_data = std::tuple<real_buffer, real_buffer, real_buffer, int_buffer>;
	model_data load_model_file(const char* model_file);
}
