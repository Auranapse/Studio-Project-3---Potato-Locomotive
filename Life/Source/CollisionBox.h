#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include "GabrielDLC.h"

struct CollisionBox
{
	enum COLLISION_TYPE
	{
		CT_AABB,
		CT_POINT,
		CT_SPHERE,
	};

	COLLISION_TYPE Type;
	Vector3 Position;
	Vector3 ColBox;
	Vector3 ColOffset;

	CollisionBox();
	~CollisionBox();

	bool checkCollision(CollisionBox &CB1, CollisionBox &CB2);
};

#endif
