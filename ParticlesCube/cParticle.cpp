#include "cParticle.h"

#define IMAGINARY_CUBE 20

Particle::Particle()
{
	restitutionCoefficient = 0.2f;
	radius = 0.2f + 0.1*((float)rand()) / (float)RAND_MAX;
	mass = radius;

	position[0] = -1.0f + 2 * ((float)rand()) / (float)RAND_MAX;
	position[1] = -1.0f + 2 * ((float)rand()) / (float)RAND_MAX;
	position[2] = -1.0f + 2 * ((float)rand()) / (float)RAND_MAX;

	oldPosition[0] = 0.0;
	oldPosition[1] = 0.0;
	oldPosition[2] = 0.0;

	forces[0] = 0;
	forces[1] = 9.81;
	forces[2] = 0;



	dragForce[0] = forces[0];
	dragForce[1] = forces[1];
	dragForce[2] = forces[2];

	collisionStart = false;
	first = 0;

	if (vecMagnitude(dragForce) > 1.0f)
	{
		unitVector(dragForce);
	}
	alive = true;
	age = 0;

	diffuse[0] = 0.8f * ((float)rand()) / (float)RAND_MAX; 
	diffuse[1] = ((float) rand())/(float) RAND_MAX;
	diffuse[2] = ((float) rand())/(float) RAND_MAX;

	oDiffuse[0] = diffuse[0];
	oDiffuse[1] = diffuse[1];
	oDiffuse[2] = diffuse[2];

	specular[0] = 0.8;
	specular[1] = 0.8;
	specular[2] = 0.8;
	shininess = 64;
}

Particle::~Particle()
{
}

void Particle::draw()
{
	
	glPushMatrix();
	{
		glEnable(GL_COLOR_MATERIAL);
		{
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
			glMaterialf(GL_FRONT, GL_SHININESS, shininess);
			glEnable(GL_COLOR_MATERIAL);
			glColor3fv(diffuse);
			glTranslatef(position[0], position[1], position[2]);
			glutSolidSphere(radius, 20, 20);
		}
		glDisable(GL_COLOR_MATERIAL);
	}
	glPopMatrix();
}

void Particle::stepSimulation(float dt)
{
	integrateVerlet(dt);
}

void Particle::clearForces()
{
	forces[0] = 0;
	forces[1] = 0;
	forces[2] = 0;

	dragForce[0] = 0;
	dragForce[1] = 0;
	dragForce[2] = 0;
}

void Particle::addForce(float *force)
{
	forces[0] += force[0];
	forces[1] += force[1];
	forces[2] += force[2];

	dragForce[0] = forces[0];
	dragForce[1] = forces[1];
	dragForce[2] = forces[2];


	if (vecMagnitude(dragForce) > 1.0f)
	{
		unitVector(dragForce);
	}

}

void Particle::integrateVerlet(float dt)
{
	// ToDo: complete here.

	forces[0] -= dragForce[0];
	forces[1] -= dragForce[1];
	forces[2] -= dragForce[2];

	float accel[3];
	accel[0] = forces[0] / mass;
	accel[1] = forces[1] / mass;
	accel[2] = forces[2] / mass;

	float positionAux[3];

	positionAux[0] = position[0];
	positionAux[1] = position[1];
	positionAux[2] = position[2];

	position[0] = 2.0f * position[0] - oldPosition[0] + accel[0] * dt * dt;
	position[1] = 2.0f * position[1] - oldPosition[1] + accel[1] * dt * dt;
	position[2] = 2.0f * position[2] - oldPosition[2] + accel[2] * dt * dt;

	oldPosition[0] = positionAux[0];
	oldPosition[1] = positionAux[1];
	oldPosition[2] = positionAux[2];

	/*cout << "x:";
	cout << position[0];
	
	cout << "|y:";
	cout << position[1];
	
	cout << "|z:";
	cout << position[2];
	cout << "\n";*/
}

void Particle::incrementAge(float time)
{
	age+=time;
}

