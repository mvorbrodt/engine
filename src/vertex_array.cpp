#include <cassert>
#include <array>
#include <utility>
#include "vertex_array.hpp"

using namespace std;

namespace engine
{
	vertex_array::vertex_array(const model_data_ptr& data)
	{
		glGenVertexArrays(1, &m_vertex_array_handle);
		glBindVertexArray(m_vertex_array_handle);

		array<const buffer*, 7> buffers =
		{
			&get<0>(*data),
			&get<1>(*data),
			&get<2>(*data),
			&get<3>(*data),
			&get<4>(*data),
			&get<5>(*data),
			&get<6>(*data)
		};

		unsigned int next_attribute = 0;

		for(auto buffer : buffers)
		{
			if(buffer->size() > 0)
			{
				opengl_buffer_handle handle;
				glGenBuffers(1, &handle);
				glBindBuffer(buffer->opengl_buffer_type(), handle);
				glBufferData(buffer->opengl_buffer_type(), buffer->size() * buffer->element_size(), buffer->data(), GL_STATIC_DRAW);

				GLenum error_code = glGetError();
				if(error_code != GL_NO_ERROR) throw opengl_exception("glBufferData failed!", error_code);

				if(buffer->opengl_buffer_type() == GL_ARRAY_BUFFER)
				{
					glVertexAttribPointer(next_attribute, buffer->component_count(), buffer->opengl_element_type(), GL_FALSE, buffer->element_size(), (void*)0);
					glEnableVertexAttribArray(next_attribute);
					++next_attribute;
				}

				if(buffer->opengl_buffer_type() == GL_ELEMENT_ARRAY_BUFFER) m_indices = buffer->size();

				m_buffer_handles.push_back(move(handle));
			}
		}

		glBindVertexArray(0);
	}

	void vertex_array::draw() const
	{
		glBindVertexArray(m_vertex_array_handle);
		glDrawElements(GL_TRIANGLES, m_indices, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	}

	vertex_array_ptr make_vertex_array(const model_data_ptr& data)
	{
		return make_shared<vertex_array>(data);
	}
}
