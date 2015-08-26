/******************************************************************************/
/*!
\file	Shape.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Abstract Base Class to define Collision Shapes
*/
/******************************************************************************/
#include "Vector3.h"
#pragma once

/******************************************************************************/
/*!
Class Shape:
\brief	Abstract Base Class of 3D Shapes used for 3D Collision
*/
/******************************************************************************/
class Shape
{
public:
	Shape(Vector3);
	Shape();
	~Shape();
	//Updates new Position of Shape
	virtual void moveTo(Vector3) = 0;

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