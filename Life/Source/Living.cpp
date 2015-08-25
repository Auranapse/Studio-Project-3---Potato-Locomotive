#include "Living.h"

/***************************************************************
Constructor/Destructor
****************************************************************/
Living::Living()
{}

Living::~Living()
{}

void Living::applyForce(Vector3 appliedForce)
{
	this->force += appliedForce;
}

/***************************************************************
ACCESSOR(S)
****************************************************************/
Vector3 Living::getVelo(void)const
{
	return this->velo;
}
Vector3 Living::getAcc(void)const
{
	return this->acc;
}
Vector3 Living::getForce(void)const
{
	return this->force;
}
int Living::getHealth(void)const
{
	return this->health;
}

/***************************************************************
MUTATOR(S)
****************************************************************/
void Living::setVelo(Vector3 velo)
{
	this->velo = velo;
}
void Living::setAcc(Vector3 acc)
{
	this->acc = acc;
}
void Living::setForce(Vector3 force)
{
	this->force;
}
void Living::setHealth(int health)
{
	this->health = health;
}