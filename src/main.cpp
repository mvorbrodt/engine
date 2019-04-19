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

engine::point light{0.0f, 0.0f, 25.0f};
engine::point eye{0.0f, 10.0f, 45.0f};
engine::pov camera(WINDOW_WIDTH, WINDOW_HEIGHT, 47.0f, 1.0f, 1000.0f, eye, point(0.0f, 2.0f, 0.0f) - eye, UNIT_Y);

bool points = false;

engine::uniform_buffer_ptr ub1, ub2;
engine::shader_ptr s, cube_shader;
engine::texture_ptr t, n, cube_texture, cube_map;
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
		ub1 = make_uniform_buffer("Matrices", 2 * sizeof(matrix), 0);
		ub2 = make_uniform_buffer("Lights", sizeof(point), 1);

		cube = make_flat_vertex_array(make_flat_model_data(
			cube_vertices,
			cube_colors,
			cube_texcoords,
			{}, {}, {}));
		cube_shader = load_shader("data/shaders/cube.vs", "data/shaders/cube.fs");
		cube_shader->set_int("texture1", 0);
		cube_shader->connect_uniform_block(ub1);

		cube_texture = load_texture_map("data/textures/cpp.png");

		cube_map = load_texture_cube_map
		(
			{
				"data/textures/skybox2/skybox_right.jpg",
				"data/textures/skybox2/skybox_left.jpg",
				"data/textures/skybox2/skybox_top.jpg",
				"data/textures/skybox2/skybox_bottom.jpg",
				"data/textures/skybox2/skybox_back.jpg",
				"data/textures/skybox2/skybox_front.jpg"
			}, true
		);

		s = load_shader("data/shaders/test_vertex_shader.vs", "data/shaders/test_fragment_shader.fs");
		s->set_bool("Reflect", false);
		s->set_int("texture1", 0);
		s->set_int("texture2", 1);
		s->set_int("texture3", 2);
		s->connect_uniform_block(ub1);
		s->connect_uniform_block(ub2);

		t = load_texture_map("data/textures/skull.jpg", true, true);
		n = load_texture_map("data/textures/skull_normal.jpg", false, false);
		auto data = load_indexed_model_data("data/models/skull.obj");
		for(auto& d : data) v.push_back(make_indexed_vertex_array(d));

		l.rotate(-90, UNIT_X);
		l.translate(10 * -UNIT_Y);
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
			case GLFW_KEY_3: points = true; break;
			case GLFW_KEY_4: points = false; break;
			case GLFW_KEY_5: s->set_bool("Reflect", false); break;
			case GLFW_KEY_6: s->set_bool("Reflect", true); break;
			case GLFW_KEY_Q: l.rotate( 10, UNIT_Y); break;
			case GLFW_KEY_E: l.rotate(-10, UNIT_Y); break;
			case GLFW_KEY_A: camera.move( 0,  0.5f); break;
			case GLFW_KEY_D: camera.move( 0, -0.5f); break;
			case GLFW_KEY_W: camera.move( 0.5f,  0); break;
			case GLFW_KEY_S: camera.move(-0.5f,  0); break;
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
		last_x = (int)x;
		last_y = (int)y;
		first = false;
	}

	int dx = (int)x - last_x;
	int dy = (int)y - last_y;

	camera.turn(-dx / 5.0f);
	camera.look(-dy / 5.0f);

	last_x = (int)x;
	last_y = (int)y;
}

void scroll(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.set_fov(camera.get_fov() - (real)yoffset);
}

void draw()
{
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ub1->copy_data(0, sizeof(matrix), camera.projection_matrix().data());
	ub1->copy_data(sizeof(matrix), sizeof(matrix), camera.view_matrix().data());
	ub2->copy_data(0, sizeof(point), light.data());

	light *= rotate(1, UNIT_Y);
	light_system = engine::system(IDENTITY_AXIS, light);
	//light_system.scale(5.0f);
	l.rotate(-0.25, UNIT_Y);

	cube_shader->set_mat4("Model", light_system.to_global().data());
	cube_shader->make_current();
	cube_texture->bind(0);
	cube->draw(GL_TRIANGLES);

	s->set_mat4("Model", l.to_global().data());
	s->make_current();
	t->bind(0);
	n->bind(1);
	cube_map->bind(2);

	for(auto& m : v) m->draw(points ? GL_POINTS : GL_TRIANGLES);
}

int main(int argc, char** argv)
{
	glfwSetErrorCallback(error);
	if(!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
#endif
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
