#include "Ray.h"

/********************************
ACCESSOR(S)
*********************************/
Vector3 Ray::getEnd(void)const
{
	return this->end;
}

/********************************
MUTATOR(S)
*********************************/
void Ray::setEnd(Vector3 end)
{
	this->end = end;
}

void Ray::moveTo(Vector3 newPosition)
{
	Vector3 change = newPosition - this->origin;
	this->origin = newPosition;
	this->end += change;
}