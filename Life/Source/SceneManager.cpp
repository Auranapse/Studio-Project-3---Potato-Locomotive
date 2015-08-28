#include "SceneManager.h"
#include <math.h>


SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

/******************************************************************************/
/*!
\brief	Adds Asset into Manager

\param	newAsset
Asset To Add
*/
/******************************************************************************/
void SceneManager::Add(Asset* newAsset)
{
	this->SceneAssets.push_back(newAsset);
}

/******************************************************************************/
/*!
\brief	Updates Collision Checks/Effects and call Living Assets' Updates

\param	dt
Time used for the Update
\param speed
Speed of Living Assets in Scene
*/
/******************************************************************************/
void SceneManager::Update(double dt, float speed)
{
	//Transverse through for Collision
	for (unsigned i = 0; i < SceneAssets.size(); ++i)
	{
		for (unsigned k = i + 1; k < SceneAssets.size(); ++k)
		{
			if (checkCollision(SceneAssets[i], SceneAssets[k]))//Does it collide?
			{
				effCollision(SceneAssets[i], SceneAssets[k]);//Enforce collision effect
			}
		}
	}

	//Transverse through for Living Assets Update
	for (unsigned i = 0; i < SceneAssets.size(); ++i)
	{
		if (SceneAssets[i]->getFixed() == false)
		{
			Living* targetAsset = (Living*)(SceneAssets[i]);
			targetAsset->update(dt, speed);
		} 
	}
}

