#include <cassert>
#include <glad/glad.h>
#include "vertex_array.hpp"

using namespace std;

namespace engine
{
	vertex_array::vertex_array(const model_data& data)
	{
		glGenVertexArrays(1, &m_vertex_array_handle);
		glBindVertexArray(m_vertex_array_handle);

		const auto& position_buffer = get<0>(data);
		const auto& normal_buffer = get<1>(data);
		const auto& texcoord_buffer = get<2>(data);
		const auto& index_buffer = get<3>(data);

		m_triangles = index_buffer.size();

		glGenBuffers(1, &m_position_buffer_handle);
		glBindBuffer(GL_ARRAY_BUFFER, m_position_buffer_handle);
		glBufferData(GL_ARRAY_BUFFER, position_buffer.size() * sizeof(real), position_buffer.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &m_normal_buffer_handle);
		glBindBuffer(GL_ARRAY_BUFFER, m_normal_buffer_handle);
		glBufferData(GL_ARRAY_BUFFER, normal_buffer.size() * sizeof(real), normal_buffer.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &m_texcoord_buffer_handle);
		glBindBuffer(GL_ARRAY_BUFFER, m_texcoord_buffer_handle);
		glBufferData(GL_ARRAY_BUFFER, texcoord_buffer.size() * sizeof(real), texcoord_buffer.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &m_index_buffer_handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer.size() * sizeof(int), index_buffer.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	vertex_array::~vertex_array()
	{
		glDeleteBuffers(1, &m_position_buffer_handle);
		glDeleteBuffers(1, &m_normal_buffer_handle);
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

	vertex_array_ptr make_vertex_array(const model_data& data)
	{
		return make_shared<vertex_array>(data);
	}
}
