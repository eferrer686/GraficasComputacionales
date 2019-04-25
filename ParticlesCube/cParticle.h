// For Mac users:   #include <random>

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

class Particle
{

public:
	Particle();
	~Particle();

	void draw();
	void stepSimulation(float dt);
	void addForce(float *force);
	bool inCollision(Particle* other);
	bool alive;
	bool collisionStart;
	int first;
	float position[3];
	float oDiffuse[3];
	float diffuse[3];

	float getAge() {return age; }
	void setAge(float time) {age = time;}
	void incrementAge(float time);
	void checkFloorCollisions();
	void checkCubeCollisions();

private:	
	void clearForces();
	void integrateVerlet(float dt);

	float mass;
	
	float oldPosition[3];
	float forces[3];
	float dragForce[3];
	
	float specular[3];
	float shininess;

	float radius;
	float age;
	float restitutionCoefficient;
	
};
