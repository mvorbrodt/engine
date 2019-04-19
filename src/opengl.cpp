#include <iostream>
#include "opengl.hpp"

using namespace std;

namespace engine
{
	void GLAPIENTRY OpenGLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		cerr << "OpenGL:" << (type == GL_DEBUG_TYPE_ERROR ? " ERROR " : " ") <<
			"type = " << type <<
			", severity = " << severity <<
			", message = " << message << endl;
	}

	void opengl_defaults()
	{
		cout << "VENDOR   : " << glGetString(GL_VENDOR) << endl;
		cout << "RENDERED : " << glGetString(GL_RENDERER) << endl;
		cout << "VERSION  : " << glGetString(GL_VERSION) << endl;
		cout << "GLSL     : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

		if(glDebugMessageCallback != nullptr)
		{
			glEnable(GL_DEBUG_OUTPUT);
			//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLDebugMessageCallback, 0);
		}

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_MULTISAMPLE);
		//glEnable(GL_FRAMEBUFFER_SRGB);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
