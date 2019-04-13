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

engine::system l;

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
	auto p = projection(60, r, 1, 100);
	glLoadMatrixf(p.data());
}

void keyboard(unsigned char c, int x, int y)
{
	switch(c)
	{
		case '1': glEnable(GL_MULTISAMPLE_ARB); break;
		case '2': glDisable(GL_MULTISAMPLE_ARB); break;
		case 'q': l.rotate({ 10, UNIT_Y}); break;
		case 'e': l.rotate({-10, UNIT_Y}); break;
		case 'a': l.translate(-UNIT_X); break;
		case 'd': l.translate( UNIT_X); break;
		case 'w': l.translate(-UNIT_Z); break;
		case 's': l.translate( UNIT_Z); break;
	}
}

void mouse(int button, int state, int x, int y)
{
}

void motion(int x, int y)
{
	static int last_x = 640 / 2;
	static int last_y = 480 / 2;

	real dx = x - last_x;
	real dy = y - last_y;

	l.translate({dx / (real)100.0f, 0.0, 0.0});
	l.translate({0.0, 0.0, dy / 100.0f});

	last_x = x;
	last_y = y;
}

void draw()
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	auto lm = look_at({0.0, 2.0, 5.0}, {0.0, 0.0, 0.0}, UNIT_Y);
	auto mv = lm * l.to_global();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(mv.data());

	glColor3f(0.0, 0.0, 0.0);
	//glutWireCube(1);
	glutWireTeapot(1.0);

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(/*GLUT_3_2_CORE_PROFILE |*/ GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);
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
