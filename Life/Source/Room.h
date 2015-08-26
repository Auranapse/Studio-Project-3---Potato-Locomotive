/******************************************************************************/
/*!
\file	Room.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Class to define Rooms in Scene
*/
/******************************************************************************/
#include "NonLiving.h"
#pragma once
/******************************************************************************/
/*!
		Class Room:
\brief	Defines a Room in the Scene
*/
/******************************************************************************/
class Room : public NonLiving
{
public:
	Room(Mesh *mesh, Shape *bound, float mass,
			bool visible, bool reverseCollision,
			float staticFriction, float kineticFriction)
			: NonLiving(mesh, bound, mass, visible, reverseCollision)
	{
		this->type = 3;
		this->staticFriction = staticFriction;
		this->kineticFriction = kineticFriction;
	}

	Room();
	~Room();

	//Accessors
	float getStatic(void)const;
	float getKinetic(void)const;

	//Mutators
	void setStatic(float);
	void setKinetic(float);
private:
	float staticFriction;
	float kineticFriction;
};