/******************************************************************************/
/*!
\file	Wall.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Class to define Walls in Scene
*/
/******************************************************************************/
#include "NonLiving.h"
#pragma once
/******************************************************************************/
/*!
		Class Wall:
\brief	Defines a Wall in the Scene
*/
/******************************************************************************/
class Wall : public NonLiving
{
public:
	Wall(Mesh *mesh, Shape *bound, float mass,
			bool visible, bool reverseCollision,
			float height)
			: NonLiving(mesh, bound, mass, visible, reverseCollision)
	{
		this->type = 6;
		this->height = height;
	}

	Wall();
	~Wall();

	//Accessors
	float getHeight(void)const;

	//Mutators
	void setHeight(float);
private:
	float height;
};