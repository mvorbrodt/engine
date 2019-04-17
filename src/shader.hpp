#pragma once

#include <memory>
#include "matrix.hpp"

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

		void set_matrix(const char* name, const matrix& m) const;
		void bind_texture(const char* name, int unit) const;

	private:
		int m_vertex_shader_handle;
		int m_fragment_shader_handle;
		int m_program_handle;
	};

	using shader_ptr = std::shared_ptr<shader>;

	shader_ptr load_shader(const char* vertex_shader_file, const char* fragment_shader_file);
}
