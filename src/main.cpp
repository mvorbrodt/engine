#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "opengl.hpp"
#include "types.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "quaternion.hpp"
#include "matrix.hpp"
#include "axis.hpp"
#include "system.hpp"
#include "pov.hpp"
#include "transforms.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"
#include "load_model.hpp"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

using namespace std;
using namespace engine;

engine::point eye{0.0, 1.0, 3.0};
engine::pov camera(WINDOW_WIDTH, WINDOW_HEIGHT, 45.0f, 1.0, 100.0, eye, point{0.0, 0.5, 0.0} - eye, UNIT_Y);

engine::shader_ptr s;
engine::texture_ptr t;
engine::vertex_arrays v;
engine::system l;

void error(int error, const char* description)
{
	cout << "GLFW error code: " << error << ", description: " << description << endl;
}

void init()
{
	try
	{
		s = load_shader("data/shaders/test_vertex_shader.vs", "data/shaders/test_fragment_shader.fs");
		t = load_texture("data/textures/chalet.jpg", true, true);
		auto data = load_model("data/models/chalet.obj");
		for(auto& d : data) v.push_back(make_vertex_array(d));

		s->use();
		s->set_float("brightness", 1.0);
		l.rotate(-90, UNIT_X);
		l.rotate( 90, UNIT_Y);
	}
	catch(exception& e)
	{
		cerr << e.what() << endl;
		exit(-1);
	}
}

void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	camera.set_width(w);
	camera.set_height(h);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch(key)
		{
			case GLFW_KEY_1: s->use(); s->set_bool("show_normal", false); break;
			case GLFW_KEY_2: s->use(); s->set_bool("show_normal", true); break;
			case GLFW_KEY_3: s->use(); s->set_float("brightness", 1.00); break;
			case GLFW_KEY_4: s->use(); s->set_float("brightness", 0.75); break;
			case GLFW_KEY_5: s->use(); s->set_float("brightness", 0.50); break;
			case GLFW_KEY_6: s->use(); s->set_float("brightness", 0.25); break;
			case GLFW_KEY_Q: l.rotate( 10, UNIT_Y); break;
			case GLFW_KEY_E: l.rotate(-10, UNIT_Y); break;
			case GLFW_KEY_A: camera.move( 0,  0.5); break;
			case GLFW_KEY_D: camera.move( 0, -0.5); break;
			case GLFW_KEY_W: camera.move( 0.5,  0); break;
			case GLFW_KEY_S: camera.move(-0.5,  0); break;
			case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true); break;
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

	int dx = x - last_x;
	int dy = y - last_y;

	camera.turn(-dx / 5.0f);
	camera.look(-dy / 5.0f);

	last_x = x;
	last_y = y;
}

void scroll(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.set_fov(camera.get_fov() - yoffset);
}

void draw()
{
	glClearColor(0.25, 0.25, 0.25, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	s->use();
	s->set_mat4("Projection", camera.projection_matrix().data());
	s->set_mat4("Camera", camera.view_matrix().data());
	s->set_mat4("Model", l.to_global().data());
	s->bind_texture("texture1", 0);
	t->bind(0);

	for(auto& m : v) m->draw();
}

int main(int argc, char** argv)
{
	glfwSetErrorCallback(error);
	if(!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 16);
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Engine Test", NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, keyboard);
	glfwSetCursorPosCallback(window, mouse);
	glfwSetScrollCallback(window, scroll);
	glfwSetFramebufferSizeCallback(window, reshape);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	opengl_defaults();
	init();
	reshape(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	while (!glfwWindowShouldClose(window))
	{
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}
