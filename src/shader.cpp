#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <glad/glad.h>
#include "shader.hpp"

using namespace std;

namespace engine
{
	shader::shader(const char* vertex_shader_source, const char* fragment_shader_source)
	{
		int success{};
		int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
		glCompileShader(vertex_shader);
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			GLint length{};
			glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &length);
			string error;
			error.resize(length + 1);
			glGetShaderInfoLog(vertex_shader, length, NULL, error.data());
			throw runtime_error("OpenGL Vertex Shader: " + error);
		}

		int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
		glCompileShader(fragment_shader);
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			GLint length{};
			glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &length);
			string error;
			error.resize(length + 1);
			glGetShaderInfoLog(fragment_shader, length, NULL, error.data());
			throw runtime_error("OpenGL Fragment Shader: " + error);
		}

		m_handle = glCreateProgram();
		glAttachShader(m_handle, vertex_shader);
		glAttachShader(m_handle, fragment_shader);
		glLinkProgram(m_handle);
		glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
		if(!success)
		{
			GLint length{};
			glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &length);
			string error;
			error.resize(length);
			glGetShaderInfoLog(m_handle, length, NULL, error.data());
			throw runtime_error("OpenGL Program: " + error);
		}

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}

	shader::~shader()
	{
		glDeleteProgram(m_handle);
	}

	void shader::use() const
	{
		glUseProgram(m_handle);
	}

	void shader::bind_texture(const char* name, int unit) const
	{
		auto id = glGetUniformLocation(m_handle, name);
		glUniform1i(id, unit);
	}

	void shader::load_matrix(const char* name, const matrix& m) const
	{
		auto id = glGetUniformLocation(m_handle, name);
		glUniformMatrix4fv(id, 1, GL_FALSE, m.data());
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
