#include "AABB.h"

/********************************
ACCESSOR(S)
*********************************/
Vector3 AABB::getMax(void)const
{
	return this->max;
}
Vector3 AABB::getMin(void)const
{
	return this->min;
}
float AABB::getLength(void)const
{
	return this->length;
}
float AABB::getWidth(void)const
{
	return this->width;
}
float AABB::getHeight(void)const
{
	return this->height;
}

/********************************
MUTATOR(S)
*********************************/
void AABB::setLength(float length)
{
	this->length = length;
	this->max.z = this->origin.z + this->length/2;
	this->min.z = this->origin.z - this->length/2;
	this->origin.z = min.z + length/2;
}
void AABB::setWidth(float width)
{
	this->width = width;
	this->max.x = this->origin.x + this->width/2;
	this->min.x = this->origin.x - this->width/2;
	this->origin.x = min.x + width/2;
}
void AABB::setHeight(float height)
{
	this->height = height;

	this->max.y = this->origin.y + this->height/2;
	this->min.y = this->origin.y - this->height/2;
	this->origin.y = min.y + height/2;
}
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


void AABB::moveTo(Vector3 newPosition)
{
	Vector3 change = newPosition - this->origin;
	setMax(this->origin + change);
	setMin(this->origin - change);
	this->origin = newPosition;
}