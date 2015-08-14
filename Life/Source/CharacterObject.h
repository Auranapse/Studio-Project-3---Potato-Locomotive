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
#include "Vector3.h"
#include "Anim4.h"


class CharacterObject
{
protected:
	Anim4 Animation;
	Vector3 Position;

public:
	Vector3 Lookat;
	Vector3 Velocity;
	Vector3 Scale;

	Mesh *Head;
	Mesh *Chest;
	Mesh *Arms;
	Mesh *Legs;

	float f_move_crawl;
	float f_move_walk;
	float f_move_run;
	float f_movementSpeed;

	Anim4 getAnimation(void);
	void setPosition(Vector3 &newpos);
	Vector3 getPosition(void);
	Vector3 getDirection(void);

	virtual void Update(double &dt);

	CharacterObject();
	~CharacterObject();
};

#endif