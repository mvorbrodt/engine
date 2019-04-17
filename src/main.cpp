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

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

using namespace std;
using namespace engine;

engine::point eye{0.0, 1.0, 3.0};
engine::pov camera(WINDOW_WIDTH, WINDOW_HEIGHT, 60.0f, 1.0, 100.0, eye, ORIGIN - eye, UNIT_Y);

unsigned int VBO_V, VBO_N, VBO_T, VBO_I;
unsigned int VAO;
unsigned int triangles{};

engine::shader_ptr s;
engine::texture_ptr t;

void error(int error, const char* description)
{
	cout << "GLFW error code: " << error << ", description: " << description << endl;
}

void init()
{
	try
	{
		s = load_shader("data/shaders/test_vertex_shader.vs", "data/shaders/test_fragment_shader.fs");
		t = load_texture("data/textures/chalet.jpg", false, false);
		auto [pb, nb, tb, ib] = load_model_file("data/models/chalet.obj");
		triangles = ib.size();

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO_V);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_V);
		glBufferData(GL_ARRAY_BUFFER, pb.size() * sizeof(real), pb.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &VBO_N);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_N);
		glBufferData(GL_ARRAY_BUFFER, nb.size() * sizeof(real), nb.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &VBO_T);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_T);
		glBufferData(GL_ARRAY_BUFFER, tb.size() * sizeof(real), tb.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &VBO_I);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_I);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ib.size() * sizeof(int), ib.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAO);

	s->use();
	s->set_matrix("Projection", camera.projection_matrix());
	s->set_matrix("Camera", camera.view_matrix());
	s->bind_texture("ourTexture", 0);
	t->bind(0);

	static float rotation{};
	engine::system l1;
	quaternion q(++rotation, UNIT_Y);
	l1.rotate(-90, UNIT_X);
	l1.rotate(q);

	s->set_matrix("Model", l1.to_global());
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, triangles, GL_UNSIGNED_INT, (void*)0);
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
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello 3D Engine", NULL, NULL);
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

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_V);
	glDeleteBuffers(1, &VBO_T);
	glDeleteBuffers(1, &VBO_N);
	glDeleteBuffers(1, &VBO_I);

	glfwTerminate();
}
