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
		virtual ~vertex_array() {}

		virtual void draw() const = 0;
	};

	using vertex_array_ptr = std::shared_ptr<vertex_array>;
	using vertex_arrays = std::vector<vertex_array_ptr>;

	vertex_array_ptr make_flat_vertex_array(const flat_model_data_ptr& data);
	vertex_array_ptr make_indexed_vertex_array(const indexed_model_data_ptr& data);

	class flat_vertex_array : public vertex_array
	{
	public:
		flat_vertex_array(const flat_model_data_ptr& data);

		void draw() const;

	private:
		std::size_t m_elements = 0;

		opengl_buffer_handles m_buffer_handles;
		opengl_vertex_array_handle m_vertex_array_handle;
	};

	class indexed_vertex_array : public vertex_array
	{
	public:
		indexed_vertex_array(const indexed_model_data_ptr& data);

		void draw() const;

	private:
		std::size_t m_indices = 0;

		opengl_buffer_handles m_buffer_handles;
		opengl_vertex_array_handle m_vertex_array_handle;
	};
}
