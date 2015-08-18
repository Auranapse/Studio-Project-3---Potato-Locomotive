/****************************************************************************/
/*!
\file GameObject.h
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Game Object base class
!*/
/****************************************************************************/
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vector3.h"
#include "Mesh.h"

class GameObject
{
public:
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 rotation;
	Vector3 ColBox;
	Vector3 ColBoxOffset;
	bool active;
	bool enablePhysics;
	bool colEnable;
	bool dynamicRendering;
	bool holdable;
	bool isHeld;
	float mass;
	Mesh* mesh;

	/*Material material;
	float lightRadius;
	float lightPower;*/

	GameObject();
	~GameObject();

	virtual void Update(double &dt);
};

#endif