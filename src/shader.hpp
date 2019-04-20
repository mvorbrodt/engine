#pragma once

#include <cstddef>
#include <string>
#include <memory>
#include "types.hpp"
#include "opengl.hpp"

namespace engine
{
	class uniform_buffer
	{
	public:
		uniform_buffer(const char* name, std::size_t size, unsigned int binding_point);

		const char* get_name() const { return m_buffer_name.c_str(); }
		std::size_t get_size() const { return m_buffer_size; }
		unsigned int get_binding_point() const { return m_binding_point; }

		void copy_data(std::size_t offset, std::size_t length, void* data) const;

	private:
		std::string m_buffer_name;
		std::size_t m_buffer_size;
		unsigned int m_binding_point;
		opengl_buffer_handle m_buffer_handle;
	};

	using uniform_buffer_ptr = std::shared_ptr<uniform_buffer>;

	uniform_buffer_ptr make_uniform_buffer(const char* name, std::size_t size, unsigned int binding_point);

	class shader_buffer
	{
	public:
		shader_buffer(const char* name, std::size_t size, unsigned int binding_point);

		const char* get_name() const { return m_buffer_name.c_str(); }
		std::size_t get_size() const { return m_buffer_size; }
		unsigned int get_binding_point() const { return m_binding_point; }

		void copy_data(std::size_t offset, std::size_t length, void* data) const;

	private:
		std::string m_buffer_name;
		std::size_t m_buffer_size;
		unsigned int m_binding_point;
		opengl_buffer_handle m_buffer_handle;
	};

	using shader_buffer_ptr = std::shared_ptr<shader_buffer>;

	shader_buffer_ptr make_shader_buffer(const char* name, std::size_t size, unsigned int binding_point);

	class shader
	{
	public:
		shader(const char* vertex_shader_source, const char* fragment_shader_source);;

		void set_bool(const char* name, bool value) const;
		void set_int(const char* name, int value) const;
		void set_float(const char* name, float value) const;

		void set_vec2(const char* name, real x, real y) const;
		void set_vec3(const char* name, real x, real y, real z) const;
		void set_vec4(const char* name, real x, real y, real z, real w) const;

		void set_vec2(const char* name, const real* vec) const;
		void set_vec3(const char* name, const real* vec) const;
		void set_vec4(const char* name, const real* vec) const;

		void set_mat2(const char* name, const real* mat) const;
		void set_mat3(const char* name, const real* mat) const;
		void set_mat4(const char* name, const real* mat) const;

		void bind_uniform_block(const char* name, unsigned int binding_point) const;
		void bind_shader_block(const char* name, unsigned int binding_point) const;

		void connect_uniform_block(const uniform_buffer_ptr& uniform_buffer) const;
		void connect_shader_block(const shader_buffer_ptr& shader_buffer) const;

		void make_current() const;

	private:
		opengl_shader_handle m_vertex_shader_handle;
		opengl_shader_handle m_fragment_shader_handle;
		opengl_program_handle m_program_handle;
	};

	using shader_ptr = std::shared_ptr<shader>;

	shader_ptr load_shader(const char* vertex_shader_file, const char* fragment_shader_file);
}
