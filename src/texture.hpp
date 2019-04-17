#pragma once

#include <memory>

namespace engine
{
	class texture_map
	{
	public:
		texture_map(int width, int height, int channels, const unsigned char* data, bool gamma_correction, bool mipmaps);
		texture_map(int width, int height, int channels, const float* data, bool mipmaps);
		texture_map(const texture_map&) = delete;
		texture_map(texture_map&&) = delete;
		texture_map& operator = (const texture_map&) = delete;
		texture_map& operator = (texture_map&&) = delete;
		~texture_map();

		void bind(int unit) const;

	private:
		void build_texture(GLenum internal_format, int width, int height, int channels, GLenum data_format, GLenum data_type, const void* data, bool mipmaps);
		unsigned int m_texture_handle = 0;
	};

	using texture_map_ptr = std::shared_ptr<texture_map>;

	texture_map_ptr load_texture_map(const char* texture_file, bool gamma_correction, bool mipmaps, int desired_channels = 0, bool always_load_as_hdr = false);
}
