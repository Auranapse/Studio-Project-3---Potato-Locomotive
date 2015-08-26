/******************************************************************************/
/*!
\file	Ray.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Class to define Ray Shape for Collision
*/
/******************************************************************************/
#include "Shape.h"
#pragma once
/******************************************************************************/
/*!
Class Ray:
\brief	 Child of Shape, stores Variables for Ray Collisions
*/
/******************************************************************************/
class Ray : public Shape
{
public:
	Ray(Vector3 origin, Vector3 end) : Shape(origin)
	{
		type = 2;
		this->end = end;
	}
	Ray(){};
	~Ray(){};
	void moveTo(Vector3);

	//Accessor
	Vector3 getEnd(void)const;

	//Mutator
	void setEnd(Vector3);
private:
	Vector3 end;//End of Ray
};