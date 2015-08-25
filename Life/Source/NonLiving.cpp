#include "NonLiving.h"
/***************************************************************
Constructor/Destructor
****************************************************************/
NonLiving::NonLiving()
{}

NonLiving::~NonLiving()
{}


/***************************************************************
ACCESSOR(S)
****************************************************************/
bool NonLiving::getReverse(void)const
{
	return this->reverseCollision;
}
bool NonLiving::getVisible(void)const
{
	return this->visible;
}

/***************************************************************
MUTATOR(S)
****************************************************************/
void NonLiving::setReverse(bool reverse)
{
	this->reverseCollision = reverse;
}
void NonLiving::setVisible(bool visible)
{
	this->visible = visible;
}