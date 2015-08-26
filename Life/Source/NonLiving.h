/******************************************************************************/
/*!
\file	SoundRange.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Abstract Class to define NonLiving Assets in the Scene
*/
/******************************************************************************/
#include "Asset.h"
#pragma once
/******************************************************************************/
/*!
		Class NonLiving:
\brief	Defines NonLiving Assets in the Scene
*/
/******************************************************************************/
class NonLiving : public Asset
{
public:
	NonLiving(Mesh *mesh, Shape *bound, float mass,
			bool visible = false, bool reverseCollision = false) 
			: Asset(mesh, bound, mass)
	{
		this->visible = visible;
		this->reverseCollision = reverseCollision;
		this->isFixed = true;

	}
	NonLiving();
	virtual ~NonLiving() = 0;

	//Accessors
	bool getReverse(void)const;
	bool getVisible(void)const;

	//Mutators
	void setReverse(bool);
	void setVisible(bool);
protected:
	bool reverseCollision;
	bool visible;
	//Collision Effect takes place upon No Collision instead of Colliding
};