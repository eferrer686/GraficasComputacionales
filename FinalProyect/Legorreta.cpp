#include "Legorreta.h"

Legorreta::Legorreta()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	Id = new float[4];
	Id[0] = 0.6f;
	Id[1] = 0.6f;
	Id[2] = 0.6f;
	Id[3] = 0.6f;

	Ia = new float[4];
	Ia[0] = 0.5f;
	Ia[1] = 0.5f;
	Ia[2] = 0.5f;
	Ia[3] = 0.5f;

	Is = new float[4];
	Is[0] = 0.5f;
	Is[1] = 0.5f;
	Is[2] = 0.5f;
	Is[3] = 0.5f;

	Ip = new float[4];
	Ip[0] = 0.5f;
	Ip[1] = 0.5f;
	Ip[2] = 0.5f;
	Ip[3] = 0.5f;

	glLightfv(GL_LIGHT0, GL_AMBIENT, Ia);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Id);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Is);
	glLightfv(GL_LIGHT0, GL_POSITION, Ip);

	pos = new float[3];
	dims = new float[3];

	pos[0] = 0.0f;
	pos[1] = 0.0f;
	pos[2] = 0.0f;

	sintel = glmReadOBJ("assets/modelo/Tec.obj");
	//sintel = glmReadOBJ("assets/lego.obj");
	glmUnitize(sintel);
	glmFacetNormals(sintel);
	glmDimensions(sintel, dims);
	center = new float[3];
	center[0] = pos[0] + dims[0] / 2.0f;
	center[1] = pos[1] + dims[1] / 2.0f;
	center[2] = pos[2] + dims[2] / 2.0f;
}


Legorreta::~Legorreta() {}

void Legorreta::draw() {
	glPushMatrix();
	{
		glScalef(100, 100, 100);
		glTranslatef(pos[0], pos[1]+0.05, pos[2]);
		glRotatef(180, 1, 0, 0);
		glmDraw(sintel, GLM_SMOOTH | GLM_TEXTURE);
	}
	glPopMatrix();
}