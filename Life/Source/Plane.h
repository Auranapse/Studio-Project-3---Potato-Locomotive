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
	void moveTo(Vector3);

	//Accessor
	Vector3 getPoint(void)const;

	//Mutator
	void setPoint(Vector3);
private:
	Vector3 pointOnPlane;//End of Ray
};