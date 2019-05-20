#pragma once
#include <stdio.h>
#include <math.h>
#include "glm.h"

class Legorreta
{
public:
	float		rotation;
	GLfloat*	global_ambient;


	GLMmodel*	sintel;

	float		sintel_pos[3];
	float		sintel_dims[3];
	float		radius;
	float		mode;

	Legorreta();

	~Legorreta();

	void draw();

};

