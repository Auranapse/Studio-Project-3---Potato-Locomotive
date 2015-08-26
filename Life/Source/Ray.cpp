#include "Ray.h"

/******************************************************************************/
/*!
\brief	
	Gets the End of Ray
\return
	Ray end
*/
/******************************************************************************/
Vector3 Ray::getEnd(void)const
{
	return this->end;
}

/******************************************************************************/
/*!
\brief	
	Sets End of ray
\param end
	Ray end
*/
/******************************************************************************/
void Ray::setEnd(Vector3 end)
{
	this->end = end;
}

/******************************************************************************/
/*!
\brief	
	Updates new Position of Ray
\param newPosition
	New Position of Ray
*/
/******************************************************************************/
void Ray::moveTo(Vector3 newPosition)
{
	Vector3 change = newPosition - this->origin;
	this->origin = newPosition;
	this->end += change;
}