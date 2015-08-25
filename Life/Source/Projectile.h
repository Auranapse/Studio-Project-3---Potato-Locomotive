#include "Living.h"

class Projectile : public Living
{
public:
	Projectile(Mesh *mesh, Shape *bound, float mass,
				int health, Vector3 velo, 
				Vector3 acc, Vector3 force, int damage) 
				: Living(mesh, bound, mass, health, velo, acc, force)
	{
		this->type = 2;
		this->damage = damage;
	}
	Projectile();
	~Projectile();

	//Accessor(s)
	int getDamage(void)const;

	//Mutator(s)
	void setDamage(int);
private:
	int damage;
};