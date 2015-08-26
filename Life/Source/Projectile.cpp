#include "Projectile.h"
/******************************************************************************/
/*!
\brief	
constructor
*/
/******************************************************************************/
Projectile::Projectile(){}
/******************************************************************************/
/*!
\brief	
destructor
*/
/******************************************************************************/
Projectile::~Projectile(){}

/******************************************************************************/
/*!
\brief	
Updates the Acceleration/Velocity/Position of Projectile
\param dt
	time stamp
\param speed
	the speed at which they are moving(scalar to multiply)
*/
/******************************************************************************/
void Projectile::update(double dt, float speed)
{
	this->acc = this->force / this->mass;
	this->velo += acc;
	this->getBound()->moveTo(this->getBound()->getOrigin() + (velo * speed * static_cast<float>(dt)));
	this->lifetime -= 1;
	if (this->lifetime <= 0)
		delete this;
}

/******************************************************************************/
/*!
\brief	
Gets damage of Projectile
\return
	damage
*/
/******************************************************************************/
int Projectile::getDamage(void)const
{
	return this->damage;
}
/******************************************************************************/
/*!
\brief	
Gets lifetime of Projectile
\return
	lifetime
*/
/******************************************************************************/
int Projectile::getLife(void)const
{
	return this->lifetime;
}
/******************************************************************************/
/*!
\brief	
Sets damage of projectile
\param dmg
	damage of projectile
*/
/******************************************************************************/
void Projectile::setDamage(int dmg)
{
	this->damage = dmg;
}
/******************************************************************************/
/*!
\brief	
Sets lifetime of projectile
\param life
	lifetime
*/
/******************************************************************************/
void Projectile::setLife(int life)
{
	this->lifetime = life;
}