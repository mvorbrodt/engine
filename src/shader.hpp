#pragma once

#include <memory>
#include "types.hpp"
#include "opengl.hpp"

namespace engine
{
	class shader
	{
	public:
		shader(const char* vertex_shader_source, const char* fragment_shader_source);;

		void use() const;

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

		void bind_texture(const char* name, int unit) const;

	private:
		opengl_shader_handle m_vertex_shader_handle;
		opengl_shader_handle m_fragment_shader_handle;
		opengl_program_handle m_program_handle;
	};

	using shader_ptr = std::shared_ptr<shader>;

	shader_ptr load_shader(const char* vertex_shader_file, const char* fragment_shader_file);
}
