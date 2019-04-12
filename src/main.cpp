#include <iostream>
#include <GL/glew.h>
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
#include "transforms.hpp"

using namespace std;
using namespace engine;

void init()
{
	cout << glGetString(GL_VENDOR) << endl;
	cout << glGetString(GL_RENDERER) << endl;
	cout << glGetString(GL_VERSION) << endl;
	cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	glewExperimental=true;
	glewInit();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	auto r = (real)w / (real)h;
	auto p = projection(60, r, 1, 10);
	glLoadMatrixf(p.data());
}

void keyboard(unsigned char c, int x, int y)
{
	switch(c)
	{
		case 'a': break;
		case 'd': break;
		case 'w': break;
		case 's': break;
	}
}

void mouse(int button, int state, int x, int y)
{
}

void motion(int x, int y)
{
	static int last_x = 0;
	static int last_y = 0;

	int dx = x - last_x;
	int dy = y - last_y;

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

	/*auto q1 = quaternion(45, UNIT_Y);
	auto q2 = quaternion(90, UNIT_X);
	static real a = 0.0;
	a+=0.01;
	real i = (sin(a) + 1.0) / 2.0;
	auto q = ((1.0 - i) * q1) + (i * q2);
	auto t = translate(UNIT_Z * -3);
	auto mv = t * rotate(q);
	glLoadMatrixf(mv.data());*/

	static real a = 0.0;
	engine::system l;
	//l.translate(UNIT_Y / 2);
	l.rotate({a, UNIT_X});
	l.rotate({a * 1.5f, UNIT_Y});
	l.rotate({a * 2.0f, UNIT_Z});
	//l.translate(-3 * UNIT_Z);
	++a;

	engine::system g;
	g.rotate(90, UNIT_Y);
	g.translate({3.0, 0.0, 0.0});

	auto mv = g.to_local() * l.to_global();
	glLoadMatrixf(mv.data());

	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glutWireTeapot(1.0);

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(/*GLUT_3_2_CORE_PROFILE |*/ GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Hello 3D Engine");
	init();
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	//glutPassiveMotionFunc(motion);
	glutIdleFunc(draw);
	glutDisplayFunc(draw);
	glutMainLoop();
}
