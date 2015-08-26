#include "AABB.h"

/******************************************************************************/
/*!
\brief	
	Gets the Maximum Point of AABB
\return
	AABB Maximum
*/
/******************************************************************************/
Vector3 AABB::getMax(void)const
{
	return this->max;
}
/******************************************************************************/
/*!
\brief	
	Gets the Minimum Point of AABB
\return
	AABB Minimum
*/
/******************************************************************************/
Vector3 AABB::getMin(void)const
{
	return this->min;
}
/******************************************************************************/
/*!
\brief	
	Gets the Length of AABB
\return
	AABB Length
*/
/******************************************************************************/
float AABB::getLength(void)const
{
	return this->length;
}
/******************************************************************************/
/*!
\brief	
	Gets the Width of AABB
\return
	AABB Width
*/
/******************************************************************************/
float AABB::getWidth(void)const
{
	return this->width;
}
/******************************************************************************/
/*!
\brief	
	Gets the Height of AABB
\return
	AABB Height
*/
/******************************************************************************/
float AABB::getHeight(void)const
{
	return this->height;
}

/******************************************************************************/
/*!
\brief	
	Sets the length of AABB
\param length
	AABB length
*/
/******************************************************************************/
void AABB::setLength(float length)
{
	this->length = length;
	this->max.z = this->origin.z + this->length/2;
	this->min.z = this->origin.z - this->length/2;
	this->origin.z = min.z + length/2;
}
/******************************************************************************/
/*!
\brief	
	Sets the width of AABB
\param width
	AABB width
*/
/******************************************************************************/
void AABB::setWidth(float width)
{
	this->width = width;
	this->max.x = this->origin.x + this->width/2;
	this->min.x = this->origin.x - this->width/2;
	this->origin.x = min.x + width/2;
}
/******************************************************************************/
/*!
\brief	
	Sets the height of AABB
\param height
	AABB height
*/
/******************************************************************************/
void AABB::setHeight(float height)
{
	this->height = height;

	this->max.y = this->origin.y + this->height/2;
	this->min.y = this->origin.y - this->height/2;
	this->origin.y = min.y + height/2;
}
/******************************************************************************/
/*!
\brief	
	Sets the Maximum Point of AABB
\param max
	AABB Maximum
*/
/******************************************************************************/
void AABB::setMax(Vector3 max)
{
	this->max = max;
	this->length = max.z - min.z;
	this->width = max.x - min.x;
	this->height = max.y - min.y;
	this->origin.Set(min.x + width/2,
					 min.y + height/2,
					 min.z + length/2);
}
/******************************************************************************/
/*!
\brief	
	Sets the Minimum Point of AABB
\param min
	AABB minimum
*/
/******************************************************************************/
void AABB::setMin(Vector3 min)
{
	this->min = min;
	this->length = max.z - min.z;
	this->width = max.x - min.x;
	this->height = max.y - min.y;
	this->origin.Set(min.x + width/2,
					 min.y + height/2,
					 min.z + length/2);
}

/******************************************************************************/
/*!
\brief	
	Updates new position of AABB
\param newPosition
	New Position of AABB
*/
/******************************************************************************/
void AABB::moveTo(Vector3 newPosition)
{
	Vector3 change = newPosition - this->origin;
	setMax(this->origin + change);
	setMin(this->origin - change);
	this->origin = newPosition;
}