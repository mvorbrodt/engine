#pragma once

#include <memory>
#include "opengl.hpp"

namespace engine
{
	class texture_map
	{
	public:
		texture_map(int width, int height, int channels, const unsigned char* data, bool gamma_correction, bool mipmaps);
		texture_map(int width, int height, int channels, const float* data, bool mipmaps);

		void bind(int unit) const;

	private:
		void build_texture(GLenum internal_format, int width, int height, int channels, GLenum data_format, GLenum data_type, const void* data, bool mipmaps);

		opengl_texture_handle m_texture_handle;
	};

	using texture_map_ptr = std::shared_ptr<texture_map>;

	texture_map_ptr load_texture_map(const char* texture_file, bool gamma_correction, bool mipmaps, int desired_channels = 0, bool always_load_as_hdr = false);
}
