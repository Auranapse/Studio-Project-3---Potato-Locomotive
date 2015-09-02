/****************************************************************************/
/*!
\file ItemObject.h
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Item Object class
!*/
/****************************************************************************/
#ifndef ITEMOBJECT_H
#define ITEMOBJECT_H

#include "GameObject.h"

class ItemObject : public GameObject
{
public:
	ItemObject();
	virtual ~ItemObject();

	bool isWeapon;
	bool isGun;

	Vector3 pos1;
	Vector3 Rotation1;

	virtual void Update(double &dt);
};

#endif