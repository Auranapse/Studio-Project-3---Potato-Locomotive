#include "NonLiving.h"
/******************************************************************************/
/*!
\brief	
constructor
*/
/******************************************************************************/
NonLiving::NonLiving()
{}
/******************************************************************************/
/*!
\brief	
destructor
*/
/******************************************************************************/
NonLiving::~NonLiving()
{
}


/******************************************************************************/
/*!
\brief	
Collision Effect activated upon reverse
\return
true = Effect Without Collide
*/
/******************************************************************************/
bool NonLiving::getReverse(void)const
{
	return this->reverseCollision;
}
/******************************************************************************/
/*!
\brief	
Gets Visibility
\return
true = visible
*/
/******************************************************************************/
bool NonLiving::getVisible(void)const
{
	return this->visible;
}

/******************************************************************************/
/*!
\brief	
sets Collision Effect activated upon reverse
\param reverse
Collision Effect Activate on Reverse
*/
/******************************************************************************/
void NonLiving::setReverse(bool reverse)
{
	this->reverseCollision = reverse;
}
/******************************************************************************/
/*!
\brief	
Sets Visibility
\param visible
Visibility
*/
/******************************************************************************/
void NonLiving::setVisible(bool visible)
{
	this->visible = visible;
}