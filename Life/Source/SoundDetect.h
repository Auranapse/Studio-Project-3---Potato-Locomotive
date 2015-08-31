/******************************************************************************/
/*!
\file	SoundDetect.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Class to define Sound Detection Range for Collision on Enemies
*/
/******************************************************************************/
#include "CollisionBox.h"
#include "Vector3.h"

#pragma once
/******************************************************************************/
/*!
Class AABB:
\brief	
Helps to find out whether enemy hears Sound
*/
/******************************************************************************/
class SoundDetect
{
public:
	SoundDetect(Vector3 pos, float soundRadius, float soundMultiplier = 1.0f);
	SoundDetect();
	~SoundDetect();

	bool heard(CollisionBox &target);

	//Accessors
	float getSoundRadius(void)const;
	float getSoundMultplier(void)const;
	CollisionBox getSoundBound(void)const;

	//Mutators
	void setSoundRadius(float);
	void setSoundMultiplier(float);
private:
	float soundMultiplier;
	float soundRadius;
	CollisionBox soundBound;//Defined by soundRadius;
};