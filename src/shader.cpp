#include <cassert>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "shader.hpp"

using namespace std;

namespace engine
{
	uniform_buffer::uniform_buffer(const char* name, std::size_t size, unsigned int binding_point)
	: m_buffer_name{ name }, m_buffer_size{ size }, m_binding_point{ binding_point }
	{
		assert(name != nullptr && size > 0);

		glGenBuffers(1, &m_buffer_handle);
		glBindBuffer(GL_UNIFORM_BUFFER, m_buffer_handle);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, m_buffer_handle);
	}

	void uniform_buffer::copy_data(std::size_t offset, std::size_t length, void* data) const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_buffer_handle);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, length, data);
	}

	uniform_buffer_ptr make_uniform_buffer(const char* name, std::size_t size, unsigned int binding_point)
	{
		return make_shared<uniform_buffer>(name, size, binding_point);
	}

	shader_buffer::shader_buffer(const char* name, std::size_t size, unsigned int binding_point)
	: m_buffer_name{ name }, m_buffer_size{ size }, m_binding_point{ binding_point }
	{
		assert(name != nullptr && size > 0);

		glGenBuffers(1, &m_buffer_handle);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_buffer_handle);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_point, m_buffer_handle);
	}

	void shader_buffer::copy_data(std::size_t offset, std::size_t length, void* data) const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_buffer_handle);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, length, data);
	}

	shader_buffer_ptr make_shader_buffer(const char* name, std::size_t size, unsigned int binding_point)
	{
		return make_shared<shader_buffer>(name, size, binding_point);
	}

	#define CHECK_SHADER_STATUS(getter, handle, status, logger) \
	{ \
		int success{}; \
		getter(handle, status, &success); \
		if(!success) \
		{ \
			GLint length{}; \
			getter(handle, GL_INFO_LOG_LENGTH, &length); \
			string error; \
			error.resize(length); \
			logger(handle, length, NULL, error.data()); \
			throw runtime_error(error.c_str()); \
		} \
	}

	#define DUMP_PROGRAM_INFO() \
	{ \
		cout << "Program info: " << endl; \
		GLint count{}, max_length{}, size{}; \
		GLenum type{}; \
		glGetProgramiv(m_program_handle, GL_ACTIVE_ATTRIBUTES, &count); \
		glGetProgramiv(m_program_handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length); \
		for(GLint attribute = 0; attribute < count; ++attribute) \
		{ \
			string name(max_length, '\0'); \
			glGetActiveAttrib(m_program_handle, attribute, (GLsizei)name.length(), NULL, &size, &type, name.data()); \
			cout << "\tActive attribute: " << name << ", size: " << size << ", type: " << type << endl; \
		} \
		glGetProgramiv(m_program_handle, GL_ACTIVE_UNIFORMS, &count); \
		glGetProgramiv(m_program_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length); \
		for(GLint uniform = 0; uniform < count; ++uniform) \
		{ \
			string name(max_length, '\0'); \
			glGetActiveUniform(m_program_handle, uniform, (GLsizei)name.length(), NULL, &size, &type, name.data()); \
			cout << "\tActive uniform: " << name << ", size: " << size << ", type: " << type << endl; \
		} \
		glGetProgramiv(m_program_handle, GL_ACTIVE_UNIFORM_BLOCKS, &count); \
		glGetProgramiv(m_program_handle, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &max_length); \
		for(GLint block = 0; block < count; ++block) \
		{ \
			string name(max_length, '\0'); \
			glGetActiveUniformBlockName(m_program_handle, block, (GLsizei)name.length(), NULL, name.data()); \
			glGetActiveUniformBlockiv(m_program_handle, block, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &size); \
			cout << "\tActive uniform block: " << name << ", active uniforms: " << size << endl; \
		} \
		glGetProgramInterfaceiv(m_program_handle, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &count); \
		glGetProgramInterfaceiv(m_program_handle, GL_SHADER_STORAGE_BLOCK, GL_MAX_NAME_LENGTH, &max_length); \
		for(GLint block = 0; block < count; ++block) \
		{ \
			string name(max_length, '\0'); \
			glGetProgramResourceName(m_program_handle, GL_SHADER_STORAGE_BLOCK, block, (GLsizei)name.length(), NULL, name.data()); \
			GLenum prop = GL_NUM_ACTIVE_VARIABLES; \
			glGetProgramResourceiv(m_program_handle, GL_SHADER_STORAGE_BLOCK, block, 1, &prop, sizeof(size), NULL, &size); \
			cout << "\tActive storage block: " << name << ", active variables: " << size << endl; \
		} \
	}

	shader::shader(const char* vertex_shader_source, const char* fragment_shader_source)
	{
		m_vertex_shader_handle = opengl_shader_handle{ glCreateShader(GL_VERTEX_SHADER) };
		glShaderSource(m_vertex_shader_handle, 1, &vertex_shader_source, NULL);
		glCompileShader(m_vertex_shader_handle);
		CHECK_SHADER_STATUS(glGetShaderiv, m_vertex_shader_handle, GL_COMPILE_STATUS, glGetShaderInfoLog);

		m_fragment_shader_handle = opengl_shader_handle{ glCreateShader(GL_FRAGMENT_SHADER) };
		glShaderSource(m_fragment_shader_handle, 1, &fragment_shader_source, NULL);
		glCompileShader(m_fragment_shader_handle);
		CHECK_SHADER_STATUS(glGetShaderiv, m_fragment_shader_handle, GL_COMPILE_STATUS, glGetShaderInfoLog);

		m_program_handle = opengl_program_handle{ glCreateProgram() };
		glAttachShader(m_program_handle, m_vertex_shader_handle);
		glAttachShader(m_program_handle, m_fragment_shader_handle);
		glLinkProgram(m_program_handle);
		CHECK_SHADER_STATUS(glGetProgramiv, m_program_handle, GL_LINK_STATUS, glGetProgramInfoLog);

		DUMP_PROGRAM_INFO();
	}

	void shader::set_bool(const char* name, bool value) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glProgramUniform1i(m_program_handle, id, value);
	}

	void shader::set_int(const char* name, int value) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glProgramUniform1i(m_program_handle, id, value);
	}

	void shader::set_float(const char* name, float value) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glProgramUniform1f(m_program_handle, id, value);
	}

	void shader::set_vec2(const char* name, real x, real y) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glProgramUniform2f(m_program_handle, id, x, y);
	}

	void shader::set_vec3(const char* name, real x, real y, real z) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glProgramUniform3f(m_program_handle, id, x, y, z);
	}

	void shader::set_vec4(const char* name, real x, real y, real z, real w) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glProgramUniform4f(m_program_handle, id, x, y, z, w);
	}

	void shader::set_vec2(const char* name, const real* vec) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glProgramUniform2fv(m_program_handle, id, 1, vec);
	}

	void shader::set_vec3(const char* name, const real* vec) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glProgramUniform3fv(m_program_handle, id, 1, vec);
	}

	void shader::set_vec4(const char* name, const real* vec) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glProgramUniform4fv(m_program_handle, id, 1, vec);
	}

	void shader::set_mat2(const char* name, const real* mat) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glProgramUniformMatrix2fv(m_program_handle, id, 1, GL_FALSE, mat);
	}

	void shader::set_mat3(const char* name, const real* mat) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glProgramUniformMatrix3fv(m_program_handle, id, 1, GL_FALSE, mat);
	}

	void shader::set_mat4(const char* name, const real* mat) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glProgramUniformMatrix4fv(m_program_handle, id, 1, GL_FALSE, mat);
	}

	void shader::bind_uniform_block(const char* name, unsigned int binding_point) const
	{
		unsigned int index = glGetUniformBlockIndex(m_program_handle, name);
		glUniformBlockBinding(m_program_handle, index, binding_point);
	}

	void shader::bind_shader_block(const char* name, unsigned int binding_point) const
	{
		unsigned int index = glGetProgramResourceIndex(m_program_handle, GL_SHADER_STORAGE_BLOCK, name);
		glShaderStorageBlockBinding(m_program_handle, index, binding_point);
	}

	void shader::connect_uniform_block(const uniform_buffer_ptr& uniform_buffer) const
	{
		unsigned int index = glGetUniformBlockIndex(m_program_handle, uniform_buffer->get_name());
		glUniformBlockBinding(m_program_handle, index, uniform_buffer->get_binding_point());
	}

	void shader::connect_shader_block(const shader_buffer_ptr& shader_buffer) const
	{
		unsigned int index = glGetProgramResourceIndex(m_program_handle, GL_SHADER_STORAGE_BLOCK, shader_buffer->get_name());
		glShaderStorageBlockBinding(m_program_handle, index, shader_buffer->get_binding_point());
	}

	void shader::make_current() const
	{
		glUseProgram(m_program_handle);
	}

	shader_ptr make_shader(const char* vertex_shader_source, const char* fragment_shader_source)
	{
		return make_shared<shader>(vertex_shader_source, fragment_shader_source);
	}

	shader_ptr load_shader(const char* vertex_shader_file, const char* fragment_shader_file)
	{
		ifstream vsf;
		vsf.exceptions(ios::failbit | ios::badbit);
		vsf.open(vertex_shader_file);
		stringstream vsbuffer;
		vsbuffer << vsf.rdbuf();
		string vertex_shader_source = vsbuffer.str();

		ifstream fsf;
		fsf.exceptions(ios::failbit | ios::badbit);
		fsf.open(fragment_shader_file);
		stringstream fsbuffer;
		fsbuffer << fsf.rdbuf();
		string fragment_shader_source = fsbuffer.str();

		return make_shader(vertex_shader_source.c_str(), fragment_shader_source.c_str());
	}
}
