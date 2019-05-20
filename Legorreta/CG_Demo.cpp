/*
	Computer Graphics. TC3022.

	Eduardo Ferrer Mac Gregor Ruiz A01651867

	Basic TGA textures.
	Displays a textured OBJ.
*/

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include "freeglut.h"
#endif

#include <stdio.h>
#include <math.h>
#include "Legorreta.h"
Legorreta* lego;

/*
	Display mode:
	0: Vertices.
	1: Flat.
	2: Smooth.
	3: Textured.
*/
int			mode;

// Display text:
void displayText(int x, int y, char* txt);

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 1, 2, 0, 0, 0, 0, 1, 0);

	lego->draw();


	glutSwapBuffers();
}

void idle(void)
{
	
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, w / h * 1.0, 0.01, 1024.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void)
{
	
	lego = new Legorreta();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '0':
		mode = 0;
		break;
	case '1':
		mode = 1;
		break;
	case '2':
		mode = 2;
		break;
	case '3':
		mode = 3;
		break;
	case 27:
		//glmDelete( sintel );
		//exit(0);
		break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 10);
	glutCreateWindow("SINTEL v.1.0.");
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	init();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

/*
Displays characters stored in the "txt" array at position (x,y).
*/
void displayText(int x, int y, char* txt)
{
	GLboolean lighting;
	GLint viewportCoords[4];
	glColor3f(0.0, 1.0, 0.0);
	glGetBooleanv(GL_LIGHTING, &lighting);
	glGetIntegerv(GL_VIEWPORT, viewportCoords);
	if (lighting)
	{
		glDisable(GL_LIGHTING);
	}
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, viewportCoords[2], 0.0, viewportCoords[3]);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x, viewportCoords[3] - y);
	while (*txt)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *txt);
		txt++;
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	if (lighting)
	{
		glEnable(GL_LIGHTING);
	}
}
