/******************************************************************************/
/*!
\file	Sphere.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Class to define Sphere Shape for Collision
*/
/******************************************************************************/
#include "Shape.h"
#pragma once
/******************************************************************************/
/*!
Class Sphere:
\brief	Child of Shape, stores Variables for Sphere Collisions
*/
/******************************************************************************/
class Sphere : public Shape
{
public:
	Sphere(Vector3 origin, float radius) : Shape(origin){
		this->radius = radius;
		this->type = 0;
	}
	Sphere(){};
	~Sphere(){};

	//Updates the new Position of the Shape
	void moveTo(Vector3);

	//Accessors
	float getRadius(void)const;

	//Mutators
	void setRadius(float);
private:
	float radius;//length of Sphere starting from origin(centre) to the edge
};