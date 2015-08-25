#include "Enemy.h"

Enemy::Enemy(){}
Enemy::~Enemy(){}

//Accessor(s)
int Enemy::getDamage(void)const
{
	return this->damage;
}

//Mutator(s)
void Enemy::setDamage(int dmg)
{
	this->damage = dmg;
}