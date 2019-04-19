#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "texture.hpp"

using namespace std;

namespace engine
{
	texture_map::texture_map(int width, int height, int channels, const unsigned char* data, bool mipmaps, bool gamma_correction)
	{
		assert(width > 0 && height > 0 && (channels == 3 || channels == 4) && data != nullptr);

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

	texture_map::texture_map(int width, int height, int channels, const float* data, bool mipmaps)
	{
		assert(width > 0 && height > 0 && (channels == 3 || channels == 4) && data != nullptr);

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

	void texture_map::build_texture(GLenum internal_format, int width, int height, int channels, GLenum data_format, GLenum data_type, const void* data, bool mipmaps)
	{
		glGenTextures(1, &m_texture_handle);
		glBindTexture(GL_TEXTURE_2D, m_texture_handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, data_type, data);

		if(mipmaps)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void texture_map::bind(int unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_texture_handle);
	}

	texture_cube_map::texture_cube_map(const texture_cube_map_data_array<unsigned char>& data_array, bool gamma_correction)
	{
		glGenTextures(1, &m_texture_handle);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_handle);

		unsigned int face_index{};

		for(auto& data : data_array)
		{
			assert(data.width > 0 && data.height > 0 && (data.channels == 3 || data.channels == 4) && data.data != nullptr);

			GLenum data_format;
			GLenum internal_format;
			switch(data.channels)
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

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face_index++, 0, internal_format, data.width, data.height, 0, data_format, GL_UNSIGNED_BYTE, data.data);
		}

		apply_common_parameters();

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	texture_cube_map::texture_cube_map(const texture_cube_map_data_array<float>& data_array)
	{
		glGenTextures(1, &m_texture_handle);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_handle);

		unsigned int face_index{};

		for(auto& data : data_array)
		{
			assert(data.width > 0 && data.height > 0 && (data.channels == 3 || data.channels == 4) && data.data != nullptr);

			GLenum data_format;
			GLenum internal_format;
			switch(data.channels)
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

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face_index++, 0, internal_format, data.width, data.height, 0, data_format, GL_FLOAT, data.data);
		}

