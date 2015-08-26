/******************************************************************************/
/*!
\file	Plane.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Class to define Plane Shape for Collision
*/
/******************************************************************************/
#include "Shape.h"
#pragma once
/******************************************************************************/
/*!
Class Plane:
\brief	 Child of Shape, stores Variables for Plane Collisions
*/
/******************************************************************************/
class Plane : public Shape
{
public:
	Plane(Vector3 origin, Vector3 pointOnPlane) : Shape(origin)
	{
		type = 3;
		this->pointOnPlane = pointOnPlane;
	}
	Plane(){};
	~Plane(){};
	//Updates new Position of Shape
	void moveTo(Vector3);

	//Accessor
	Vector3 getPoint(void)const;

	//Mutator
	void setPoint(Vector3);
private:
	Vector3 pointOnPlane;//End of Ray
};