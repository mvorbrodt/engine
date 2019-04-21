#pragma once

#include <array>
#include <memory>
#include "opengl.hpp"

namespace engine
{
	class texture
	{
	public:
		virtual ~texture() {}

		virtual void bind(int unit) const = 0;
	};

	using texture_ptr = std::shared_ptr<texture>;

	using texture_cube_map_files = std::array<const char*, 6>;

	template<typename T>
	struct texture_cube_map_data
	{
		int width;
		int height;
		int channels;
		const T* data;
	};

	template<typename T> using texture_cube_map_data_array = std::array<texture_cube_map_data<T>, 6>;

	texture_ptr make_texture_map(int width, int height, int channels, const unsigned char* data, bool mipmaps, bool gamma_correction);
	texture_ptr make_texture_map(int width, int height, int channels, const float* data, bool mipmaps);

	texture_ptr make_texture_cube_map(const texture_cube_map_data_array<unsigned char>& data_array, bool gamma_correction);
	texture_ptr make_texture_cube_map(const texture_cube_map_data_array<float>& data_array);

	class texture_map : public texture
	{
	public:
		texture_map(int width, int height, int channels, const unsigned char* data, bool mipmaps, bool gamma_correction);
		texture_map(int width, int height, int channels, const float* data, bool mipmaps);

		void bind(int unit) const override;

	private:
		void build_texture(GLenum internal_format, int width, int height, int channels, GLenum data_format, GLenum data_type, const void* data, bool mipmaps);

		opengl_texture_handle m_texture_handle;
	};

	class texture_cube_map : public texture
	{
	public:
		texture_cube_map(const texture_cube_map_data_array<unsigned char>& data_array, bool gamma_correction);
		texture_cube_map(const texture_cube_map_data_array<float>& data_array);

		void bind(int unit) const override;

	private:
		void apply_common_parameters() const;

		opengl_texture_handle m_texture_handle;
	};

	texture_ptr load_texture_map(const char* texture_file, bool mipmaps = true, bool gamma_correction = false, int desired_channels = 0, bool always_load_as_hdr = false);
	texture_ptr load_texture_cube_map(const texture_cube_map_files& files, bool gamma_correction = false, int desired_channels = 0, bool always_load_as_hdr = false);
}
