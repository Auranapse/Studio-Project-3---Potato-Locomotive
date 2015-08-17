/****************************************************************************/
/*!
\file ItemObject.cpp
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Item Object class
!*/
/****************************************************************************/
#include "ItemObject.h"

ItemObject::ItemObject()
{
	colEnable = true;
	enablePhysics = true;
	mass = 10.f;
	pickupable = true;
	isWeapon = false;
	isGun = false;
}

ItemObject::~ItemObject()
{

}