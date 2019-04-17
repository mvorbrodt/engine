#include <cassert>
#include <glad/glad.h>
#include "vertex_array.hpp"

namespace engine
{
	vertex_array::vertex_array()
	{
		glGenVertexArrays(1, &m_vertex_array_handle);
	}

	vertex_array::~vertex_array()
	{
		glDeleteVertexArrays(1, &m_vertex_array_handle);
	}

	void vertex_array::bind() const
	{
		glBindVertexArray(m_vertex_array_handle);
	}

	void vertex_array::draw() const
	{
	}
}
