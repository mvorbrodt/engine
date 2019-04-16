#include <cassert>
#include <string>
#include <stdexcept>
#include <glad/glad.h>
#include <stb_image.h>
#include "texture.hpp"

using namespace std;

namespace engine
{
	texture::texture(int width, int height, int channels, const unsigned char* data, bool mipmaps)
	{
		assert(width > 0 && height > 0 && (channels == 3 || channels == 4));
		auto type = (channels == 4) ? GL_RGBA : GL_RGB;
		glGenTextures(1, &m_handle);
		glBindTexture(GL_TEXTURE_2D, m_handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
		if(glGetError() != GL_NO_ERROR) throw runtime_error("glTexImage2D failed!");
		if(mipmaps) glGenerateMipmap(GL_TEXTURE_2D);
	}

	texture::~texture()
	{
		glDeleteTextures(1, &m_handle);
	}

	void texture::bind(int unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_handle);
	}

	texture_ptr load_texture(const char* texture_file, bool mipmaps)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(texture_file, &width, &height, &channels, 0);
		if(data == nullptr) throw runtime_error((string("Error loading texture ") + texture_file).c_str());
		return make_shared<texture>(width, height, channels, data, mipmaps);
	}
}
