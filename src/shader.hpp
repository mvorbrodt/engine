#pragma once

#include <memory>
#include "types.hpp"

namespace engine
{
	class shader
	{
	public:
		shader(const char* vertex_shader_source, const char* fragment_shader_source);
		shader(const shader&) = delete;
		shader(shader&&) = delete;
		shader& operator = (const shader&) = delete;
		shader& operator = (shader&&) = delete;
		~shader();

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
		int m_vertex_shader_handle = 0;
		int m_fragment_shader_handle = 0;
		int m_program_handle = 0;
	};

	using shader_ptr = std::shared_ptr<shader>;

	shader_ptr load_shader(const char* vertex_shader_file, const char* fragment_shader_file);
}
