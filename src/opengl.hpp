#pragma once

#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <glad/glad.h>
#include "handle.hpp"

namespace engine
{
/*
	layout (location = 0) in vec3 Point;
	layout (location = 1) in vec4 Color;
	layout (location = 2) in vec2 Texcoord;
	layout (location = 3) in vec3 Normal;
	layout (location = 4) in vec3 Tangent;
	layout (location = 5) in vec3 Bitangent;
*/
	inline const char k_point_attribute_name[]     = "Point";
	inline const char k_color_attribute_name[]     = "Color";
	inline const char k_texcoord_attribute_name[]  = "Texcoord";
	inline const char k_normal_attribute_name[]    = "Normal";
	inline const char k_tangent_attribute_name[]   = "Tangent";
	inline const char k_bitangent_attribute_name[] = "Bitangent";
	inline const char k_index_attribute_name[]     = "Index";

	inline const unsigned int k_point_attribute_location     = 0;
	inline const unsigned int k_color_attribute_location     = 1;
	inline const unsigned int k_texcoord_attribute_location  = 2;
	inline const unsigned int k_normal_attribute_location    = 3;
	inline const unsigned int k_tangent_attribute_location   = 4;
	inline const unsigned int k_bitangent_attribute_location = 5;

	struct attribute_property
	{
		const char* name;
		const unsigned int location;
	};

	using attribute_name_to_location_map = std::map<std::string, unsigned int>;
	inline const attribute_name_to_location_map k_attribute_name_to_location_map =
	{
		{ k_point_attribute_name,     k_point_attribute_location },
		{ k_color_attribute_name,     k_color_attribute_location },
		{ k_texcoord_attribute_name,  k_texcoord_attribute_location },
		{ k_normal_attribute_name,    k_normal_attribute_location },
		{ k_tangent_attribute_name,   k_tangent_attribute_location },
		{ k_bitangent_attribute_name, k_bitangent_attribute_location },
	};

	template<typename T> struct OpenGLTextureReleasePolicy { static void Execute(T t) noexcept { glDeleteTextures(1, &t); } };
	using opengl_texture_handle = handle<GLuint, OpenGLTextureReleasePolicy>;
	using opengl_texture_handles = std::vector<opengl_texture_handle>;

	template<typename T> struct OpenGLBufferReleasePolicy { static void Execute(T t) noexcept { glDeleteBuffers(1, &t); } };
	using opengl_buffer_handle = handle<GLuint, OpenGLBufferReleasePolicy>;
	using opengl_buffer_handles = std::vector<opengl_buffer_handle>;

	template<typename T> struct OpenGLVertexArrayReleasePolicy { static void Execute(T t) noexcept { glDeleteVertexArrays(1, &t); } };
	using opengl_vertex_array_handle = handle<GLuint, OpenGLVertexArrayReleasePolicy>;
	using opengl_vertex_array_handles = std::vector<opengl_vertex_array_handle>;

	template<typename T> struct OpenGLShaderReleasePolicy { static void Execute(T t) noexcept { glDeleteShader(t); } };
	using opengl_shader_handle = handle<GLuint, OpenGLShaderReleasePolicy>;
	using opengl_shader_handles = std::vector<opengl_shader_handle>;

	template<typename T> struct OpenGLProgramReleasePolicy { static void Execute(T t) noexcept { glDeleteProgram(t); } };
	using opengl_program_handle = handle<GLuint, OpenGLProgramReleasePolicy>;
	using opengl_program_handles = std::vector<opengl_program_handle>;

	void init_opengl_defaults();
}
