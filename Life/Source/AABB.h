/******************************************************************************/
/*!
\file	AABB.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Class to define AABB Shape for Collision
*/
/******************************************************************************/
#include "Shape.h"
#pragma once
/******************************************************************************/
/*!
Class AABB:
\brief	Child of Shape, stores Variables for Box Collisions
*/
/******************************************************************************/
class AABB : public Shape
{
public:
	AABB(Vector3 origin, float length, float width, float height) : Shape(origin) //Using Width/Height to create the Box(Same result, just preference)
	{
		this->type = 1;
		this->length = length;
		this->width = width;
		this->height = height;
		this->max = Vector3(this->origin.x + this->width/2,
							this->origin.y + this->height/2,
							this->origin.z + this->length/2);
		this->min = Vector3(this->origin.x - this->width/2,
							this->origin.y - this->height/2,
							this->origin.z + this->length/2);
	}
	AABB(Vector3 max, Vector3 min)//Using Max/Min to create the Box(Same result, just preference)
	{
		this->type = 1;
		this->max = max;
		this->min = min;
		this->length = max.z - min.z;
		this->width = max.x - min.x;
		this->height = max.y - min.y;
		this->origin.Set(width / 2, height / 2);
	}
	AABB(){};
	~AABB(){};
	//Updates new Position of Shape
	void moveTo(Vector3);

	//Accessors
	Vector3 getMax(void)const;
	Vector3 getMin(void)const;
	float getLength(void)const;
	float getWidth(void)const;
	float getHeight(void)const;
	

	//Mutators
	void setLength(float);
	void setWidth(float);
	void setHeight(float);
	void setMax(Vector3);
	void setMin(Vector3);
private:
	//Just a matter of preference, allows to set with these values(Not used for Collisions, but used as QuadTree Bound)
	float length;
	float width;
	float height;

	//Maximum and Minimum Bound of our Box (Used for Collisions)
	Vector3 max;
	Vector3 min;
};