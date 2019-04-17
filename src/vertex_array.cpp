#include <cassert>
#include <glad/glad.h>
#include "opengl.hpp"
#include "vertex_array.hpp"

using namespace std;

namespace engine
{
	vertex_array::vertex_array(const model_data_ptr& data)
	{
		glGenVertexArrays(1, &m_vertex_array_handle);
		glBindVertexArray(m_vertex_array_handle);

		const auto& position_buffer  = get<0>(*data);
		const auto& normal_buffer    = get<1>(*data);
		const auto& tangent_buffer   = get<2>(*data);
		const auto& bitangent_buffer = get<3>(*data);
		const auto& texcoord_buffer  = get<4>(*data);
		const auto& index_buffer     = get<5>(*data);

		assert(position_buffer.size() > 0 && index_buffer.size() > 0);

		m_triangles = index_buffer.size();

		unsigned int next_attribute = 0;

		glGenBuffers(1, &m_position_buffer_handle);
		glBindBuffer(GL_ARRAY_BUFFER, m_position_buffer_handle);
		glBufferData(GL_ARRAY_BUFFER, position_buffer.size() * sizeof(point), position_buffer.data(), GL_STATIC_DRAW);
		GLenum error_code = glGetError();
		if(error_code != GL_NO_ERROR) throw opengl_exception("glBufferData failed!", error_code);
		glVertexAttribPointer(next_attribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(next_attribute);
		++next_attribute;

		if(normal_buffer.size() > 0)
		{
			glGenBuffers(1, &m_normal_buffer_handle);
			glBindBuffer(GL_ARRAY_BUFFER, m_normal_buffer_handle);
			glBufferData(GL_ARRAY_BUFFER, normal_buffer.size() * sizeof(vector), normal_buffer.data(), GL_STATIC_DRAW);
			error_code = glGetError();
			if(error_code != GL_NO_ERROR) throw opengl_exception("glBufferData failed!", error_code);
			glVertexAttribPointer(next_attribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(next_attribute);
			++next_attribute;
		}

		if(tangent_buffer.size() > 0 && bitangent_buffer.size() > 0)
		{
			glGenBuffers(1, &m_tangent_buffer_handle);
			glBindBuffer(GL_ARRAY_BUFFER, m_tangent_buffer_handle);
			glBufferData(GL_ARRAY_BUFFER, tangent_buffer.size() * sizeof(vector), tangent_buffer.data(), GL_STATIC_DRAW);
			error_code = glGetError();
			if(error_code != GL_NO_ERROR) throw opengl_exception("glBufferData failed!", error_code);
			glVertexAttribPointer(next_attribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(next_attribute);
			++next_attribute;

			glGenBuffers(1, &m_bitangent_buffer_handle);
			glBindBuffer(GL_ARRAY_BUFFER, m_bitangent_buffer_handle);
			glBufferData(GL_ARRAY_BUFFER, bitangent_buffer.size() * sizeof(vector), bitangent_buffer.data(), GL_STATIC_DRAW);
			error_code = glGetError();
			if(error_code != GL_NO_ERROR) throw opengl_exception("glBufferData failed!", error_code);
			glVertexAttribPointer(next_attribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(next_attribute);
			++next_attribute;
		}

		if(texcoord_buffer.size() > 0)
		{
			glGenBuffers(1, &m_texcoord_buffer_handle);
			glBindBuffer(GL_ARRAY_BUFFER, m_texcoord_buffer_handle);
			glBufferData(GL_ARRAY_BUFFER, texcoord_buffer.size() * sizeof(texcoord), texcoord_buffer.data(), GL_STATIC_DRAW);
			error_code = glGetError();
			if(error_code != GL_NO_ERROR) throw opengl_exception("glBufferData failed!", error_code);
			glVertexAttribPointer(next_attribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(next_attribute);
			++next_attribute;
		}

		glGenBuffers(1, &m_index_buffer_handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer.size() * sizeof(int), index_buffer.data(), GL_STATIC_DRAW);
		error_code = glGetError();
		if(error_code != GL_NO_ERROR) throw opengl_exception("glBufferData failed!", error_code);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	vertex_array::~vertex_array()
	{
		glDeleteBuffers(1, &m_position_buffer_handle);
		glDeleteBuffers(1, &m_normal_buffer_handle);
		glDeleteBuffers(1, &m_tangent_buffer_handle);
		glDeleteBuffers(1, &m_bitangent_buffer_handle);
		glDeleteBuffers(1, &m_texcoord_buffer_handle);
		glDeleteBuffers(1, &m_index_buffer_handle);
		glDeleteVertexArrays(1, &m_vertex_array_handle);
	}

	void vertex_array::draw() const
	{
		glBindVertexArray(m_vertex_array_handle);
		glDrawElements(GL_TRIANGLES, m_triangles, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	}

	vertex_array_ptr make_vertex_array(const model_data_ptr& data)
	{
		return make_shared<vertex_array>(data);
	}
}
