#include "Vector3.h"
#pragma once

/******************************************************************************/
/*!
Class Shape:
\brief	Abstract Base Class of 2D Shapes used for 2D Collision
*/
/******************************************************************************/
class Shape
{
public:
	Shape(Vector3);
	Shape();
	~Shape();
	virtual void moveTo(Vector3) = 0;//Moves Shape to New Position

	//Accessors
	Vector3 getOrigin(void)const;
	int getType(void)const;

	//Mutators
	void setOrigin(Vector3);

protected:
	Vector3 origin; //Start of the Shape(Centre for Box/Sphere, One End for Rays...etc)
	     
	int type;//Type of Shape, helps Data Searching
	/*
	0 = Sphere
	1 = Box
	2 = Ray(Line)
	3 = Plane
	*/
	//type = ONLY SET IN CHILD CONSTRUCTORS(No Mutator)
};