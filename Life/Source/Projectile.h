/******************************************************************************/
/*!
\file	Projectile.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Class to define Projectiles in Scene
*/
/******************************************************************************/
#include "Living.h"
/******************************************************************************/
/*!
		Class Projectile:
\brief	Defines Projectiles in Scene that have Collision(Not Particles)
*/
/******************************************************************************/
class Projectile : public Living
{
public:
	Projectile(Mesh *mesh, Shape *bound, float mass,
				int health, Vector3 velo, 
				Vector3 acc, Vector3 force, 
				int damage, int lifetime) 
				: Living(mesh, bound, mass, health, velo, acc, force)
	{
		this->type = 2;
		this->damage = damage;
		this->lifetime = lifetime;
	}
	Projectile();
	~Projectile();
	void update(double dt, float speed);

	//Accessor(s)
	int getDamage(void)const;
	int getLife(void)const;

	//Mutator(s)
	void setDamage(int);
	void setLife(int);
private:
	int damage;
	int lifetime;
};