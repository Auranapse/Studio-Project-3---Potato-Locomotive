#include "Asset.h"
#pragma once
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