bool Particle::inCollision(Particle* other)
{
	float sumR = radius + other->radius;

	float dx = (position[0]-other->position[0]) * (position[0]-other->position[0]);

	float dy = (position[1]-other->position[1]) * (position[1]-other->position[1]);
	
	float dz = (position[2]-other->position[2]) * (position[2]-other->position[2]);

	float dist = dx+dy+dz;
	float sumR2 = sumR*sumR;

	//Reverse direction if not beginning
	

	if (dist < sumR2) {
		if (collisionStart) {

	
		float dD[3];
		dD[0] = position[0] - oldPosition[0];
		dD[1] = position[1] - oldPosition[1];
		dD[2] = position[2] - oldPosition[2];


			float positionAux;
			positionAux = position[0];
			position[0] = oldPosition[0] + dD[0] * restitutionCoefficient;
			oldPosition[0] = positionAux;

			positionAux = position[1];
			position[1] = oldPosition[1] + dD[1] * restitutionCoefficient;
			oldPosition[1] = positionAux;

			positionAux = position[2];
			position[2] = oldPosition[2] + dD[2] * restitutionCoefficient;
			oldPosition[2] = positionAux;

		}
		return true;
	}
	else {
		return false;
	}
}


void Particle::checkCubeCollisions()
{
	//X axis
	if (position[0] + radius >= 10 )
	{
		float distCube = position[0] - (-radius + 10);
		position[0] -= distCube;
		oldPosition[0] -= distCube;

		float dD = 0.0;
		dD = position[0] - oldPosition[0];

		
		//forces[0] = restitutionCoefficient * forces[0];

		float positionAux;
		positionAux = position[0];
		position[0] = oldPosition[0]+dD*restitutionCoefficient;
		oldPosition[0] = positionAux;


	}
	else if (position[0] - radius <= -10) {
		float distCube = position[0] - (radius - 10);
		position[0] -= distCube;
		oldPosition[0] -= distCube;
	
		float dD = 0.0;
		dD = position[0] - oldPosition[0];

		//forces[0] = restitutionCoefficient * forces[0];

		float positionAux;
		positionAux = position[0];
		position[0] = oldPosition[0]+dD*restitutionCoefficient;
		oldPosition[0] = positionAux;

		
	}
	//Y axis
	if (position[1] - radius <= -10)
	{
		float distCube = position[1]-(radius-10);
		position[1] -= distCube;
		oldPosition[1] -= distCube;

		float dD = 0.0;
		dD = position[1] - oldPosition[1];

		//forces[1] = restitutionCoefficient * forces[1];

		float positionAux;
		positionAux = position[1];
		position[1] = oldPosition[1]+dD*restitutionCoefficient;
		oldPosition[1] = positionAux;


		
	}
	else if (position[1] + radius >= 10) {
		float distCube = position[1] - (-radius + 10);
		position[1] -= distCube;
		oldPosition[1] -= distCube;

		float dD = 0.0;
		dD = position[1] - oldPosition[1];

		//forces[1] = restitutionCoefficient * forces[1];

		float positionAux;
		positionAux = position[1];
		position[1] = oldPosition[1]+dD*restitutionCoefficient;
		oldPosition[1] = positionAux;

		
	}
	//Z axis
	if (position[2] - radius <= -10)
	{
		float distCube = position[2] - (radius - 10);
		position[2] -= distCube;
		oldPosition[2] -= distCube;

		float dD = 0.0;
		dD = position[2] - oldPosition[2];

		//forces[2] = restitutionCoefficient * forces[2];

		float positionAux;
		positionAux = position[2];
		position[2] = oldPosition[2]+dD*restitutionCoefficient;
		oldPosition[2] = positionAux;

	}

	else if (position[2] + radius >= 10) {
		float distCube = position[2] - (-radius + 10);
		position[2] -= distCube;
		oldPosition[2] -= distCube;

		float dD = 0.0;
		dD = position[2] - position[2];
		
		//forces[2] = restitutionCoefficient * forces[2];

		float positionAux;
		positionAux = position[2];
		position[2] = oldPosition[2]+dD*restitutionCoefficient;
		oldPosition[2] = positionAux;

		
	}

}
