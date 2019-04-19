#pragma once

#include <vector>
#include <initializer_list>
#include "opengl.hpp"

namespace engine
{
	class buffer
	{
	public:
		virtual ~buffer() {}

		virtual const char* element_name() const = 0;
		virtual std::size_t element_size() const = 0;
		virtual std::size_t component_count() const = 0;

		virtual std::size_t size() const = 0;
		virtual void reserve(std::size_t count) = 0;

		virtual void* data() = 0;
		virtual const void* data() const = 0;

		virtual GLenum opengl_buffer_type() const = 0;
		virtual GLenum opengl_element_type() const = 0;
	};

	template<typename T, const char* T_name, std::size_t T_component_count, GLenum buffer_type, GLenum element_type>
	class buffer_t : public buffer
	{
	public:
		buffer_t() = default;
		buffer_t(std::initializer_list<T> l) : m_buffer(l) {}

		const char* element_name() const override { return T_name; }
		std::size_t element_size() const override { return sizeof(T); }
		std::size_t component_count() const override { return T_component_count; }

		std::size_t size() const override { return m_buffer.size(); }
		void reserve(std::size_t count) override { m_buffer.reserve(count); }

		void* data() override { return m_buffer.data(); }
		const void* data() const override { return m_buffer.data(); }

		GLenum opengl_buffer_type() const override { return buffer_type; }
		GLenum opengl_element_type() const override { return element_type; }

		void push_back(const T& element) { m_buffer.push_back(element); }
		void push_back(T&& element) { m_buffer.push_back(element); }

		T& operator[](std::size_t i) { return m_buffer[i]; }
		const T& operator[](std::size_t i) const { return m_buffer[i]; }

	private:
		std::vector<T> m_buffer;
	};

	inline const char k_point_buffer_name[]     = "point";
	inline const char k_color_buffer_name[]     = "color";
	inline const char k_texcoord_buffer_name[]  = "texcoord";
	inline const char k_normal_buffer_name[]    = "normal";
	inline const char k_tangent_buffer_name[]   = "tangent";
	inline const char k_bitangent_buffer_name[] = "bitangent";
	inline const char k_index_buffer_name[]     = "index";

	using index_buffer = buffer_t<int, k_index_buffer_name, 1, GL_ELEMENT_ARRAY_BUFFER, GL_INT>;
}