/******************************************************************************/
/*!
\brief	
Checks if two Assets collided with each other
\param	a1
First Asset for Check Collision
\param a2
Second Asset to Check Collision With a1

\return
TRUE if collided
*/
/******************************************************************************/
bool SceneManager::checkCollision(Asset* a1, Asset* a2)//Collision Check
{	
	//Assigns lower type to c1, higher to c2
	int c1, c2;
	c1 = (a1->getBound()->getType() < a2->getBound()->getType()) ? (c1 = a1->getBound()->getType()) : (c1 = a2->getBound()->getType());
	c2 = (a2->getBound()->getType() > a1->getBound()->getType()) ? (c2 = a2->getBound()->getType()) : (c2 = a1->getBound()->getType());

	if (a2->getBound()->getType() < a1->getBound()->getType())
	{
		Asset* Temp = a1;
		a1 = a2;
		a2 = Temp;
	}

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
/******************************************************************************/
/*!
\brief	
Performs the Effect due to Collision on the two Assets
\param	a1
First Asset for Check Collision
\param a2
Second Asset to Check Collision With a1
*/
/******************************************************************************/
void SceneManager::effCollision(Asset* a1, Asset* a2)//Collision effect
{
	int c1, c2;
	c1 = (a1->getType() < a2->getType()) ? (c1 = a1->getType()) : (c1 = a2->getType());
	c2 = (a2->getType() > a1->getType()) ? (c2 = a2->getType()) : (c2 = a1->getType());

	if (a2->getType() < a1->getType())
	{
		Asset* Temp = a1;
		a1 = a2;
		a2 = Temp;
	}

	switch(c1)
	{
	case 0://Player Effects
		switch(c2)
		{
		case 0://Player To Player
			PLAYER_PLAYER(a1, a2);
			break;
		case 1://Player To Enemy
			PLAYER_ENEMY(a1, a2);
			break;
		case 2://Player To Projectile
			PLAYER_PROJECTILE(a1, a2);
			break;
		case 3://Player To Room
			PLAYER_ROOM(a1, a2);
			break;
		case 4://Player To SoundRange
			PLAYER_SOUND(a1, a2);
			break;
		case 5:
			PLAYER_WALL(a1, a2);
			break;
		case 6:
			PLAYER_WEAPON(a1, a2);
			break;
		}
		break;
	case 1://Enemy Collisions
		switch(c2)
		{
		case 1://Enemy To Enemy
			ENEMY_ENEMY(a1, a2);
			break;
		case 2://Enemy To Projectile
			ENEMY_PROJECTILE(a1, a2);
			break;
		case 3://Enemy To Room
			ENEMY_ROOM(a1, a2);
			break;
		case 4://Enemy To SoundRange
			ENEMY_SOUND(a1, a2);
			break;
		case 5:
			ENEMY_WALL(a1, a2);
		case 6:
			ENEMY_WEAPON(a1, a2);
		}
		break;
	case 2://Projectile Collisions
		switch(c2)
		{
		case 2://Projectile To Projectile
			PROJECTILE_PROJECTILE(a1, a2);
			break;
		case 3://Projectile To Room
			PROJECTILE_ROOM(a1, a2);
			break;
		case 4://Projectile To SoundRange
			PROJECTILE_SOUND(a1, a2);
			break;
		case 5:
			PROJECTILE_WALL(a1, a2);
			break;
		case 6:
			PROJECTILE_WEAPON(a1, a2);
			break;
		}
		break;

	case 3: //Room Collisions
		{
			switch (c2)
			{
			case 6:
				ROOM_WEAPON(a1, a2);
				break;
			}
			break;
		}
	case 6: //Weapon Collisions
		{
			switch(c2)
			{
				case 5:
				WEAPON_WALL(a1, a2);
				break;
			case 6:
				WEAPON_WEAPON(a1, a2);
				break;
			}
			break;
		}
	}
}


/************************************************************************
//Collision Checks
*************************************************************************/
//Sphere Collisions
/******************************************************************************/
/*!
\brief	
Checks if Sphere/Sphere collided with each other
\param	a1
Sphere Shape for Collision
\param a2
Sphere Shape for Collision

\return
TRUE if collided
*/
/******************************************************************************/
bool SceneManager::SPHERE_SPHERE(Asset* a1, Asset* a2)
{
	Sphere* c1 = (Sphere*)(a1->getBound());
	Sphere* c2 = (Sphere*)(a2->getBound());
	//Distance Between Their Centres
	float d = pow((c1->getOrigin().x - c2->getOrigin().x), 2)
		+ pow((c1->getOrigin().y - c2->getOrigin().y), 2)
		+ pow((c1->getOrigin().z - c2->getOrigin().z), 2);
	//Combined Radius of Two Bounding Spheres
	float tR = pow((c1->getRadius() + c2->getRadius()), 2);
	if (d <= tR)
		return true;
	return false;
}
/******************************************************************************/
/*!
\brief	
Checks if Sphere/AABB collided with each other
\param	a1
Sphere Shape for Collision
\param a2
AABB Shape for Collision

\return
TRUE if collided
*/
/******************************************************************************/
bool SceneManager::SPHERE_AABB(Asset* a1, Asset* a2)
{
	Sphere* c1 = (Sphere*)(a1->getBound());
	AABB* c2 = (AABB*)(a2->getBound());
	float DistanceBetween = 0;
	float Temp;


	if (c1->getOrigin().x < c2->getMin().x)
	{
		Temp = c1->getOrigin().x - c2->getMin().x;
		DistanceBetween += Temp*Temp;
	}
	else if (c1->getOrigin().x > c2->getMax().x)
	{
		Temp = c1->getOrigin().x - c2->getMax().x;
		DistanceBetween += Temp*Temp;
	}


	if (c1->getOrigin().z < c2->getMin().z)
	{
		Temp = c1->getOrigin().z - c2->getMin().z;
		DistanceBetween += Temp*Temp;
	}
	else if (c1->getOrigin().z > c2->getMax().z){
		Temp = c1->getOrigin().z - c2->getMax().z;
		DistanceBetween += Temp*Temp;
	}

	if (c1->getOrigin().y < c2->getMin().y)
	{
		Temp = c1->getOrigin().y - c2->getMin().y;
		DistanceBetween += Temp*Temp;
	}
	else if (c1->getOrigin().y > c2->getMax().y)
	{
		Temp = c1->getOrigin().y - c2->getMax().y;
		DistanceBetween += Temp*Temp;
	}

	return DistanceBetween <= (c1->getRadius() * c1->getRadius());
}
/******************************************************************************/
/*!
\brief	
Checks if Sphere/Ray collided with each other
\param	a1
Sphere Shape for Collision
\param a2
Ray Shape for Collision

\return
TRUE if collided
*/
/******************************************************************************/
bool SceneManager::SPHERE_RAY(Asset*, Asset*){return false;}
/******************************************************************************/
/*!
\brief	
Checks if Sphere/Plane collided with each other
\param	a1
Sphere Shape for Collision
\param a2
Plane Shape for Collision

\return
TRUE if collided
*/
/******************************************************************************/
bool SceneManager::SPHERE_PLANE(Asset*, Asset*){return false;}

//AABB Collisions
/******************************************************************************/
/*!
\brief	
Checks if AABB/AABB collided with each other
\param	a1
AABB Shape for Collision
\param a2
AABB Shape for Collision

\return
TRUE if collided
*/
/******************************************************************************/
bool SceneManager::AABB_AABB(Asset*, Asset*){return false;}
/******************************************************************************/
/*!
\brief	
Checks if AABB/Ray collided with each other
\param	a1
AABB Shape for Collision
\param a2
Ray Shape for Collision

\return
TRUE if collided
*/
/******************************************************************************/
bool SceneManager::AABB_RAY(Asset*, Asset*){return false;}
/******************************************************************************/
/*!
\brief	
Checks if AABB/Plane collided with each other
\param	a1
AABB Shape for Collision
\param a2
Plane Shape for Collision

\return
TRUE if collided
*/
/******************************************************************************/
bool SceneManager::AABB_PLANE(Asset*, Asset*){return false;}

//Ray Collisions
/******************************************************************************/
/*!
\brief	
Checks if Ray/Ray collided with each other
\param	a1
Ray Shape for Collision
\param a2
Ray Shape for Collision

\return
TRUE if collided
*/
/******************************************************************************/
bool SceneManager::RAY_RAY(Asset*, Asset*){return false;}
/******************************************************************************/
/*!
\brief	
Checks if Ray/Plane collided with each other
\param	a1
Ray Shape for Collision
\param a2
Plane Shape for Collision

\return
TRUE if collided
*/
/******************************************************************************/
bool SceneManager::RAY_PLANE(Asset*, Asset*){return false;}

//Plane Collisions
/******************************************************************************/
/*!
\brief	
Checks if Plane/Plane collided with each other
\param	a1
Plane Shape for Collision
\param a2
Plane Shape for Collision

\return
TRUE if collided
*/
/******************************************************************************/
bool SceneManager::PLANE_PLANE(Asset*, Asset*){return false;}

/************************************************************************
//Collision Effects
*************************************************************************/
//Player Effects
/******************************************************************************/
/*!
\brief	
Performs Player/Player Collision Effect
\param	a1
Player Asset for Collision
\param a2
Player Asset for Collision
*/
/******************************************************************************/
void SceneManager::PLAYER_PLAYER(Asset*, Asset*){}
/******************************************************************************/
/*!
\brief	
Performs Player/Enemy Collision Effect
\param	a1
Player Asset for Collision
\param a2
Enemy Asset for Collision
*/
/******************************************************************************/
void SceneManager::PLAYER_ENEMY(Asset* a1, Asset* a2)
{
	aPlayer* c1 = (aPlayer*)a1;
	Enemy* c2 = (Enemy*)a2;
	float multiplier = 0.2f;
	if (c2->getForce().LengthSquared() == 0)
	{
		c1->applyForce(Vector3(50,0,50));
	}
	c1->applyForce(c2->getForce() * multiplier);

	c1->setHealth(c1->getHealth() - c2->getDamage());
}
/******************************************************************************/
/*!
\brief	
Performs Player/Projectile Collision Effect
\param	a1
Player Asset for Collision
\param a2
Projectile Asset for Collision
*/
/******************************************************************************/
void SceneManager::PLAYER_PROJECTILE(Asset* a1, Asset* a2)
{

}
/******************************************************************************/
/*!
\brief	
Performs Player/Room Collision Effect
\param	a1
Player Asset for Collision
\param a2
Room Asset for Collision
*/
/******************************************************************************/
void SceneManager::PLAYER_ROOM(Asset* a1, Asset* a2)
{
	aPlayer* c1 = (aPlayer*)a1;
	Room* c2 = (Room*)a2;
	float gravity = 10;

	float netForce = c1->getForce().Length();

	//std::cout<<"\n"<<c1->getForce().x<<", "<<c1->getForce().z<<"\n";
	//Friction 
	if (netForce != 0)
	{
		if (!(c1->getMove()))//Standing Still
		{
			Vector3 budgingForce = c1->getMass() * gravity * c2->getStatic();
			budgingForce.y = 0;
			if (netForce > budgingForce.Length())
			{
				c1->setMove(true);
				Vector3 opposingForce = c1->getForce().Normalized() * budgingForce.Length();
				c1->setForce(c1->getForce() - opposingForce);
			}
		}
		else if (c1->getMove())//Moving
		{
			if (c1->getVelo() == Vector3(0,0,0) || c1->getAcc() == Vector3(0,0,0))
			{
			c1->setVelo(Vector3(0,0,0));
			c1->setAcc(Vector3(0,0,0));
			return;
			}
			Vector3 kineticForce = c1->getMass() * gravity * c2->getKinetic();
			Vector3 opposingForce = c1->getVelo().Normalize() * kineticForce.Length();
			opposingForce.y = 0;

			if ((c1->getForce().LengthSquared() < opposingForce.LengthSquared()))//Kinetic Force > Forward Force
			{
				if (c1->getVelo().Normalize() != c1->getAcc().Normalize())//It is Decelerating
				{
					if (c1->getVelo().LengthSquared() < c1->getAcc().LengthSquared())//Deceleration > Velocity
					{
						c1->setVelo(Vector3(0, 0, 0));
						c1->setAcc(Vector3(0, 0, 0));
						c1->setMove(false);
					}
				}
				else
				{	
					c1->setForce(c1->getForce() - opposingForce);
				}
			}
			else
			{
				c1->setVelo(Vector3(0, 0, 0));
				c1->setAcc(Vector3(0, 0, 0));
				c1->setMove(false);
			}
		}
	}

	//Gravity
	if (c1->getBound()->getOrigin().y > 0)
	{
		c1->applyForce(Vector3(0,-10,0));
	}
	else if (c1->getBound()->getOrigin().y < 0)
	{
		Vector3 newBound = c1->getBound()->getOrigin();
		newBound.y = 0;
		c1->getBound()->setOrigin(newBound);
	}
}
/******************************************************************************/
/*!
\brief	
Performs Player/Sound Collision Effect
\param	a1
Player Asset for Collision
\param a2
Sound Asset for Collision
*/
/******************************************************************************/
void SceneManager::PLAYER_SOUND(Asset*, Asset*){}
/******************************************************************************/
/*!
\brief	
Performs Player/Wall Collision Effect
\param	a1
Player Asset for Collision
\param a2
Wall Asset for Collision
*/
/******************************************************************************/
void SceneManager::PLAYER_WALL(Asset*, Asset*){}
/******************************************************************************/
/*!
\brief	
Performs Player/Weapon Collision Effect
\param	a1
Player Asset for Collision
\param a2
Weapon Asset for Collision
*/
/******************************************************************************/
void SceneManager::PLAYER_WEAPON(Asset*, Asset*){}

//Enemy Effects
/******************************************************************************/
/*!
\brief	
Performs Enemy/Enemy Collision Effect
\param	a1
Enemy Asset for Collision
\param a2
Enemy Asset for Collision
*/
/******************************************************************************/
void SceneManager::ENEMY_ENEMY(Asset*, Asset*){}
/******************************************************************************/
/*!
\brief	
Performs Enemy/Projectile Collision Effect
\param	a1
Enemy Asset for Collision
\param a2
Projectile Asset for Collision
*/
/******************************************************************************/
void SceneManager::ENEMY_PROJECTILE(Asset* a1, Asset* a2)
{
	Enemy* c1 = (Enemy*)a1;
	Projectile* c2 = (Projectile*)a2;

	if (c2->getVelo().LengthSquared() > 2500)//Greater than 50 Velocity
		c1->setHealth(c1->getHealth() - 1);

	if (c1->getHealth() <= 0)
		delete a1;

}
/******************************************************************************/
/*!
\brief	
Performs Enemy/Room Collision Effect
\param	a1
Enemy Asset for Collision
\param a2
Room Asset for Collision
*/
/******************************************************************************/
void SceneManager::ENEMY_ROOM(Asset* a1, Asset* a2)
{
	Enemy* c1 = (Enemy*)a1;
	Room* c2 = (Room*)a2;
	float gravity = 10;

	float netForce = c1->getForce().Length();


	//Friction 
	if (netForce != 0)
	{
		if (!(c1->getMove()))//Standing Still
		{
			Vector3 budgingForce = c1->getMass() * gravity * c2->getStatic();
			budgingForce.y = 0;
			if (netForce > budgingForce.Length())
			{
				c1->setMove(true);
				Vector3 opposingForce = c1->getForce().Normalized() * budgingForce.Length();
				c1->setForce(c1->getForce() - opposingForce);
			}
		}
		else if (c1->getMove())//Moving
		{
			if (c1->getVelo() == Vector3(0,0,0) || c1->getAcc() == Vector3(0,0,0))
			{
			c1->setVelo(Vector3(0,0,0));
			c1->setAcc(Vector3(0,0,0));
			return;
			}
			Vector3 kineticForce = c1->getMass() * gravity * c2->getKinetic();
			Vector3 opposingForce = c1->getVelo().Normalize() * kineticForce.Length();
			opposingForce.y = 0;

			if ((c1->getForce().LengthSquared() < opposingForce.LengthSquared()))//Kinetic Force > Forward Force
			{
				if (c1->getVelo().Normalize() != c1->getAcc().Normalize())//It is Decelerating
				{
					if (c1->getVelo().LengthSquared() < c1->getAcc().LengthSquared())//Deceleration > Velocity
					{
						c1->setVelo(Vector3(0, 0, 0));
						c1->setAcc(Vector3(0, 0, 0));
						c1->setMove(false);
					}
				}
				else
				{	
					c1->setForce(c1->getForce() - opposingForce);
				}
			}
			else
			{
				c1->setVelo(Vector3(0, 0, 0));
				c1->setAcc(Vector3(0, 0, 0));
				c1->setMove(false);
			}
		}
	}

	//Gravity
	if (c1->getBound()->getOrigin().y > 0)
	{
		c1->applyForce(Vector3(0,-10,0));
	}
	else if (c1->getBound()->getOrigin().y < 0)
	{
		Vector3 newBound = c1->getBound()->getOrigin();
		newBound.y = 0;
		c1->getBound()->setOrigin(newBound);
	}
}
/******************************************************************************/
/*!
\brief	
Performs Enemy/Sound Collision Effect
\param	a1
Enemy Asset for Collision
\param a2
Sound Asset for Collision
*/
/******************************************************************************/
void SceneManager::ENEMY_SOUND(Asset*, Asset*){}//sound thingy destination
/******************************************************************************/
/*!
\brief	
Performs Enemy/Wall Collision Effect
\param	a1
Enemy Asset for Collision
\param a2
Wall Asset for Collision
*/
/******************************************************************************/
void SceneManager::ENEMY_WALL(Asset*, Asset*){}

/******************************************************************************/
/*!
\brief	
Performs Enemy/Weapon Collision Effect
\param	a1
Enemy Asset for Collision
\param a2
Weapon Asset for Collision
*/
/******************************************************************************/
void SceneManager::ENEMY_WEAPON(Asset*, Asset*){}

//Projectile Effects 
/******************************************************************************/
/*!
\brief	
Performs Projectile/Projectile Collision Effect
\param	a1
Projectile Asset for Collision
\param a2
Projectile Asset for Collision
*/
/******************************************************************************/
void SceneManager::PROJECTILE_PROJECTILE(Asset* a1, Asset* a2)
{
	delete a1;
	delete a2;
}
/******************************************************************************/
/*!
\brief	
Performs Projectile/Room Collision Effect
\param	a1
Projectile Asset for Collision
\param a2
Room Asset for Collision
*/
/******************************************************************************/ 
void SceneManager::PROJECTILE_ROOM(Asset*, Asset*){}
/******************************************************************************/
/*!
\brief	
Performs Projectile/Sound Collision Effect
\param	a1
Projectile Asset for Collision
\param a2
Sound Asset for Collision
*/
/******************************************************************************/
void SceneManager::PROJECTILE_SOUND(Asset*, Asset*){}
/******************************************************************************/
/*!
\brief	
Performs Projectile/Wall Collision Effect
\param	a1
Projectile Asset for Collision
\param a2
Wall Asset for Collision
*/
/******************************************************************************/
void SceneManager::PROJECTILE_WALL(Asset*, Asset*){}

/******************************************************************************/
/*!
\brief	
Performs Projectile/Weapon Collision Effect
\param	a1
Projectile Asset for Collision
\param a2
Weapon Asset for Collision
*/
/******************************************************************************/
void SceneManager::PROJECTILE_WEAPON(Asset*, Asset*){}

/******************************************************************************/
/*!
\brief	
Performs Room/Weapon Collision Effect
\param	a1
Room Asset for Collision
\param a2
Weapon Asset for Collision
*/
/******************************************************************************/
void SceneManager::ROOM_WEAPON(Asset*, Asset*){}

//Weapon Effects
/******************************************************************************/
/*!
\brief	
Performs Weapon/Weapon Collision Effect
\param	a1
Weapon Asset for Collision
\param a2
Weapon Asset for Collision
*/
/******************************************************************************/

void SceneManager::WEAPON_WEAPON(Asset*, Asset*){}
/******************************************************************************/
/*!
\brief	
Performs Weapon/Wall Collision Effect
\param	a1
Weapon Asset for Collision
\param a2
Wall Asset for Collision
*/
/******************************************************************************/

void SceneManager::WEAPON_WALL(Asset*, Asset*){}