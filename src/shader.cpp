#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <glad/glad.h>
#include "opengl.hpp"
#include "shader.hpp"

using namespace std;

namespace engine
{
	shader::shader(const char* vertex_shader_source, const char* fragment_shader_source)
	{
		int success{};
		m_vertex_shader_handle = glCreateShader(GL_VERTEX_SHADER);
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

		m_fragment_shader_handle = glCreateShader(GL_FRAGMENT_SHADER);
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

		m_program_handle = glCreateProgram();
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

	shader::~shader()
	{
		glDetachShader(m_program_handle, m_vertex_shader_handle);
		glDetachShader(m_program_handle, m_fragment_shader_handle);
		glDeleteShader(m_vertex_shader_handle);
		glDeleteShader(m_fragment_shader_handle);
		glDeleteProgram(m_program_handle);
	}

	void shader::use() const
	{
		glUseProgram(m_program_handle);
	}

	void shader::set_matrix(const char* name, const matrix& m) const
	{
		auto id = glGetUniformLocation(m_program_handle, name);
		glUniformMatrix4fv(id, 1, GL_FALSE, m.data());
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
