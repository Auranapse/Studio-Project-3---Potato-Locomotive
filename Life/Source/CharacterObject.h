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

#include "Mesh.h"
#include "MeshBuilder.h"
#include "LoadOBJ.h"
#include "LoadTGA.h"
#include "Vector3.h"
#include "Anim4.h"
#include "ItemObject.h"


class CharacterObject
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
	Position of the character
	*/
	/******************************************************************************/
	Vector3 Position;

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
	Character velocity
	*/
	/******************************************************************************/
	Vector3 Velocity;

	/******************************************************************************/
	/*!
	\brief
	Scale of the character
	*/
	/******************************************************************************/
	Vector3 Scale;

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
	void setPosition(Vector3 &newpos);
	Vector3 getPosition(void);
	Vector3 getDirection(void);

	ItemObject *holding;

	virtual void Init(Vector3 Pos, Vector3 Lookat, const char* texturedir);
	void HoldObject(ItemObject *obj);
	void UpdateHold(double &dt);
	virtual void Update(double &dt);

	CharacterObject();
	~CharacterObject();
};

#endif