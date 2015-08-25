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

/******************************************************************************/
/*!
\brief
Default Constructor
*/
/******************************************************************************/
ItemObject::ItemObject()
{
	colEnable = true;
	enablePhysics = true;
	mass = 10.f;
	holdable = true;
	isWeapon = false;
	isGun = false;
}

/******************************************************************************/
/*!
\brief
Default Destructor
*/
/******************************************************************************/
ItemObject::~ItemObject()
{

}

/******************************************************************************/
/*!
\brief
Updates itemobject position on characterobject
\param dt
delta time
*/
/******************************************************************************/
void ItemObject::Update(double &dt)
{
	if (isHeld)
	{
		if (pos != pos1)
		{
			Vector3 v3_temp1 = pos1 - pos;
			pos += v3_temp1 * static_cast<float>(dt) * 12.f;
		}

		if (rotation != Rotation1)
		{
			Vector3 v3_temp1 = Rotation1 - rotation;
			rotation += v3_temp1 * static_cast<float>(dt) * 12.f;
		}

		if (pos.x < pos1.x + 0.01f && pos.x > pos1.x - 0.01f && pos.y < pos1.y + 0.01f && pos.y > pos1.y - 0.01f && pos.z < pos1.z + 0.01f && pos.z > pos1.z - 0.01f)
		{
			pos = pos1;

			if (rotation.x < Rotation1.x + 0.01f && rotation.x > Rotation1.x - 0.01f && rotation.y < Rotation1.y + 0.01f && rotation.y > Rotation1.y - 0.01f && rotation.z < Rotation1.z + 0.01f && rotation.z > Rotation1.z - 0.01f)
			{
				rotation = Rotation1;
			}
		}
	}
}