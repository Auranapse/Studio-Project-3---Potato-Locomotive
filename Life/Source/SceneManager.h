/******************************************************************************/
/*!
\file	SceneManager.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Class to handle Collisions/Updates of Assets in Scenes
*/
/******************************************************************************/
#include "Sphere.h"
#include "AABB.h"
#include "Ray.h"
#include "AABB.h"

#include "Enemy.h"
#include "aPlayer.h"
#include "Room.h"
#include "Projectile.h"
#include "SoundRange.h"
#include "Asset.h"
#include "Living.h"
#include "NonLiving.h"
#include <vector>
#pragma once
/******************************************************************************/
/*!
		Class SceneManager:
\brief	Handler for Collisions/Updates of Assets in Scenes
*/
/******************************************************************************/
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Add(Asset*);//Adds an Asset into our Vector
	void Update(double dt, float speed);
	void Render();

	bool checkCollision(Asset*, Asset*);//Collision Check
	void effCollision(Asset*, Asset*);//Collision effect


	//Collision Checks
	//Sphere Collisions
	bool SPHERE_SPHERE(Asset*, Asset*);
	bool SPHERE_AABB(Asset*, Asset*);
	bool SPHERE_RAY(Asset*, Asset*);
	bool SPHERE_PLANE(Asset*, Asset*);

	//AABB Collisions
	bool AABB_AABB(Asset*, Asset*);
	bool AABB_RAY(Asset*, Asset*);
	bool AABB_PLANE(Asset*, Asset*);

	//Ray Collisions
	bool RAY_RAY(Asset*, Asset*);
	bool RAY_PLANE(Asset*, Asset*);

	//Plane Collisions
	bool PLANE_PLANE(Asset*, Asset*);

	//Collision Effects
	//Player Effects
	void PLAYER_PLAYER(Asset*, Asset*);
	void PLAYER_ENEMY(Asset*, Asset*);
	void PLAYER_PROJECTILE(Asset*, Asset*);
	void PLAYER_ROOM(Asset*, Asset*);
	void PLAYER_SOUND(Asset*, Asset*);
	void PLAYER_WALL(Asset*, Asset*);
	void PLAYER_WEAPON(Asset*, Asset*);

	//Enemy Effects
	void ENEMY_ENEMY(Asset*, Asset*);
	void ENEMY_PROJECTILE(Asset*, Asset*);
	void ENEMY_ROOM(Asset*, Asset*);
	void ENEMY_SOUND(Asset*, Asset*);
	void ENEMY_WALL(Asset*, Asset*);
	void ENEMY_WEAPON(Asset*, Asset*);

	//Projectile Effects 
	void PROJECTILE_PROJECTILE(Asset*, Asset*);
	void PROJECTILE_ROOM(Asset*, Asset*);
	void PROJECTILE_SOUND(Asset*, Asset*);
	void PROJECTILE_WALL(Asset*, Asset*);
	void PROJECTILE_WEAPON(Asset*, Asset*);

	//Room Effects
	void ROOM_WEAPON(Asset*, Asset*);

	//Weapon Effects
	void WEAPON_WEAPON(Asset*, Asset*);
	void WEAPON_WALL(Asset*, Asset*);
	std::vector<Asset*>SceneAssets;//go private later, this is just for testing...
private:
};