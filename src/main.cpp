#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "types.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "quaternion.hpp"
#include "matrix.hpp"
#include "axis.hpp"
#include "system.hpp"
#include "pov.hpp"
#include "transforms.hpp"

using namespace std;
using namespace engine;

engine::pov camera({0.0, 0.0, 3.0}, -UNIT_Z, UNIT_Y);

void error(int error, const char* description)
{
	cout << "GLFW error code: " << error << ", description: " << description << endl;
}

void init()
{
	cout << glGetString(GL_VENDOR) << endl;
	cout << glGetString(GL_RENDERER) << endl;
	cout << glGetString(GL_VERSION) << endl;
	cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	auto r = (real)w / (real)h;
	auto p = projection(60, r, 1, 100);
	glLoadMatrixf(p.data());
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS)
	{
		switch(key)
		{
			case GLFW_KEY_Q: camera.roll(-10); break;
			case GLFW_KEY_E: camera.roll( 10); break;
			case GLFW_KEY_A: camera.move( 0,  1); break;
			case GLFW_KEY_D: camera.move( 0, -1); break;
			case GLFW_KEY_W: camera.move( 1,  0); break;
			case GLFW_KEY_S: camera.move(-1,  0); break;
		}
	}
}

void mouse(GLFWwindow* window, double x, double y)
{
	static bool first = true;
	static int last_x = 0;
	static int last_y = 0;

	if(first)
	{
		last_x = x;
		last_y = y;
		first = false;
	}

	real dx = x - last_x;
	real dy = y - last_y;

	camera.turn(-dx / 3.0f);
	camera.look(-dy / 3.0f);

	last_x = x;
	last_y = y;
}

void draw()
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	/*static float rotation{};
	quaternion q1(30, UNIT_X);
	quaternion q2(rotation, UNIT_Y);
	auto r = rotate(q1 * q2);
	auto t = translate(UNIT_Z * -3);
	auto mv = t * r;
	++rotation;
	glLoadMatrixf(mv.data());*/

	auto q1 = quaternion(45, UNIT_Y);
	auto q2 = quaternion(90, UNIT_X);
	static real a = 0.0;
	a+=0.01;
	real i = (sin(a) + 1.0) / 2.0;
	auto q = interpolate(i, q1, q2);
	auto t = translate(UNIT_Z * -3);
	auto mv = t * rotate(q);
	glLoadMatrixf(mv.data());

	//glLoadMatrixf(camera.view_matrix().data());

	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(2);
	glutWireTeapot(1.0);

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glfwSetErrorCallback(error);
	if(!glfwInit()) return -1;
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello 3D Engine", NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, keyboard);
	glfwSetCursorPosCallback(window, mouse);
	glfwSetFramebufferSizeCallback(window, reshape);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	init();
	reshape(window, 640, 480);

	while (!glfwWindowShouldClose(window))
	{
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}
