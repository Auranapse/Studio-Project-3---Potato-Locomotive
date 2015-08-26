#include "Plane.h"

/******************************************************************************/
/*!
\brief	
	Gets a point on plane
\return
	Point On Plane
*/
/******************************************************************************/
Vector3 Plane::getPoint(void)const
{
	return this->pointOnPlane;
}

/******************************************************************************/
/*!
\brief	
	Sets Point on Plane
\param pointOnPlane
	Point On Plane
*/
/******************************************************************************/
void Plane::setPoint(Vector3 pointOnPlane)
{
	this->pointOnPlane = pointOnPlane;
}

/******************************************************************************/
/*!
\brief	
	Updates position of Plane
\param newPosition
	Plane Position
*/
/******************************************************************************/
void Plane::moveTo(Vector3 newPosition)
{
	Vector3 change = newPosition - this->origin;
	this->origin = newPosition;
	this->pointOnPlane += change;
}