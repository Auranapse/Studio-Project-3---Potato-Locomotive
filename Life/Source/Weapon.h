/******************************************************************************/
/*!
\file	Weapon.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Class to define Weapon in Scene
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
class Weapon : public NonLiving
{
public:
	Weapon(Mesh *mesh, Shape *bound, float mass,
			bool visible, bool reverseCollision,
			float ammo, float cooldown,
			float damage, Vector3 targetAim)
			: NonLiving(mesh, bound, mass, visible, reverseCollision)
	{
		this->type = 5;
		this->ammo = ammo;
		this->cooldown = cooldown;
		this->damage = damage;
		this->targetAim = targetAim;
	}

	Weapon();
	~Weapon();

	//Accessors
	float getAmmo(void)const;
	float getCD(void)const;
	float getDamage(void)const;
	Vector3 getAim(void)const;

	//Mutators
	void setAmmo(float);
	void setCD(float);
	void setDmg(float);
	void setAim(Vector3);
private:
	float ammo;
	float cooldown;
	float damage;
	Vector3 targetAim;
};