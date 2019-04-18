#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "shader.hpp"

using namespace std;

namespace engine
{
	shader::shader(const char* vertex_shader_source, const char* fragment_shader_source)
	{
		int success{};
		m_vertex_shader_handle = opengl_shader_handle{ glCreateShader(GL_VERTEX_SHADER) };
		glShaderSource(m_vertex_shader_handle, 1, &vertex_shader_source, NULL);
		glCompileShader(m_vertex_shader_handle);
		glGetShaderiv(m_vertex_shader_handle, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			GLint length{};
			glGetShaderiv(m_vertex_shader_handle, GL_INFO_LOG_LENGTH, &length);
			string error;
			error.resize(length + 1);
			glGetShaderInfoLog(m_vertex_shader_handle, length, NULL, error.data());
			throw opengl_exception(("OpenGL Vertex Shader: " + error).c_str(), glGetError());
		}

		m_fragment_shader_handle = opengl_shader_handle{ glCreateShader(GL_FRAGMENT_SHADER) };
		glShaderSource(m_fragment_shader_handle, 1, &fragment_shader_source, NULL);
		glCompileShader(m_fragment_shader_handle);
		glGetShaderiv(m_fragment_shader_handle, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			GLint length{};
			glGetShaderiv(m_fragment_shader_handle, GL_INFO_LOG_LENGTH, &length);
			string error;
			error.resize(length + 1);
			glGetShaderInfoLog(m_fragment_shader_handle, length, NULL, error.data());
			throw opengl_exception(("OpenGL Fragment Shader: " + error).c_str(), glGetError());
		}

		m_program_handle = opengl_program_handle{ glCreateProgram() };
		glAttachShader(m_program_handle, m_vertex_shader_handle);
		glAttachShader(m_program_handle, m_fragment_shader_handle);
		glLinkProgram(m_program_handle);
		glGetProgramiv(m_program_handle, GL_LINK_STATUS, &success);
		if(!success)
		{
			GLint length{};
			glGetShaderiv(m_program_handle, GL_INFO_LOG_LENGTH, &length);
			string error;
			error.resize(length);
			glGetShaderInfoLog(m_program_handle, length, NULL, error.data());
			throw opengl_exception(("OpenGL Program: " + error).c_str(), glGetError());
		}
	}

	void shader::use() const
	{
		glUseProgram(m_program_handle);
	}

	void shader::set_bool(const char* name, bool value) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glUniform1i(id, value);
	}

	void shader::set_int(const char* name, int value) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glUniform1i(id, value);
	}

	void shader::set_float(const char* name, float value) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glUniform1f(id, value);
	}

	void shader::set_vec2(const char* name, real x, real y) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glUniform2f(id, x, y);
	}

	void shader::set_vec3(const char* name, real x, real y, real z) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glUniform3f(id, x, y, z);
	}

	void shader::set_vec4(const char* name, real x, real y, real z, real w) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glUniform4f(id, x, y, z, w);
	}

	void shader::set_vec2(const char* name, const real* vec) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glUniform2fv(id, 1, vec);
	}

	void shader::set_vec3(const char* name, const real* vec) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glUniform3fv(id, 1, vec);
	}

	void shader::set_vec4(const char* name, const real* vec) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glUniform4fv(id, 1, vec);
	}

	void shader::set_mat2(const char* name, const real* mat) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glUniformMatrix2fv(id, 1, GL_FALSE, mat);
	}

	void shader::set_mat3(const char* name, const real* mat) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glUniformMatrix3fv(id, 1, GL_FALSE, mat);
	}

	void shader::set_mat4(const char* name, const real* mat) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glUniformMatrix4fv(id, 1, GL_FALSE, mat);
	}

	void shader::bind_texture(const char* name, int unit) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glUniform1i(id, unit);
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

		return make_shared<shader>(vertex_shader_source.c_str(), fragment_shader_source.c_str());
	}
}
