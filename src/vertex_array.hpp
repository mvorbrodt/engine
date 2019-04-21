#pragma once

#include <tuple>
#include <vector>
#include <memory>
#include "buffer.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "color.hpp"
#include "texcoord.hpp"
#include "opengl.hpp"

namespace engine
{
	// <vertex, color, texture, normal, tangent, bitangent>
	using flat_vertex_array_data = std::tuple<point_buffer, color_buffer, texcoord_buffer, normal_buffer, tangent_buffer, bitangent_buffer>;
	using flat_vertex_array_data_ptr = std::shared_ptr<flat_vertex_array_data>;
	using flat_vertex_array_data_array = std::vector<flat_vertex_array_data_ptr>;

	flat_vertex_array_data_ptr make_flat_vertex_array_data(
		const point_buffer& point_buffer,
		const color_buffer& color_buffer,
		const texcoord_buffer& texcoord_buffer,
		const normal_buffer& normal_buffer,
		const tangent_buffer& tangent_buffer,
		const bitangent_buffer& bitangent_buffer);

	// <vertex, color, texture, normal, tangent, bitangent, index>
	using indexed_vertex_array_data = std::tuple<point_buffer, color_buffer, texcoord_buffer, normal_buffer, tangent_buffer, bitangent_buffer, index_buffer>;
	using indexed_vertex_array_data_ptr = std::shared_ptr<indexed_vertex_array_data>;
	using indexed_vertex_array_data_array = std::vector<indexed_vertex_array_data_ptr>;

	indexed_vertex_array_data_ptr make_indexed_vertex_array_data(
		const point_buffer& point_buffer,
		const color_buffer& color_buffer,
		const texcoord_buffer& texcoord_buffer,
		const normal_buffer& normal_buffer,
		const tangent_buffer& tangent_buffer,
		const bitangent_buffer& bitangent_buffer,
		const index_buffer& index_buffer);

	class vertex_array
	{
	public:
		virtual ~vertex_array() {}

		virtual void draw(GLenum primitive) const = 0;
	};

	using vertex_array_ptr = std::shared_ptr<vertex_array>;
	using vertex_arrays = std::vector<vertex_array_ptr>;

	vertex_array_ptr make_flat_vertex_array(const flat_vertex_array_data_ptr& data);
	vertex_array_ptr make_indexed_vertex_array(const indexed_vertex_array_data_ptr& data);

	class flat_vertex_array : public vertex_array
	{
	public:
		flat_vertex_array(const flat_vertex_array_data_ptr& data);

		void draw(GLenum primitive) const override;

	private:
		std::size_t m_elements = 0;

		opengl_buffer_handles m_buffer_handles;
		opengl_vertex_array_handle m_vertex_array_handle;
	};

	class indexed_vertex_array : public vertex_array
	{
	public:
		indexed_vertex_array(const indexed_vertex_array_data_ptr& data);

		void draw(GLenum primitive) const override;

	private:
		std::size_t m_indices = 0;

		opengl_buffer_handles m_buffer_handles;
		opengl_vertex_array_handle m_vertex_array_handle;
	};
}
