/******************************************************************************/
/*!
\file	aPlayer.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Class to define Player in the Scene
*/
/******************************************************************************/
#include "Living.h"
/******************************************************************************/
/*!
		Class aPlayer:
\brief	Defines a Player to control in the Scene
*/
/******************************************************************************/
class aPlayer : public Living
{
public:
	aPlayer(Mesh *mesh, Shape *bound, float mass,
				int health, Vector3 velo, 
				Vector3 acc, Vector3 force, 
				int damage, float walkForce,
				float jumpForce, float energy) 
				: Living(mesh, bound, mass, health, velo, acc, force)
	{
		this->type = 0;
		this->damage;
		this->walkForce = walkForce;
		this->energy = energy;
		this->jumpForce = jumpForce;
	}
	aPlayer();
	~aPlayer();
	void update(double dt, float speed);
	//Accessor(s)
	int getDamage(void)const;
	float getWalk(void)const;
	float getJump(void)const;
	float getEnergy(void)const;

	//Mutator(s)
	void setDamage(int);
	void setWalk(float);
	void setJump(float);
	void setEnergy(float);
private:
	int damage;
	float walkForce;
	float jumpForce;
	float energy;
};