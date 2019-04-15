#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "types.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "quaternion.hpp"
#include "matrix.hpp"
#include "axis.hpp"
#include "system.hpp"
#include "pov.hpp"
#include "transforms.hpp"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

using namespace std;
using namespace engine;

engine::point eye{0.0, 5.0, 10.0};
engine::pov camera(eye, ORIGIN - eye, UNIT_Y);

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
unsigned int indices[] =
{
	0, 1, 2, 2, 3, 0
};
unsigned int VBO_V, VBO_C, VBO_I;
unsigned int VAO;

const GLchar * const vs =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"out vec3 ourColor;\n"
	"uniform mat4 Projection;\n"
	"uniform mat4 ModelView;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = Projection * ModelView * vec4(aPos, 1.0);\n"
	"	ourColor = aColor;\n"
	"}\n";


const GLchar * const fs =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(ourColor, 1.0);\n"
	"}\n";

unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;

void error(int error, const char* description)
{
	cout << "GLFW error code: " << error << ", description: " << description << endl;
}

void init()
{
	cout << "VENDOR   : " << glGetString(GL_VENDOR) << endl;
	cout << "RENDERED : " << glGetString(GL_RENDERER) << endl;
	cout << "VERSION  : " << glGetString(GL_VERSION) << endl;
	cout << "GLSL     : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	glEnable(GL_DEPTH_TEST);

	int  success;
	char infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vs, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << infoLog << endl;
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fs, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << infoLog << endl;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << infoLog << endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO_V);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_V);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_C);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 3 * sizeof(unsigned char), (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &VBO_I);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_I);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	glUseProgram(shaderProgram);
	auto p = projection(60, (real)w / (real)h, 1, 100);
	auto id = glGetUniformLocation(shaderProgram, "Projection");
	glUniformMatrix4fv(id, 1, GL_FALSE, p.data());
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
	glColor3f(0.0, 0.0, 0.0);
	glMatrixMode(GL_MODELVIEW);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

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
	auto id = glGetUniformLocation(shaderProgram, "ModelView");
	glUniformMatrix4fv(id, 1, GL_FALSE, mv.data());
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

	mv = camera.view_matrix() * l2.to_global();
	glUniformMatrix4fv(id, 1, GL_FALSE, mv.data());
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

	mv = camera.view_matrix() * l3.to_global();
	glUniformMatrix4fv(id, 1, GL_FALSE, mv.data());
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

	mv = camera.view_matrix() * l4.to_global();
	glUniformMatrix4fv(id, 1, GL_FALSE, mv.data());
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

	mv = camera.view_matrix() * l5.to_global();
	glUniformMatrix4fv(id, 1, GL_FALSE, mv.data());
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
	glfwSetFramebufferSizeCallback(window, reshape);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

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
