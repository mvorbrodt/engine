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

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

using namespace std;
using namespace engine;

engine::point eye{0.0, 5.0, 10.0};
engine::pov camera(WINDOW_WIDTH, WINDOW_HEIGHT, 60.0f, 1.0, 100.0, eye, ORIGIN - eye, UNIT_Y);

real vertices[] =
{
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
};

unsigned char colors[] =
{
	255, 0, 0,
	0, 255, 0,
	0, 0, 255,
	255, 255, 255
};

real texture_coords[] =
{
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
	0.0, 1.0
};

unsigned int indices[] =
{
	0, 1, 2, 2, 3, 0
};

unsigned int VBO_V, VBO_T, VBO_C, VBO_I;
unsigned int VAO;

engine::shader_ptr s1, s2;
engine::texture_ptr t1, t2, t3;

void error(int error, const char* description)
{
	cout << "GLFW error code: " << error << ", description: " << description << endl;
}

void init()
{
	try
	{
		s1 = load_shader("data/shaders/test_vertex_shader.vs", "data/shaders/test_fragment_shader_1.fs");
		s2 = load_shader("data/shaders/test_vertex_shader.vs", "data/shaders/test_fragment_shader_2.fs");
		t1 = load_texture("data/textures/avatar.png", true);
		t2 = load_texture("data/textures/cpp.png", true);
		t3 = load_texture("data/textures/mask.png", true);

		s1->use();
		s1->bind_texture("ourTexture1", 0);
		s1->bind_texture("ourTexture2", 1);

		s2->use();
		s2->bind_texture("ourTexture1", 0);
	}
	catch(exception& e)
	{
		cerr << e.what() << endl;
		exit(-1);
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO_V);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_V);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBO_T);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_T);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_C);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_UNSIGNED_BYTE, GL_TRUE, 3 * sizeof(unsigned char), (void*)0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &VBO_I);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_I);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	camera.set_width(w);
	camera.set_height(h);
	s1->use();
	s1->load_matrix("Projection", camera.projection_matrix());
	s2->use();
	s2->load_matrix("Projection", camera.projection_matrix());
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
	s1->use();
	s1->load_matrix("Projection", camera.projection_matrix());
	s2->use();
	s2->load_matrix("Projection", camera.projection_matrix());
}

void draw()
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAO);

	s2->use();
	t1->bind(0);

	static float rotation{};
	engine::system l1, l2, l3, l4, l5;
	quaternion q(++rotation, UNIT_Y);
	l1.rotate(q);
	l2.rotate(q);
	l3.rotate(q);
	l4.rotate(q);
	l5.rotate(q);

	l2.translate(5 *  UNIT_X);
	l3.translate(5 * -UNIT_X);
	l4.translate(5 * -UNIT_Z);
	l5.translate(5 *  UNIT_Z);

	auto mv = camera.view_matrix() * l1.to_global();
	s2->load_matrix("ModelView", mv);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

	s1->use();
	t2->bind(0);
	t3->bind(1);

	mv = camera.view_matrix() * l2.to_global();
	s1->load_matrix("ModelView", mv);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

	mv = camera.view_matrix() * l3.to_global();
	s1->load_matrix("ModelView", mv);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

	mv = camera.view_matrix() * l4.to_global();
	s1->load_matrix("ModelView", mv);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

	mv = camera.view_matrix() * l5.to_global();
	s1->load_matrix("ModelView", mv);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

}

int main(int argc, char** argv)
{
	glfwSetErrorCallback(error);
	if(!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

	init_opengl();
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
	glDeleteBuffers(1, &VBO_C);

	glfwTerminate();
}
