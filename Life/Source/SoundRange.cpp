#include "SoundRange.h"
/******************************************************************************/
/*!
\brief	
constructor
*/
/******************************************************************************/
SoundRange::SoundRange(){}
/******************************************************************************/
/*!
\brief	
destructor
*/
/******************************************************************************/
SoundRange::~SoundRange(){}

/******************************************************************************/
/*!
\brief	
Gets Scalar to Multiply with Sound's Range
\return
SoundRange's multiplier
*/
/******************************************************************************/
float SoundRange::getMultiplier(void)const
{
	return this->multiplier;
}
/******************************************************************************/
/*!
\brief	
Gets Force that causes the sound to emit
\return
Sound's Force
*/
/******************************************************************************/
Vector3 SoundRange::getSound(void)const
{
	return this->soundForce;
}

/******************************************************************************/
/*!
\brief	
Sets Scalar to Multiply with Sound's Range
\param multi
SoundRange's multiplier
*/
/******************************************************************************/
void SoundRange::setMultiplier(float multi)
{
	this->multiplier = multi;
}
/******************************************************************************/
/*!
\brief	
Sets Force that causes the sound to emit
\param sound
Sound's force
*/
/******************************************************************************/
void SoundRange::setSound(Vector3 sound)
{
	this->soundForce = sound;
}