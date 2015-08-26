#include "Sphere.h"

/******************************************************************************/
/*!
\brief	
	Gets the Radius of Sphere
\return
	Sphere Radius
*/
/******************************************************************************/
float Sphere::getRadius(void)const
{
	return this->radius;
}


/******************************************************************************/
/*!
\brief	
	Sets radius of Sphere
\param	radius
	Radius to be Set
*/
/******************************************************************************/
void Sphere::setRadius(float radius)
{
	this->radius = radius;
}

/******************************************************************************/
/*!
\brief	
	Updates new position of Sphere
\param	newPosition
	New Position of Sphere
*/
/******************************************************************************/
void Sphere::moveTo(Vector3 newPosition)
{
	this->origin = newPosition;
}