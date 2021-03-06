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

	void init_opengl_defaults()
	{
		cout << "VENDOR   : " << glGetString(GL_VENDOR) << endl;
		cout << "RENDERED : " << glGetString(GL_RENDERER) << endl;
		cout << "VERSION  : " << glGetString(GL_VERSION) << endl;
		cout << "GLSL     : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(OpenGLDebugMessageCallback, 0);

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		//glFrontFace(GL_CCW);
		glEnable(GL_FRAMEBUFFER_SRGB);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_MULTISAMPLE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
