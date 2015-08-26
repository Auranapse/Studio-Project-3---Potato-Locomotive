#include "Room.h"

/******************************************************************************/
/*!
\brief	
Constructor
*/
/******************************************************************************/
Room::Room(){}
/******************************************************************************/
/*!
\brief	
Destructor
*/
/******************************************************************************/
Room::~Room(){}

/******************************************************************************/
/*!
\brief	
Gets coEfficient of Static Friction
\return
coEfficient Static Friction
*/
/******************************************************************************/
float Room::getStatic(void)const
{
	return this->staticFriction;
}
/******************************************************************************/
/*!
\brief	
Gets coEfficient of Kinetic Friction
\return
coEfficient Kinetic Friction
*/
/******************************************************************************/
float Room::getKinetic(void)const
{
	return this->kineticFriction;
}

/******************************************************************************/
/*!
\brief	
Sets coEfficient of Static Friction
\param staticF
coEfficient Static Friction
*/
/******************************************************************************/
void Room::setStatic(float staticF)
{
	this->staticFriction = staticF;
}
/******************************************************************************/
/*!
\brief	
Sets coEfficient of Kinetic Friction
\param kineticF
coEfficient kinetic Friction
*/
/******************************************************************************/
void Room::setKinetic(float kineticF)
{
	this->kineticFriction = kineticF;
}