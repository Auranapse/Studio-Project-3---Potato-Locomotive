#include "Sphere.h"

/********************************
ACCESSOR(S)
*********************************/
float Sphere::getRadius(void)const
{
	return this->radius;
}


/********************************
MUTATOR(S)
*********************************/
void Sphere::setRadius(float radius)
{
	this->radius = radius;
}


void Sphere::moveTo(Vector3 newPosition)
{
	this->origin = newPosition;
}