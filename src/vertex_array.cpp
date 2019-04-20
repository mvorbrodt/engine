#include <cassert>
#include <iostream>
#include <array>
#include <utility>
#include "vertex_array.hpp"

using namespace std;

namespace engine
{
	vertex_array_ptr make_flat_vertex_array(const flat_model_data_ptr& data)
	{
		return make_shared<flat_vertex_array>(data);
	}

	vertex_array_ptr make_indexed_vertex_array(const indexed_model_data_ptr& data)
	{
		return make_shared<indexed_vertex_array>(data);
	}

	flat_vertex_array::flat_vertex_array(const flat_model_data_ptr& data)
	{
		glGenVertexArrays(1, &m_vertex_array_handle);
		glBindVertexArray(m_vertex_array_handle);

		array<const buffer*, 6> buffers =
		{
			&get<0>(*data),
			&get<1>(*data),
			&get<2>(*data),
			&get<3>(*data),
			&get<4>(*data),
			&get<5>(*data)
		};

		cout << "Building flat vertex array..." << endl;

		for(auto buffer : buffers)
		{
			if(buffer->size() > 0)
			{
				cout << "\telement: " << buffer->element_name() << ", count: " << buffer->size() << endl;

				assert(buffer->opengl_buffer_type() == GL_ARRAY_BUFFER);

				opengl_buffer_handle handle;
				glGenBuffers(1, &handle);
				glBindBuffer(buffer->opengl_buffer_type(), handle);
				glBufferData(buffer->opengl_buffer_type(), buffer->size() * buffer->element_size(), buffer->data(), GL_STATIC_DRAW);

				auto location = k_attribute_name_to_location_map.at(buffer->element_name());
				glVertexAttribPointer(location, (GLint)buffer->component_count(), buffer->opengl_element_type(), GL_FALSE, (GLsizei)buffer->element_size(), (void*)0);
				glEnableVertexAttribArray(location);

				m_elements = buffer->size();

				m_buffer_handles.push_back(move(handle));
			}
		}
	}

	void flat_vertex_array::draw(GLenum primitive) const
	{
		glBindVertexArray(m_vertex_array_handle);
		glDrawArrays(primitive, 0, (GLsizei)m_elements);
	}

	indexed_vertex_array::indexed_vertex_array(const indexed_model_data_ptr& data)
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

		cout << "Building indexed vertex array..." << endl;

		for(auto buffer : buffers)
		{
			if(buffer->size() > 0)
			{
				cout << "\telement: " << buffer->element_name() << ", count: " << buffer->size() << endl;

				assert(buffer->opengl_buffer_type() == GL_ARRAY_BUFFER || buffer->opengl_buffer_type() == GL_ELEMENT_ARRAY_BUFFER);

				opengl_buffer_handle handle;
				glGenBuffers(1, &handle);
				glBindBuffer(buffer->opengl_buffer_type(), handle);
				glBufferData(buffer->opengl_buffer_type(), buffer->size() * buffer->element_size(), buffer->data(), GL_STATIC_DRAW);

				if(buffer->opengl_buffer_type() == GL_ARRAY_BUFFER)
				{
					auto location = k_attribute_name_to_location_map.at(buffer->element_name());
					glVertexAttribPointer(location, (GLint)buffer->component_count(), buffer->opengl_element_type(), GL_FALSE, (GLsizei)buffer->element_size(), (void*)0);
					glEnableVertexAttribArray(location);
				}

				if(buffer->opengl_buffer_type() == GL_ELEMENT_ARRAY_BUFFER) m_indices = buffer->size();

				m_buffer_handles.push_back(move(handle));
			}
		}
	}

	void indexed_vertex_array::draw(GLenum primitive) const
	{
		glBindVertexArray(m_vertex_array_handle);
		glDrawElements(primitive, (GLsizei)m_indices, GL_UNSIGNED_INT, (void*)0);
	}
}
