#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "Mesh.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,

		GO_2D_STATIC_BLOCK,
		GO_2D_DOOR_EXIT,

		GO_BALL,
		GO_STORAGE_CUBE_OLD,
		GO_STORAGE_CUBE,
		GO_COMPANION_CUBE,
		GO_PLAYER_CHAR,

		GOW_WALL_PANEL,
		GOW_WALL_PANEL2,
		GOW_WALL_RUSTED1,
		GOW_FLOOR_TILE,

		GO_PORTAL,
		GO_PORTAL_BULLET,
		
		GO_TOTAL, //must be last
	};

	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	float rotationalVelocity;
	Vector3 scale;
	Vector3 ColBox;
	Vector3 ColBoxOffset;
	bool active;
	bool enablePhysics;
	bool colEnable;
	bool dynamicRendering;
	bool portalble;
	float mass;
	float rotationX, rotationY, rotationZ;
	Mesh* obMesh;

	Material material;
	float lightRadius;
	float lightPower;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif