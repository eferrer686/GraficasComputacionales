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

#include <stdio.h>
#include <math.h>
#include <Camera.h>
#include "cParticle.h"

int mouseCoords[2], mouseMotionType = 0;

Camera* mainCam;
Camera* testCam;

#define MAX_PARTICLES 20
Particle* p;

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

void collisions() {
	for (int i = 0; i < MAX_PARTICLES; i++) {
		bool collide = false;

		for (int j = 0; j < MAX_PARTICLES; j++) {

			if (i != j & !collide) {
				if (p[i].inCollision(&p[j])) {
					collide = true;
					break;
				}
			}
		}

		if (collide == true) {
			p[i].diffuse[0] = 1;
			p[i].diffuse[1] = 0;
			p[i].diffuse[2] = 0;

		}
		else {
			p[i].collisionStart = true;
			p[i].diffuse[0] = p[i].oDiffuse[0];
			p[i].diffuse[1] = p[i].oDiffuse[1];
			p[i].diffuse[2] = p[i].oDiffuse[2];
		}

	}
}



void init() // FOR GLUT LOOP
{
	mainCam = new Camera();
	testCam = new Camera();

	

	testCam->pos.x = 10;
	testCam->pos.y = 0;
	testCam->pos.z = 0;
	testCam->dir.x = -1;
	testCam->dir.y = 0;
	testCam->dir.z = 0;

	//Particle Init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DITHER);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);



	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat diffusel0[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat ambientl0[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat specularl0[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientl0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusel0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularl0);

	GLfloat position[] = { 100.0, 200.0, -100.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glMatrixMode(GL_MODELVIEW);			// Go to 3D mode.
	glLoadIdentity();
	//plane = new cPlane(1000, 1000);

	p = new Particle[MAX_PARTICLES];

}

void display()							// Called for each frame (about 60 times per second).
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear color and depth buffers.
	
	mainCam->setView();
	//testCam->setView();
	// x,  y,   z
	float g[] = { 0,-9.81,0 };
	collisions();
	for (int i = 0; i < MAX_PARTICLES; i++) {
		if (p[i].alive) {
			p[i].checkCubeCollisions();
			//p[i].addForce(g);
			p[i].stepSimulation(0.0001);
			p[i].draw();
		}
	}

	glutWireCube(20);

	
	glutSwapBuffers();												// Swap the hidden and visible buffers.


}
void idle()															// Called when drawing is finished.
{
	glutPostRedisplay();											// Display again.
}

void motion(int x,int y) {
	switch (mouseMotionType)
	{
	case 1:	//FREE
		mainCam->rotate((mouseCoords[0] - x)*-0.1, vector3f(0, 1, 0));
		mainCam->rotate((mouseCoords[1] - y)*-0.1, vector3f(1, 0, 0));
		mouseCoords[0] = x;
		mouseCoords[1] = y;
		break;
	case 2:	//SHIFT
		mainCam->moveAround((mouseCoords[0] - x)*0.1, vector3f(0, 1, 0));
		mainCam->moveAround((mouseCoords[1] - y)*0.1, vector3f(1, 0, 0));
		break;
	case 3:	//ALT

		break;
	case 4:	//CTRL

		break;
	default:
		break;
	}
}
void mouse(int button, int state, int x, int y){
	int mods;
	mouseCoords[0] = x;
	mouseCoords[1] = y;
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {

		}
		else if (button == GLUT_RIGHT_BUTTON) {

		}
		else {

		}
		mods = glutGetModifiers();

		if (mods & GLUT_ACTIVE_SHIFT) {
			mouseMotionType = 2;
		}
		else if (mods & GLUT_ACTIVE_ALT) {
			mouseMotionType = 3;
		}
		else if (mods & GLUT_ACTIVE_CTRL) {
			mouseMotionType = 4;
		}
		else {
			mouseMotionType = 1;
		}
		
	}
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
	glutMotionFunc(motion);
	glutMouseFunc(mouse);

	glutReshapeFunc(reshape);										// Reshape CALLBACK function.
	glutDisplayFunc(display);										// Display CALLBACK function.
	glutIdleFunc(idle);												// Idle CALLBACK function.
	glutMainLoop();													// Begin graphics program.
	return 0;														// ANSI C requires a return value.
}
