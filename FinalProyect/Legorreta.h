#include "glm.h"

class Legorreta
{
public:


	GLfloat* Ia;
	GLfloat* Id;
	GLfloat* Is;
	GLfloat* Ip;

	GLMmodel* sintel;
	float* pos;
	float* dims;
	float* center;
	void draw();

	Legorreta();
	~Legorreta();

};