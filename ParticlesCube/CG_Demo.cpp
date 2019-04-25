//Eduardo Ferrer A01651867
//Rafael García Cadenas A01334363
//Luis Alberto Torres Romero A01335919
//Abraham armando Silva Apanco A01651490
//Enrique Landeros Ontiveros A01336571

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
// For Linux users only: g++ CG_Demo.cpp -lglut -lGL -lGLU -o CG_Demo
// ./CG_Demo
// Reference: https://www.linuxjournal.com/content/introduction-opengl-programming
	#include "GL/freeglut.h"
	#include "GL/gl.h"
#endif


#include "cParticle.h"


long frames, time_, timebase = 0;
unsigned long frameCount;
float fps = 0.0;
char bufferFPS[11];
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


void displayText(int x, int y, char *txt);
//cPlane *plane;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear color and depth buffers.
	glLoadIdentity();												// Reset 3D view matrix.
	gluLookAt(0.0, 0.0, 40.0,										// Where the camera is.
		0.0, 0.0, 0.0,										// To where the camera points at.
		0.0, 1.0, 0.0);
	

	float colorMat[] = { 128 / 255.0, 128 / 255.0 , 128 / 255.0 };
	//plane->display(4, colorMat);

	// x,  y,   z
	float g[] = { 0,-9.81,0 };
	collisions();
	for (int i = 0; i<MAX_PARTICLES; i++) {
		if (p[i].alive) {



			p[i].checkCubeCollisions();
			//p[i].addForce(g);
			p[i].stepSimulation(0.01);
			
			p[i].draw();
		}
	}
	
		glutWireCube(20);
	
	
	displayText(5, 20, bufferFPS);

	glutSwapBuffers();
}

void idle(void)
{
	glutPostRedisplay();
}

void init(void)
{
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



void keys(unsigned char key, int x, int y)
{
}

void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (double)w / h, 0.01f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluLookAt(0.0, 1.0, 4.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
	display();
}

int main(int argc, char** argv)
{
	//srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hello");
	glutReshapeFunc(reshape);
	init();
	glutReshapeFunc(reshape);										// Reshape CALLBACK function.
	glutDisplayFunc(display);										// Display CALLBACK function.
	glutIdleFunc(idle);												// Idle CALLBACK function.
	glutMainLoop();
	return 0;   /* ANSI C requires main to return int. */
}

void displayText(int x, int y, char *txt) {
	GLboolean lighting;
	GLint viewportCoords[4];
	glColor3f(1.0, 0.0, 0.0);
	glGetBooleanv(GL_LIGHTING, &lighting);
	glGetIntegerv(GL_VIEWPORT, viewportCoords);
	if (lighting) glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, viewportCoords[2], 0.0, viewportCoords[3]);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glRasterPos2i(x, viewportCoords[3] - y);


	while (*txt) { glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *txt); txt++; }
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	if (lighting) glEnable(GL_LIGHTING);
}
