#include "Sphere.h"
#include "AABB.h"
#include "Ray.h"
#include "AABB.h"

#include "Enemy.h"
#include "Room.h"
#include "Projectile.h"
#include "SoundRange.h"
#include "Asset.h"
#include "Living.h"
#include "NonLiving.h"
#include <vector>
#pragma once

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
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

	//Enemy Effects
	void ENEMY_ENEMY(Asset*, Asset*);
	void ENEMY_PROJECTILE(Asset*, Asset*);
	void ENEMY_ROOM(Asset*, Asset*);
	void ENEMY_SOUND(Asset*, Asset*);

	//Projectile Effects 
	void PROJECTILE_PROJECTILE(Asset*, Asset*);
	void PROJECTILE_ROOM(Asset*, Asset*);
	void PROJECTILE_SOUND(Asset*, Asset*);
private:
	std::vector<Asset*>SceneAssets;
};