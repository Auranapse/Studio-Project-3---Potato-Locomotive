#include "NonLiving.h"
#pragma once
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