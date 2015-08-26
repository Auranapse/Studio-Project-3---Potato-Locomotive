/******************************************************************************/
/*!
\file	SoundRange.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Class to define Sound Ranges in Scene
*/
/******************************************************************************/
#include "NonLiving.h"
#pragma once
/******************************************************************************/
/*!
		Class SoundRange:
\brief	Defines a Sound Range to check for possible Sound Detection
*/
/******************************************************************************/
class SoundRange : public NonLiving
{
public:
	SoundRange(Mesh *mesh, Shape *bound, float mass,
			bool visible, bool reverseCollision,
			Vector3 soundForce, float multiplier)
			: NonLiving(mesh, bound, mass, visible, reverseCollision)
	{
		this->type = 4;
		this->multiplier = multiplier;
		this->soundForce = soundForce;
	}

	SoundRange();
	~SoundRange();

	//Accessors
	float getMultiplier(void)const;
	Vector3 getSound(void)const;

	//Mutators
	void setMultiplier(float);
	void setSound(Vector3);
private:
	Vector3 soundForce;
	float multiplier;

};