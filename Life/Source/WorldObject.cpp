/****************************************************************************/
/*!
\file WorldObject.cpp
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
World Object class
!*/
/****************************************************************************/
#include "WorldObject.h"

WorldObject::WorldObject()
{
	holdable = false;
	enablePhysics = false;
	mass = 0.f;
}

WorldObject::~WorldObject()
{

}