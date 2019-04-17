#pragma once

#include <memory>

namespace engine
{
	class texture
	{
	public:
		texture(int width, int height, int channels, const unsigned char* data, bool gamma_correction, bool mipmaps);
		texture(int width, int height, int channels, const float* data, bool mipmaps);
		texture(const texture&) = delete;
		texture(texture&&) = delete;
		texture& operator = (const texture&) = delete;
		texture& operator = (texture&&) = delete;
		~texture();

		void bind(int unit) const;

	private:
		void build_texture(GLenum internal_format, int width, int height, int channels, GLenum data_format, GLenum data_type, const void* data, bool mipmaps);
		unsigned int m_texture_handle = 0;
	};

	using texture_ptr = std::shared_ptr<texture>;

	texture_ptr load_texture(const char* texture_file, bool gamma_correction, bool mipmaps, int desired_channels = 0, bool always_load_as_hdr = false);
}
