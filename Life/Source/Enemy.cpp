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
		this->acc = this->force * (double)(1 / this->mass);
		this->velo += acc;
		//if (this->velo.Length() <= 0
		this->getBound()->moveTo(this->getBound()->getOrigin() + (velo * speed * dt));
	}
	//PERFORM SOME STRAT BASED ON GAME STATE
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