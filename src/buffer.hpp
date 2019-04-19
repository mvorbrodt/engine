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

	template<typename T, typename CT, std::size_t CC, const char* EN, GLenum BT, GLenum ET>
	class buffer_t : public buffer
	{
	public:
		static_assert(EN != nullptr, "Missing element name!");
		static_assert(sizeof(T) / sizeof(CT) == CC, "Component count mismatch!");
		static_assert(BT == GL_ARRAY_BUFFER || BT == GL_ELEMENT_ARRAY_BUFFER, "OpenGL buffer type not allowed!");
		static_assert(ET == GL_FLOAT || ET == GL_UNSIGNED_INT, "OpenGL element type not allowed!");

		buffer_t() = default;
		buffer_t(std::initializer_list<T> l) : m_buffer(l) {}

		const char* element_name() const override { return EN; }
		std::size_t element_size() const override { return sizeof(T); }
		std::size_t component_count() const override { return CC; }

		std::size_t size() const override { return m_buffer.size(); }
		void reserve(std::size_t count) override { m_buffer.reserve(count); }

		void* data() override { return m_buffer.data(); }
		const void* data() const override { return m_buffer.data(); }

		GLenum opengl_buffer_type() const override { return BT; }
		GLenum opengl_element_type() const override { return ET; }

		void push_back(const T& element) { m_buffer.push_back(element); }
		void push_back(T&& element) { m_buffer.push_back(element); }

		T& operator[](std::size_t i) { return m_buffer[i]; }
		const T& operator[](std::size_t i) const { return m_buffer[i]; }

	private:
		std::vector<T> m_buffer;
	};

	using index_buffer = buffer_t<unsigned int, unsigned int, 1, k_index_attribute_name, GL_ELEMENT_ARRAY_BUFFER, GL_UNSIGNED_INT>;
}
