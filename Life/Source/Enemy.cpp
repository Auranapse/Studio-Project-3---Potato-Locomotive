#include "Enemy.h"
/******************************************************************************/
/*!
\brief	
constructor
*/
/******************************************************************************/
Enemy::Enemy(){}
/******************************************************************************/
/*!
\brief	
destructor
*/
/******************************************************************************/
Enemy::~Enemy(){  }

/******************************************************************************/
/*!
\brief	
Updates the Acceleration/Velocity and Strategies of Enemies
\param dt
	time stamp
\param speed
	the speed at which they are moving(scalar to multiply)
*/
/******************************************************************************/
void Enemy::update(double dt, float speed)
{
	if (this->getMove())
	{ 
		this->acc = this->force * (float)(1 / this->mass);
		this->velo += acc;
		this->getBound()->moveTo(this->getBound()->getOrigin() + (velo * speed * static_cast<float>(dt)));
	}

}

/******************************************************************************/
/*!
\brief	
Gets damage of Enemies
\return
Damage
*/
/******************************************************************************/
int Enemy::getDamage(void)const
{
	return this->damage;
}

/******************************************************************************/
/*!
\brief	
Sets Damage of Enemies
\param dmg
	damage
*/
/******************************************************************************/
void Enemy::setDamage(int dmg)
{
	this->damage = dmg;
}