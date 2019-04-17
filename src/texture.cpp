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
template<typename T>
using stbi_handle_t = handle<T, NoOpPolicy, STBIReleasePolicy>;

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

		build_texture(internal_format, width, height, channels, data_format, GL_UNSIGNED_BYTE, data, mipmaps);
	}

	texture::texture(int width, int height, int channels, const float* data, bool mipmaps)
	{
		assert(width > 0 && height > 0 && (channels == 3 || channels == 4));

		GLenum data_format;
		GLenum internal_format;
		switch(channels)
		{
			case 3:
				data_format = GL_RGB;
				internal_format = GL_RGB16F;
				break;
			case 4:
				data_format = GL_RGBA;
				internal_format = GL_RGBA16F;
				break;
		}

		build_texture(internal_format, width, height, channels, data_format, GL_FLOAT, data, mipmaps);
	}

	void texture::build_texture(GLenum internal_format, int width, int height, int channels, GLenum data_format, GLenum data_type, const void* data, bool mipmaps)
	{
		glGenTextures(1, &m_texture_handle);
		glBindTexture(GL_TEXTURE_2D, m_texture_handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, data_type, data);
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

	texture_ptr load_texture_ldr(const char* texture_file, bool gamma_correction, bool mipmaps, int desired_channels)
	{
		int width{}, height{}, channels{};
		stbi_set_flip_vertically_on_load(true);
		cout << "Loading LDR " << texture_file << endl;
		stbi_handle_t<unsigned char*> data{ stbi_load(texture_file, &width, &height, &channels, desired_channels) };
		if(data == nullptr) throw runtime_error((string("Error loading texture! ") + stbi_failure_reason()).c_str());
		if(desired_channels == 0) desired_channels = channels;

		cout << "\twidth: " << width << ", height: " << height << ", input channels: " << channels << ", output channels: " << desired_channels << endl;

		return make_shared<texture>(width, height, desired_channels, data, gamma_correction, mipmaps);
	}

	texture_ptr load_texture_hdr(const char* texture_file, bool mipmaps, int desired_channels)
	{
		int width{}, height{}, channels{};
		stbi_set_flip_vertically_on_load(true);
		cout << "Loading HDR " << texture_file << endl;
		stbi_handle_t<float*> data{ stbi_loadf(texture_file, &width, &height, &channels, desired_channels) };
		if(data == nullptr) throw runtime_error((string("Error loading texture! ") + stbi_failure_reason()).c_str());
		if(desired_channels == 0) desired_channels = channels;

		cout << "\twidth: " << width << ", height: " << height << ", input channels: " << channels << ", output channels: " << desired_channels << endl;

		return make_shared<texture>(width, height, desired_channels, data, mipmaps);
	}

	texture_ptr load_texture(const char* texture_file, bool gamma_correction, bool mipmaps, int desired_channels, bool always_load_as_hdr)
	{
		bool hdr = always_load_as_hdr ? true : stbi_is_hdr(texture_file);
		if(hdr) return load_texture_hdr(texture_file, mipmaps, desired_channels);
		else return load_texture_ldr(texture_file, gamma_correction, mipmaps, desired_channels);
	}
}
