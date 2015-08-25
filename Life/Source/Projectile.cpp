#include "Projectile.h"

Projectile::Projectile(){}
Projectile::~Projectile(){}

//Accessor(s)
int Projectile::getDamage(void)const
{
	return this->damage;
}

//Mutator(s)
void Projectile::setDamage(int dmg)
{
	this->damage = dmg;
}