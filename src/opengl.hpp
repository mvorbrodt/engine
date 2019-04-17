#pragma once

#include <stdexcept>

namespace engine
{
	class opengl_exception : public std::runtime_error
	{
	public:
		opengl_exception(const char* what, GLenum error_code)
		: runtime_error(what), m_error_code{ error_code } {}

		GLenum error_code() const { return m_error_code; }

	private:
		GLenum m_error_code;
	};

	void opengl_defaults();
}
