#include "Living.h"

class Enemy : public Living
{
public:
	Enemy(Mesh *mesh, Shape *bound, float mass,
				int health, Vector3 velo, 
				Vector3 acc, Vector3 force, 
				int damage, float walkForce) 
				: Living(mesh, bound, mass, health, velo, acc, force)
	{
		this->damage;
		this->walkForce = walkForce;
	}
	Enemy();
	~Enemy();

	//Accessor(s)
	int getDamage(void)const;
	float getWalk(void)const;

	//Mutator(s)
	void setDamage(int);
	void setWalk(float);
private:
	int damage;
	float walkForce;
};