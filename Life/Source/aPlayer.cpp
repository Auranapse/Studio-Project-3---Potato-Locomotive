#include "aPlayer.h"
/******************************************************************************/
/*!
\brief	
constructor
*/
/******************************************************************************/
aPlayer::aPlayer(){}
/******************************************************************************/
/*!
\brief	
destructor
*/
/******************************************************************************/
aPlayer::~aPlayer(){}

/******************************************************************************/
/*!
\brief	
Updates Position/Velo/Acc of player and its Moves during the frame
\param dt
time stamp
\param speed
scalar multiplier for time related movements like Velocity/acceleration
*/
/******************************************************************************/
void aPlayer::update(double dt, float speed)
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
Gets damage of Player
\return 
damage
*/
/******************************************************************************/
int aPlayer::getDamage(void)const
{
	return this->damage;
}
/******************************************************************************/
/*!
\brief	
Gets walk Force of Player
\return 
walkForce
*/
/******************************************************************************/
float aPlayer::getWalk(void)const
{
	return this->walkForce;
}
/******************************************************************************/
/*!
\brief	
Gets jump force of Player
\return 
jumpForce
*/
/******************************************************************************/
float aPlayer::getJump(void)const
{
	return this->jumpForce;
}
/******************************************************************************/
/*!
\brief	
Gets energy of Player
\return 
energy
*/
/******************************************************************************/
float aPlayer::getEnergy(void)const
{
	return this->energy;
}

/******************************************************************************/
/*!
\brief	
Sets damage of Player
\param dmg
daamge
*/
/******************************************************************************/
void aPlayer::setDamage(int dmg)
{
	this->damage = dmg;
}
/******************************************************************************/
/*!
\brief	
Sets walkForce of Player
\param walk
walkForce
*/
/******************************************************************************/
void aPlayer::setWalk(float walk)
{
	this->walkForce = walk;
}
/******************************************************************************/
/*!
\brief	
Sets jumpForce of Player
\param jump
jumpForce
*/
/******************************************************************************/
void aPlayer::setJump(float jump)
{
	this->jumpForce =jump;
}
/******************************************************************************/
/*!
\brief	
Sets energy of Player
\param energy
energy
*/
/******************************************************************************/
void aPlayer::setEnergy(float energy)
{
	this->energy = energy;
}