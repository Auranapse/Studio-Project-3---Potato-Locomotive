#include "Room.h"

/***************************************************************
Constructor/Destructor
****************************************************************/
Room::Room(){}
Room::~Room(){}

/***************************************************************
ACCESSOR(S)
****************************************************************/
float Room::getStatic(void)const
{
	return this->staticFriction;
}
float Room::getKinetic(void)const
{
	return this->kineticFriction;
}

/***************************************************************
MUTATOR(S)
****************************************************************/
void Room::setStatic(float staticF)
{
	this->staticFriction = staticF;
}
void Room::setKinetic(float kineticF)
{
	this->kineticFriction = kineticF;
}