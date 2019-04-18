#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "opengl.hpp"
#include "types.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "color.hpp"
#include "texcoord.hpp"
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
#include "cube_data.hpp"

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

using namespace std;
using namespace engine;

engine::point light{0.0, 0.0, 45.0};
engine::point eye{0.0, 25.0, 45.0};
engine::pov camera(WINDOW_WIDTH, WINDOW_HEIGHT, 47.0f, 1.0, 1000.0, eye, point{0.0, 0/*11.0*/, 0.0} - eye, UNIT_Y);

engine::shader_ptr s, cube_shader;
engine::texture_map_ptr t, n;
engine::vertex_arrays v;
engine::vertex_array_ptr cube;
engine::system l, light_system;

void error(int error, const char* description)
{
	cout << "GLFW error code: " << error << ", description: " << description << endl;
}

void init()
{
	try
	{
		cube = make_vertex_array(make_model_data(
			cube_vertices,
			cube_colors,
			cube_texcoords,
			{}, {}, {},
			cube_indices));
		cube_shader = load_shader("data/shaders/cube.vs", "data/shaders/cube.fs");

		s = load_shader("data/shaders/test_vertex_shader.vs", "data/shaders/test_fragment_shader.fs");
		t = load_texture_map("data/textures/skull.jpg", false, false);
		n = load_texture_map("data/textures/skull_normal.jpg", false, false);
		auto data = load_model("data/models/skull.obj");
		for(auto& d : data) v.push_back(make_vertex_array(d));

		l.rotate(-90, UNIT_X);
		//l.rotate( 90, UNIT_Y);
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
			case GLFW_KEY_1: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
			case GLFW_KEY_2: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
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

	light *= rotate(1, UNIT_Y);
	light *= rotate(1.33, UNIT_X);
	light *= rotate(1.66, UNIT_Z);
	light_system = engine::system(IDENTITY_AXIS, light);
	l.rotate(-0.25, UNIT_Y);

	cube_shader->use();
	cube_shader->set_mat4("Projection", camera.projection_matrix().data());
	cube_shader->set_mat4("Camera", camera.view_matrix().data());
	cube_shader->set_mat4("Model", IDENTITY_MATRIX.data());//light_system.to_global().data());
	cube_shader->set_mat4("Model", light_system.to_global().data());
	cube->draw();

	s->use();
	s->set_mat4("Projection", camera.projection_matrix().data());
	s->set_mat4("Camera", camera.view_matrix().data());
	s->set_mat4("Model", l.to_global().data());
	s->set_vec3("Light", light.data());
	s->bind_texture("texture1", 0);
	s->bind_texture("texture2", 1);
	t->bind(0);
	n->bind(1);

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
