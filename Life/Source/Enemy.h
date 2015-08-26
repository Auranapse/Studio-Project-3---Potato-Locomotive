/******************************************************************************/
/*!
\file	Enemy.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Class to define Enemies in Scene
*/
/******************************************************************************/
#include "Living.h"
/******************************************************************************/
/*!
		Class Enemy:
\brief	Defines an Enemy in the Scene
*/
/******************************************************************************/
class Enemy : public Living
{
public:
	Enemy(Mesh *mesh, Shape *bound, float mass,
				int health, Vector3 velo, 
				Vector3 acc, Vector3 force, 
				int damage, float walkForce,
				int alertLevel) 
				: Living(mesh, bound, mass, health, velo, acc, force)
	{
		this->alertLevel = alertLevel;
		this->type = 1;
		this->damage = damage;
		this->walkForce = walkForce;
	}
	Enemy();
	~Enemy();
	void update(double dt, float speed);

	//Accessor(s)
	int getDamage(void)const;
	float getWalk(void)const;

	//Mutator(s)
	void setDamage(int);
	void setWalk(float);
private:
	int damage;
	int alertLevel;
	float walkForce;
	//STATE
};