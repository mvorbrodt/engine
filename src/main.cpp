#include <iostream>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "types.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "operators.hpp"

using namespace std;

void init()
{
	vec4 v1{1,0,0}, v2{0,1,0};
	vec4 v = v1 ^ v2;
	cout << v << endl << endl;

	mat4x4 m1
	{
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10, 11, 12,
		13, 14, 15, 16
	};
	mat4x4 m2
	{
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10, 11, 12,
		13, 14, 15, 16
	};
	mat4x4 m = m1 * m2;
	cout << m << endl << endl;

	vec4 v3{4,3,2,1};
	v = m * v3;
	cout << v << endl << endl;

	cout << glGetString(GL_VENDOR) << endl;
	cout << glGetString(GL_RENDERER) << endl;
	cout << glGetString(GL_VERSION) << endl;
	cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	glewExperimental=true;
	glewInit();
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0, 640.0 / 480.0, 1.0, 100.0);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
}

void draw()
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -3.0);
	glRotatef(30, 1.0, 0.0, 0.0);

	glutSwapBuffers();
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Hello OpenGL");
	init();
	glutIdleFunc(draw);
	glutReshapeFunc(reshape);
	glutDisplayFunc(draw);
	glutMainLoop();
}
