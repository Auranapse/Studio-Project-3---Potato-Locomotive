#include "Living.h"

/******************************************************************************/
/*!
\brief	
constructor
*/
/******************************************************************************/
Living::Living()
{}
/******************************************************************************/
/*!
\brief	
destructor
*/
/******************************************************************************/
Living::~Living()
{
	delete this;
}

/******************************************************************************/
/*!
\brief	
Adds To The Net Force on our Asset
\param appliedForce
	Force to Add to Net
*/
/******************************************************************************/
void Living::applyForce(Vector3 appliedForce)
{
	this->force += appliedForce;
}

/******************************************************************************/
/*!
\brief	
Gets Velocity of Living Asset
\return
	Velocity
*/
/******************************************************************************/
Vector3 Living::getVelo(void)const
{
	return this->velo;
}
/******************************************************************************/
/*!
\brief	
Gets Acceleration of Living Asset
\return
	Acceleration
*/
/******************************************************************************/
Vector3 Living::getAcc(void)const
{
	return this->acc;
}
/******************************************************************************/
/*!
\brief	
Gets Force of Living Asset
\return
	Force
*/
/******************************************************************************/
Vector3 Living::getForce(void)const
{
	return this->force;
}
/******************************************************************************/
/*!
\brief	
Gets Health of Living Asset
\return
	Health
*/
/******************************************************************************/
int Living::getHealth(void)const
{
	return this->health;
}
/******************************************************************************/
/*!
\brief	
Is Asset Moving?
\return
	true = Asset is Moving
*/
/******************************************************************************/
bool Living::getMove(void)const
{
	return this->onMove;
}

/******************************************************************************/
/*!
\brief	
Sets Velocity of Living Asset
\param velo
	Velocity
*/
/******************************************************************************/
void Living::setVelo(Vector3 velo)
{
	this->velo = velo;
}
/******************************************************************************/
/*!
\brief	
Sets acceleration of Living Asset
\param acc
	Acceleration
*/
/******************************************************************************/
void Living::setAcc(Vector3 acc)
{
	this->acc = acc;
}
/******************************************************************************/
/*!
\brief	
Sets Net Force of Living Asset
\param force
	Net Force
*/
/******************************************************************************/
void Living::setForce(Vector3 force)
{
	this->force = force;
}
/******************************************************************************/
/*!
\brief	
Sets Health of Living Asset
\param health
	health
*/
/******************************************************************************/
void Living::setHealth(int health)
{
	this->health = health;
}
/******************************************************************************/
/*!
\brief	
Sets if Asset is moving
\param onMove
	Is it moving?
*/
/******************************************************************************/
void Living::setMove(bool onMove)
{
	this->onMove = onMove;
}