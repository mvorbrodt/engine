#pragma once

#include <vector>
#include <stdexcept>
#include <glad/glad.h>
#include "handle.hpp"

namespace engine
{
	template<typename T> struct OpenGLTextureReleasePolicy { static void Execute(T t) noexcept { glDeleteTextures(1, &t); } };
	using opengl_texture_handle = handle<GLuint, NoOpPolicy, OpenGLTextureReleasePolicy>;
	using opengl_texture_handles = std::vector<opengl_texture_handle>;

	template<typename T> struct OpenGLBufferReleasePolicy { static void Execute(T t) noexcept { glDeleteBuffers(1, &t); } };
	using opengl_buffer_handle = handle<GLuint, NoOpPolicy, OpenGLBufferReleasePolicy>;
	using opengl_buffer_handles = std::vector<opengl_buffer_handle>;

	template<typename T> struct OpenGLVertexArrayReleasePolicy { static void Execute(T t) noexcept { glDeleteVertexArrays(1, &t); } };
	using opengl_vertex_array_handle = handle<GLuint, NoOpPolicy, OpenGLVertexArrayReleasePolicy>;
	using opengl_vertex_array_handles = std::vector<opengl_vertex_array_handle>;

	template<typename T> struct OpenGLShaderReleasePolicy { static void Execute(T t) noexcept { glDeleteShader(t); } };
	using opengl_shader_handle = handle<GLuint, NoOpPolicy, OpenGLShaderReleasePolicy>;
	using opengl_shader_handles = std::vector<opengl_shader_handle>;

	template<typename T> struct OpenGLProgramReleasePolicy { static void Execute(T t) noexcept { glDeleteProgram(t); } };
	using opengl_program_handle = handle<GLuint, NoOpPolicy, OpenGLProgramReleasePolicy>;
	using opengl_program_handles = std::vector<opengl_program_handle>;

	class opengl_exception : public std::runtime_error
	{
	public:
		opengl_exception(const char* what, GLenum error_code)
		: runtime_error(what), m_error_code{ error_code } {}

		GLenum error_code() const { return m_error_code; }

	private:
		GLenum m_error_code;
	};

	void opengl_defaults();
}
