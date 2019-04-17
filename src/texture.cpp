#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include "opengl.hpp"
#include "handle.hpp"
#include "texture.hpp"

template<typename T> struct STBIReleasePolicy { static void Execute(T ptr) noexcept { stbi_image_free(ptr); } };
using stbi_handle_t = handle<unsigned char*, NoOpPolicy, STBIReleasePolicy>;

using namespace std;

namespace engine
{
	texture::texture(int width, int height, int channels, const unsigned char* data, bool gamma_correction, bool mipmaps)
	{
		assert(width > 0 && height > 0 && (channels == 3 || channels == 4));

		GLenum data_format;
		GLenum internal_format;
		switch(channels)
		{
			case 3:
				data_format = GL_RGB;
				internal_format = gamma_correction ? GL_SRGB : GL_RGB;
				break;
			case 4:
				data_format = GL_RGBA;
				internal_format = gamma_correction ? GL_SRGB_ALPHA : GL_RGBA;
				break;
		}

		glGenTextures(1, &m_texture_handle);
		glBindTexture(GL_TEXTURE_2D, m_texture_handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);
		GLenum error_code = glGetError();
		if(error_code != GL_NO_ERROR) throw opengl_exception("glTexImage2D failed!", error_code);

		if(mipmaps)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	texture::~texture()
	{
		glDeleteTextures(1, &m_texture_handle);
	}

	void texture::bind(int unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_texture_handle);
	}

	texture_ptr load_texture(const char* texture_file, bool gamma_correction, bool mipmaps)
	{
		int width{}, height{}, channels{};
		stbi_set_flip_vertically_on_load(true);
		stbi_handle_t data{ stbi_load(texture_file, &width, &height, &channels, 0) };
		if(data == nullptr) throw runtime_error((string("Error loading texture ") + texture_file).c_str());

		cout << texture_file << ", width: " << width << ", height: " << height << ", channels: " << channels << endl;

		return make_shared<texture>(width, height, channels, data, gamma_correction, mipmaps);
	}
}