		apply_common_parameters();

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void texture_cube_map::apply_common_parameters() const
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void texture_cube_map::bind(int unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_handle);
	}

	template<typename T> struct STBIReleasePolicy { static void Execute(T ptr) noexcept { stbi_image_free(ptr); } };
	template<typename T> using stbi_handle = handle<T, STBIReleasePolicy>;

	texture_ptr load_texture_map_ldr(const char* texture_file, bool mipmaps, bool gamma_correction, int desired_channels)
	{
		int width{}, height{}, channels{};

		cout << "Loading LDR " << texture_file << "..." << endl;

		stbi_set_flip_vertically_on_load(true);
		stbi_handle<unsigned char*> data{ stbi_load(texture_file, &width, &height, &channels, desired_channels) };
		if(data == nullptr) throw runtime_error((string("Error loading texture! ") + stbi_failure_reason()).c_str());

		if(desired_channels == 0) desired_channels = channels;

		cout << "\twidth: " << width << endl <<
			"\theight: " << height << endl <<
			"\tinput channels: " << channels << endl <<
			"\toutput channels: " << desired_channels << endl;

		return make_shared<texture_map>(width, height, desired_channels, data, mipmaps, gamma_correction);
	}

	texture_ptr load_texture_map_hdr(const char* texture_file, bool mipmaps, int desired_channels)
	{
		int width{}, height{}, channels{};

		cout << "Loading HDR " << texture_file << "..." << endl;

		stbi_set_flip_vertically_on_load(true);
		stbi_handle<float*> data{ stbi_loadf(texture_file, &width, &height, &channels, desired_channels) };
		if(data == nullptr) throw runtime_error((string("Error loading texture! ") + stbi_failure_reason()).c_str());

		if(desired_channels == 0) desired_channels = channels;

		cout << "\twidth: " << width << endl <<
			"\theight: " << height << endl <<
			"\tinput channels: " << channels << endl <<
			"\toutput channels: " << desired_channels << endl;

		return make_shared<texture_map>(width, height, desired_channels, data, mipmaps);
	}

	texture_ptr load_texture_map(const char* texture_file, bool mipmaps, bool gamma_correction, int desired_channels, bool always_load_as_hdr)
	{
		bool hdr = always_load_as_hdr ? true : stbi_is_hdr(texture_file);
		if(hdr) return load_texture_map_hdr(texture_file, mipmaps, desired_channels);
		else return load_texture_map_ldr(texture_file, mipmaps, gamma_correction, desired_channels);
	}

	texture_ptr load_texture_cube_map_ldr(const texture_cube_map_files& files, bool gamma_correction, int desired_channels)
	{
		using handle = stbi_handle<unsigned char*>;
		using stbi_handle_ptr = shared_ptr<handle>;
		using stbi_handle_ptr_vector = vector<stbi_handle_ptr>;

		stbi_handle_ptr_vector stbi_handles;
		stbi_handles.reserve(6);

		texture_cube_map_data_array<unsigned char> data_array;
		unsigned int array_index{};

		for(auto& texture_file : files)
		{
			int width{}, height{}, channels{};

			cout << "Loading LDR " << texture_file << "..." << endl;

			stbi_set_flip_vertically_on_load(false);
			auto data = make_shared<handle>(stbi_load(texture_file, &width, &height, &channels, desired_channels));
			if(*data == nullptr) throw runtime_error((string("Error loading texture! ") + stbi_failure_reason()).c_str());
			stbi_handles.push_back(data);

			if(desired_channels == 0) desired_channels = channels;

			cout << "\twidth: " << width << endl <<
				"\theight: " << height << endl <<
				"\tinput channels: " << channels << endl <<
				"\toutput channels: " << desired_channels << endl;

			data_array[array_index++] = texture_cube_map_data<unsigned char>{ width, height, desired_channels, *data };
		}

		return make_shared<texture_cube_map>(data_array, gamma_correction);
	}

	texture_ptr load_texture_cube_map_hdr(const texture_cube_map_files& files, int desired_channels)
	{
		using handle = stbi_handle<float*>;
		using stbi_handle_ptr = shared_ptr<handle>;
		using stbi_handle_ptr_vector = vector<stbi_handle_ptr>;

		stbi_handle_ptr_vector stbi_handles;
		stbi_handles.reserve(6);

		texture_cube_map_data_array<float> data_array;
		unsigned int array_index{};

		for(auto& texture_file : files)
		{
			int width{}, height{}, channels{};

			cout << "Loading HDR " << texture_file << "..." << endl;

			stbi_set_flip_vertically_on_load(false);
			auto data = make_shared<handle>(stbi_loadf(texture_file, &width, &height, &channels, desired_channels));
			if(*data == nullptr) throw runtime_error((string("Error loading texture! ") + stbi_failure_reason()).c_str());
			stbi_handles.push_back(data);

			if(desired_channels == 0) desired_channels = channels;

			cout << "\twidth: " << width << endl <<
				"\theight: " << height << endl <<
				"\tinput channels: " << channels << endl <<
				"\toutput channels: " << desired_channels << endl;

			data_array[array_index++] = texture_cube_map_data<float>{ width, height, desired_channels, *data };
		}

		return make_shared<texture_cube_map>(data_array);
	}

	texture_ptr load_texture_cube_map(const texture_cube_map_files& files, bool gamma_correction, int desired_channels, bool always_load_as_hdr)
	{
		bool all_are_hdr = true;
		for(auto& texture_file : files)
		{
			if(!stbi_is_hdr(texture_file))
			{
				all_are_hdr = false;
				break;
			}
		}

		if(always_load_as_hdr || all_are_hdr) return load_texture_cube_map_hdr(files, desired_channels);
		else return load_texture_cube_map_ldr(files, gamma_correction, desired_channels);
	}
}
