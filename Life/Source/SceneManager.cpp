#include "SceneManager.h"


SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Update(double dt, float speed)
{
	//Transverse through for Collision
	for (int i = 0; i < SceneAssets.size(); ++i)
	{
		for (int k = 0; k < SceneAssets.size(); ++k)
		{
			if (checkCollision(SceneAssets[i], SceneAssets[k]))//Does it collide?
				effCollision(SceneAssets[i], SceneAssets[k]);//Enforce collision effect
		}
	}


	//Transverse through for Living Assets Update
	for (int i = 0; i < SceneAssets.size(); ++i)
	{
		if (!(SceneAssets[i]->getFixed()))
		{
			Living* targetAsset = (Living*)(SceneAssets[i]);
			targetAsset->update(dt, speed);
		}
	}
}

bool SceneManager::checkCollision(Asset* a1, Asset* a2)//Collision Check
{	
	//Assigns lower type to c1, higher to c2
	int c1, c2;
	c1 = (a1->getBound()->getType() < a2->getBound()->getType()) ? (c1 = a1->getBound()->getType()) : (c1 = a2->getBound()->getType());
	c2 = (a2->getBound()->getType() > a1->getBound()->getType()) ? (c2 = a2->getBound()->getType()) : (c2 = a1->getBound()->getType());

	switch(c1)
	{
	case 0://Sphere Collisions
		switch(c2)
		{
		case 0://Sphere To Sphere
			return SPHERE_SPHERE(a1, a2);
			break;
		case 1://Sphere To AABB
			return SPHERE_AABB(a1, a2);
			break;
		case 2://Sphere To Ray
			return SPHERE_RAY(a1, a2);
			break;
		case 3://Sphere To Plane
			return SPHERE_PLANE(a1, a2);
			break;
		}
		break;
	case 1://AABB Collisions
		switch(c2)
		{
		case 1://AABB To AABB
			return AABB_AABB(a1, a2);
			break;
		case 2://AABB To Ray
			return AABB_RAY(a1, a2);
			break;
		case 3://AABB To Plane
			return AABB_PLANE(a1, a2);
			break;
		}
		break;
	case 2://Ray Collisions
		switch(c2)
		{
		case 2://Ray To Ray
			return RAY_RAY(a1, a2);
			break;
		case 3://Ray To Plane
			return RAY_PLANE(a1, a2);
			break;
		}
		break;
	case 3://Plane Collisions
		switch(c2)
		{
		case 3://Plane To Plane
			return PLANE_PLANE(a1, a2);
			break;
		}
		break;
	}
	return false;
}
void SceneManager::effCollision(Asset* a1, Asset* a2)//Collision effect
{
	int c1, c2;
	c1 = (a1->getType() < a2->getType()) ? (c1 = a1->getType()) : (c1 = a2->getType());
	c2 = (a2->getType() > a1->getType()) ? (c2 = a2->getType()) : (c2 = a1->getType());

	switch(c1)
	{
	case 0://Player Effects
		switch(c2)
		{
		case 0://Player To Player
			return PLAYER_PLAYER(a1, a2);
			break;
		case 1://Player To Enemy
			return PLAYER_ENEMY(a1, a2);
			break;
		case 2://Player To Projectile
			return PLAYER_PROJECTILE(a1, a2);
			break;
		case 3://Player To Room
			return PLAYER_ROOM(a1, a2);
			break;
		case 4://Player To SoundRange
			return PLAYER_SOUND(a1, a2);
			break;
		}
		break;
	case 1://Enemy Collisions
		switch(c2)
		{
		case 1://Enemy To Enemy
			return ENEMY_ENEMY(a1, a2);
			break;
		case 2://Enemy To Projectile
			return ENEMY_PROJECTILE(a1, a2);
			break;
		case 3://Enemy To Room
			return ENEMY_ROOM(a1, a2);
			break;
		case 4://Enemy To SoundRange
			return ENEMY_SOUND(a1, a2);
			break;
		}
		break;
	case 2://Projectile Collisions
		switch(c2)
		{
		case 2://Projectile To Projectile
			return PROJECTILE_PROJECTILE(a1, a2);
			break;
		case 3://Projectile To Room
			return PROJECTILE_ROOM(a1, a2);
			break;
		case 4://Projectile To SoundRange
			return PROJECTILE_SOUND(a1, a2);
			break;
		}
		break;
	}
}


/************************************************************************
//Collision Checks
*************************************************************************/
//Sphere Collisions
bool SceneManager::SPHERE_SPHERE(Asset*, Asset*){return false;}
bool SceneManager::SPHERE_AABB(Asset*, Asset*){return false;}
bool SceneManager::SPHERE_RAY(Asset*, Asset*){return false;}
bool SceneManager::SPHERE_PLANE(Asset*, Asset*){return false;}

//AABB Collisions
bool SceneManager::AABB_AABB(Asset*, Asset*){return false;}
bool SceneManager::AABB_RAY(Asset*, Asset*){return false;}
bool SceneManager::AABB_PLANE(Asset*, Asset*){return false;}

//Ray Collisions
bool SceneManager::RAY_RAY(Asset*, Asset*){return false;}
bool SceneManager::RAY_PLANE(Asset*, Asset*){return false;}

//Plane Collisions
bool SceneManager::PLANE_PLANE(Asset*, Asset*){return false;}

/************************************************************************
//Collision Effects
*************************************************************************/
//Player Effects
void SceneManager::PLAYER_PLAYER(Asset*, Asset*){}
void SceneManager::PLAYER_ENEMY(Asset*, Asset*){}
void SceneManager::PLAYER_PROJECTILE(Asset*, Asset*){}
void SceneManager::PLAYER_ROOM(Asset*, Asset*){}
void SceneManager::PLAYER_SOUND(Asset*, Asset*){}

//Enemy Effects
void SceneManager::ENEMY_ENEMY(Asset*, Asset*){}
void SceneManager::ENEMY_PROJECTILE(Asset*, Asset*){}
void SceneManager::ENEMY_ROOM(Asset*, Asset*){}
void SceneManager::ENEMY_SOUND(Asset*, Asset*){}

//Projectile Effects 
void SceneManager::PROJECTILE_PROJECTILE(Asset*, Asset*){}
void SceneManager::PROJECTILE_ROOM(Asset*, Asset*){}
void SceneManager::PROJECTILE_SOUND(Asset*, Asset*){}