#include "Shape.h"

/********************************
CONSTRUCTOR
*********************************/
Shape::Shape(Vector3 origin)
{
	this->origin = origin;
}


/********************************
ACCESSOR(S)
*********************************/
Vector3 Shape::getOrigin(void)const
{
	return this->origin;
}
int Shape::getType(void)const
{
	return this->type;
}


/********************************
MUTATOR(S)
*********************************/
void Shape::setOrigin(Vector3 origin)
{
	this->origin = origin;
}