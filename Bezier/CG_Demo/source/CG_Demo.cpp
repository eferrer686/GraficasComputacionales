/*
 * Sergio Ruiz.
 *
 * TC3022. Computer Graphics Course.
 * Basic template OpenGL project.
 */

// Please don't change lines 9-31 (It helps me to grade)
#ifdef __APPLE__
// For XCode only: New C++ terminal project. Build phases->Compile with libraries: add OpenGL and GLUT
// Import this whole code into a new C++ file (main.cpp, for example). Then run.
// Reference: https://en.wikibooks.org/wiki/OpenGL_Programming/Installation/Mac
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
#endif
#ifdef _WIN32
// For VS on Windows only: Download CG_Demo.zip. UNZIP FIRST. Double click on CG_Demo/CG_Demo.sln
// Run
	#include "freeglut.h"
#endif
#ifdef __unix__
// For Linux users only: g++ CG_Demo.cpp -lglut -lGL -o CG_Demo
// ./CG_Demo
// Reference: https://www.linuxjournal.com/content/introduction-opengl-programming
	#include "GL/freeglut.h"
	#include "GL/gl.h"
#endif


#define TEX_SIDE 256

#include <stdio.h>
#include <math.h>
#include "Bezier.h"
#include "Point.h"
#include "glm.h"

#include <iostream>

#include <string>
using namespace std;

void axes() {
	glLineWidth(5.0f);
	glBegin(GL_LINES);
	{
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);

	}
	glEnd();
}


#define CTRL_POINTS 5

unsigned int  texture_id = 0;

Bezier* bez;					//Declare a Bezier curve
Point* ctrl[CTRL_POINTS];		//
Point* traveler;
float param;
float dir;


GLfloat*	global_ambient;

char		bufferFPS[11];	// For on-screen text.

GLMmodel*	sintel;

float		sintel_pos[3];
float		sintel_dims[3];
float		radius;

void init() // FOR GLUT LOOP
{
	//Begin Plane
	radius = 0;
	sintel_pos[0] = 0.0f;
	sintel_pos[1] = 0.0f;
	sintel_pos[2] = 0.0f;

	// 
	sintel = glmReadOBJ("assets/plane.obj");
	
	glmUnitize(sintel);
	glmFacetNormals(sintel);
	glmDimensions(sintel, sintel_dims);
	float center[3] = { sintel_pos[0] + sintel_pos[0] / 2.0f,
						sintel_pos[1] + sintel_pos[1] / 2.0f,
						sintel_pos[2] + sintel_pos[2] / 2.0f };

	radius = sqrtf(center[0] * center[0] + center[1] * center[1] + center[2] * center[2]);
	//End Plane
	//Light properties
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat diffusel0[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat ambientl0[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularl0[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat position[4] = { 2.0f, 0.5f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientl0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusel0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularl0);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	global_ambient = new GLfloat[4];
	global_ambient[0] = 0.3f;
	global_ambient[1] = 0.3f;
	global_ambient[2] = 0.3f;
	global_ambient[3] = 1.0f;
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);


	//Begin Bezier
	dir = 1;
	param = 0.1;

	traveler = new Point(0, 0, 0, 0, 1, 0);
	

	for (int i = 0; i < CTRL_POINTS; i++)
	{
		ctrl[i] = new Point(0, 0, 0, 1, 0, 0);
	}

	ctrl[0]->x = -14.0f; ctrl[0]->z =5.0f;
	ctrl[1]->x = 2.0f; ctrl[1]->z = 6.0f;
	ctrl[2]->x = 4.0f; ctrl[2]->z = -11.0f;
	ctrl[3]->x = 1.0f; ctrl[3]->z = -6.0f;
	ctrl[4]->x = 14.0f; ctrl[4]->z = -3.0f;

	bez = new Bezier(CTRL_POINTS-1, ctrl);

	glEnable(GL_DEPTH_TEST);			// Enable check for close and far objects.
	glClearColor(0.0, 0.0, 0.0, 0.0);	// Clear the color state.
	glMatrixMode(GL_MODELVIEW);			// Go to 3D mode.
	glLoadIdentity();					// Reset 3D view matrix.

}
void setColor(float r, float g, float b) {
	
	// Generate my texture ID (just 1):
	glGenTextures(1, &texture_id);

	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	GLfloat texture_data[TEX_SIDE][TEX_SIDE][3];
	for (int u = 0; u < TEX_SIDE; u++)
	{
		for (int v = 0; v < TEX_SIDE; v++)
		{
			texture_data[u][v][0] = r;
			texture_data[u][v][1] = g;
			texture_data[u][v][2] = b;
		}
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		TEX_SIDE,
		TEX_SIDE,
		0,
		GL_RGB,
		GL_FLOAT,
		texture_data);

	glBindTexture(GL_TEXTURE_2D, NULL);
}

void display()							// Called for each frame (about 60 times per second).
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear color and depth buffers.
	glLoadIdentity();												// Reset 3D view matrix.
	gluLookAt(0.1, 40.0, 0.0,										// Where the camera is.
		      0.0, 0.0, 0.0,										// To where the camera points at.
		      0.0, 1.0, 0.0);										// "UP" vector.


	axes();
	
	//Draw Plane
	glPushMatrix();
	{
		glScalef(-15, 15, 15);
		glTranslatef(sintel_pos[0], sintel_pos[1], sintel_pos[2]);
		glmDraw(sintel, GLM_SMOOTH | GLM_TEXTURE);
	}
	glPopMatrix();
	
	setColor(1.0f, 0.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	//Draw ctrl points
	for (int i = 0; i < CTRL_POINTS; i++)
	{
		ctrl[i]->draw();
	}
	glBindTexture(GL_TEXTURE_2D, NULL);


	traveler = bez->evaluate(param);

	setColor(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	traveler->draw();
	glBindTexture(GL_TEXTURE_2D, NULL);

	delete traveler;


	
	if (param <= 0) { param = 0.001; dir *= -1; }
	if (param >= 1) { param = 0.999; dir *= -1; }
	
	
	param += 0.001f * dir;

	

	glutSwapBuffers();												// Swap the hidden and visible buffers.


}


void idle()															// Called when drawing is finished.
{
	glutPostRedisplay();											// Display again.
}

void reshape(int x, int y)											// Called when the window geometry changes.
{
	glMatrixMode(GL_PROJECTION);									// Go to 2D mode.
	glLoadIdentity();												// Reset the 2D matrix.
	gluPerspective(40.0, (GLdouble)x / (GLdouble)y, 0.5, 50.0);		// Configure the camera lens aperture.
	glMatrixMode(GL_MODELVIEW);										// Go to 3D mode.
	glViewport(0, 0, x, y);											// Configure the camera frame dimensions.
	gluLookAt(0.0, 1.0, 4.0,
		      0.0, 0.0, 0.0,
		      0.0, 1.0, 0.0);
	display();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);											// Init GLUT with command line parameters.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);		// Use 2 buffers (hidden and visible). Use the depth buffer. Use 3 color channels.
	glutInitWindowSize(800, 800);
	glutCreateWindow("RTS");
	
	init();


	glutReshapeFunc(reshape);										// Reshape CALLBACK function.
	glutDisplayFunc(display);										// Display CALLBACK function.
	glutIdleFunc(idle);												// Idle CALLBACK function.
	glutMainLoop();													// Begin graphics program.
	return 0;														// ANSI C requires a return value.
}
