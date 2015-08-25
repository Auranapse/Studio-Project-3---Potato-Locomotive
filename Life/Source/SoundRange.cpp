#include "SoundRange.h"
/***************************************************************
Constructor/Destructor
****************************************************************/
SoundRange::SoundRange(){}
SoundRange::~SoundRange(){}

/***************************************************************
ACCESSOR(S)
****************************************************************/
float SoundRange::getMultiplier(void)const
{
	return this->multiplier;
}
Vector3 SoundRange::getSound(void)const
{
	return this->soundForce;
}

/***************************************************************
MUTATOR(S)
****************************************************************/
void SoundRange::setMultiplier(float multi)
{
	this->multiplier = multi;
}
void SoundRange::setSound(Vector3 sound)
{
	this->soundForce = sound;
}