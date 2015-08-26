#include "Shape.h"

/******************************************************************************/
/*!
\brief	
constructor, sets origin of shape
\param origin
	origin(Centre for Sphere, one end for Ray...etc)
*/
/******************************************************************************/
Shape::Shape(Vector3 origin)
{
	this->origin = origin;
}

/******************************************************************************/
/*!
\brief	
Gets origin of shape
\return
	origin of shape
*/
/******************************************************************************/
Vector3 Shape::getOrigin(void)const
{
	return this->origin;
}
/******************************************************************************/
/*!
\brief	
Gets the Kind of Shape it is
\return
	type of Shape
*/
/******************************************************************************/
int Shape::getType(void)const
{
	return this->type;
}


/******************************************************************************/
/*!
\brief	
Sets origin of shape
\param origin
	origin of shape
*/
/******************************************************************************/
void Shape::setOrigin(Vector3 origin)
{
	this->origin = origin;
}