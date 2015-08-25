#include "Plane.h"

/********************************
ACCESSOR(S)
*********************************/
Vector3 Plane::getPoint(void)const
{
	return this->pointOnPlane;
}

/********************************
MUTATOR(S)
*********************************/
void Plane::setPoint(Vector3 pointOnPlane)
{
	this->pointOnPlane = pointOnPlane;
}


void Plane::moveTo(Vector3 newPosition)
{
	Vector3 change = newPosition - this->origin;
	this->origin = newPosition;
	this->pointOnPlane += change;
}