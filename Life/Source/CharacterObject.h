/****************************************************************************/
/*!
\file CharacterObject.h
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Character Object used by AI and player
!*/
/****************************************************************************/
#ifndef CHARACTEROBJECT_H
#define CHARACTEROBJECT_H

#include "GameObject.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "LoadOBJ.h"
#include "LoadTGA.h"
#include "Vector3.h"
#include "Anim4.h"
#include "ItemObject.h"
#include "Mtx44.h"
#include "GabrielDLC.h"

class CharacterObject : public GameObject
{
protected:
	/******************************************************************************/
	/*!
	\brief
	Player animation
	*/
	/******************************************************************************/
	Anim4 Animation;
	
	/******************************************************************************/
	/*!
	\brief
	Default Position of the character
	*/
	/******************************************************************************/
	Vector3 defaultPosition;

public:
	/******************************************************************************/
	/*!
	\brief
	What the character is looking at
	*/
	/******************************************************************************/
	Vector3 Lookat;

	/******************************************************************************/
	/*!
	\brief
	default value of what the character is look at
	*/
	/******************************************************************************/
	Vector3 defaultLookat;
	
	/******************************************************************************/
	/*!
	\brief
	Head Position offset relative to Position
	*/
	/******************************************************************************/
	Vector3 HeadPos;

	/******************************************************************************/
	/*!
	\brief
	Model Position offset relative to Position
	*/
	/******************************************************************************/
	Vector3 ModelPos;

	/******************************************************************************/
	/*!
	\brief
	Arm Position offset relative to Position
	*/
	/******************************************************************************/
	Vector3 ArmPos;

	/******************************************************************************/
	/*!
	\brief
	Leg Position offset relative to Position
	*/
	/******************************************************************************/
	Vector3 LegPos;

	Vector3 CamOffset;

	Mesh *Head;
	Mesh *Chest;
	Mesh *Arm_left;
	Mesh *Arm_right;
	Mesh *Leg_left;
	Mesh *Leg_right;

	float f_move_crawl;
	float f_move_walk;
	float f_move_run;
	float f_movementSpeed;

	Anim4 getAnimation(void);
	Vector3 getDirection(bool XZ = false);

	ItemObject *holding;

	virtual void Init(const Vector3 &Pos, const Vector3 &Lookat, const char* texturedir);
	void HoldObject(ItemObject *obj);
	void DropObject(const Vector3 &ThrowVel = Vector3(0, 0, 0));
	virtual void Update(const double &dt);

	CharacterObject();
	virtual ~CharacterObject();
};

#endif