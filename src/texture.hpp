#pragma once

#include <memory>

namespace engine
{
	class texture
	{
	public:
		texture(int width, int height, int channels, const unsigned char* data, bool gamma_correction, bool mipmaps);
		texture(const texture&) = delete;
		texture(texture&&) = delete;
		texture& operator = (const texture&) = delete;
		texture& operator = (texture&&) = delete;
		~texture();

		void bind(int unit) const;

	private:
		unsigned int m_texture_handle;
	};

	using texture_ptr = std::shared_ptr<texture>;

	texture_ptr load_texture(const char* texture_file, bool gamma_correction, bool mipmaps);
}
