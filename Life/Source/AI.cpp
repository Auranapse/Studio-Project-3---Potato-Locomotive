/****************************************************************************/
/*!
\file AI.cpp
\author I'sa Hong Xi
\par email: dreamwalker56@gmail.com
\brief
Handles AI position and physics
!*/
/****************************************************************************/
#include "AI.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
AI::AI(): 
f_alert_timer(0.f)
{

}

AI::AI(E_AI_STATE e_State, E_TYPE e_Type):
f_alert_timer(0.f)
{
	this->e_State = e_State;
	this->e_Type = e_Type;
}

/******************************************************************************/
/*!
\brief
Default destructor
*/
/******************************************************************************/
AI::~AI()
{

}

/******************************************************************************/
/*!
\brief
makes AI run/walk
\param forward
if true will run, else walk
*/
/******************************************************************************/
void AI::movementRW(bool run)
{
	if (run)
	{
		f_movementSpeed = f_move_run;
	}
	else
	{
		f_movementSpeed = f_move_walk;
	}
}

/******************************************************************************/
/*!
\brief
moves AI forward/back
\param forward
if true will move forward, else back
*/
/******************************************************************************/
void AI::movementFB(double &dt, bool forward)
{
	if (forward)
	{
		Velocity += (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);
	}
	else
	{
		Velocity -= (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);
	}
}

/******************************************************************************/
/*!
\brief
moves AI left/right
\param left
if true will move left, else right
*/
/******************************************************************************/
void AI::movementLR(double &dt, bool left)
{
	Mtx44 rotation;
	if(left == true)
	{
		rotation.SetToRotation(-90.f, 0.f, 1.f, 0.f);
		Lookat = rotation * Lookat;
		Velocity -= (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);

	}

	else
	{
		rotation.SetToRotation(90.f, 0.f, 1.f, 0.f);
		Lookat = rotation * Lookat;
		Velocity -= (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);
	}
}

/******************************************************************************/
/*!
\brief
Update the sensors for pathfinding
\param left
sensors made to see if there is anything in the way of the AI
*/
/******************************************************************************/
void AI::SensorUpdate(double &dt, bool left, bool mid, bool right)
{
	//when right has nothing to collide
	if(left == true && mid == true, right == false)
	{
		movementLR(dt, false);
	}

	//when left has nothing to collide
	else if(left == false && mid == true, right == true)
	{
		movementLR(dt, true);
	}

	//when middle has nothing to collide
	else if(left == true && mid == false, right == true)
	{
		movementFB(dt, true);
	}

	else if (left == false && mid == false, right == false)
	{
		movementFB(dt, true);
	}
}

/******************************************************************************/
/*!
\brief
Updates the AI
\param dt
delta time
CharacterObject vector list - To check collision
GameObject vector list - To check collision
*/
/******************************************************************************/
void AI::Update(double &dt, Vector3 playerPos, std::vector<CharacterObject *> &m_charList, std::vector<GameObject*> &m_GOList)
{
	static Vector3 destination;
	/*
	if(isVisible(Position, Lookat, 50, playerPos) && (Position - playerPos).LengthSquared() <= 144)
	{
		e_State = ATTACK;
	}
	f_movementSpeed = 1000;*/
	switch(e_State)
	{
	case ATTACK:
		{
			/*
			this->Lookat = playerPos;

			if(this->holding != NULL)
			{
				if(this->holding->isGun)
				{

				}
			}
			else
			{
				//Enemy would only move towards player if the difference between them is 15
				Vector3 diff = playerPos - Position;
				diff.Normalize();
				Velocity.x = diff.x * f_movementSpeed * static_cast<float>(dt);
				Velocity.z = diff.z * f_movementSpeed * static_cast<float>(dt);
			}

			//Change enemy state to walking if distance from player is more than 50
			if((playerPos - Position).LengthSquared() > 10000)
			{
				e_State = WALKING;
			}//*/
		}
		break;

	case WALKING:
		{
			static float offset = 40.f;
	Mtx44 rotation;
	rotation.SetToRotation(CalAnglefromPosition(Lookat, Position, true), 0.f, 1.f, 0.f);
	Vector3 L, R, C;
	C = rotation * Vector3(0.f, 0.f, 20.f);
	L = rotation * Vector3(-20.f, 0.f, 15.f);
	R = rotation * Vector3(20.f, 0.f, 15.f);

	SensorUpdate(dt, collisionChecking(Position + L , m_charList, m_GOList), collisionChecking(Position + R, m_charList, m_GOList), collisionChecking(Position + C, m_charList, m_GOList));
			/*if(isVisible(Position, Lookat, 65, playerPos) && (Position - playerPos).LengthSquared() < 10000)
			{
				e_State = ALERT;
				destination = playerPos;
			}

			//Ensure that the enemy would move to its default pos for now
			if(Position != Vector3(0, 0, 0))
			{
				Lookat = Vector3(0, 0, 0);
				Vector3 diff = Vector3(0, 0, 0) - Position;
				diff.Normalize();

				Velocity.x = diff.x * f_movementSpeed * static_cast<float>(dt);
				Velocity.z = diff.z * f_movementSpeed * static_cast<float>(dt);

				if(Collision::Length(Position, Vector3(0, 0, 0), 2, false))
				{
					Position.SetZero();
				}

			}
			else
				Lookat = defaultLookat;//*/
		}
		break;

	case ALERT:
		{
			/*
			Lookat = destination;

			//Make the enemy move towards the destination
			if(Position.x != destination.x && Position.z != destination.z)
			{
				Vector3 diff = destination - Position;
				diff.Normalize();

				Velocity.x = diff.x * f_movementSpeed * static_cast<float>(dt);
				Velocity.z = diff.z * f_movementSpeed * static_cast<float>(dt);

				if((Position - destination).LengthSquared() < 30)
				{
					Position.x = destination.x;
					Position.z = destination.z;
				}
			}//*/
			//If the enemy is at the destination
			//else
			{
				f_alert_timer += (float)dt;

				if(f_alert_timer >= 5.f)
				{
					e_State = ATTACK;
					f_alert_timer = 0.f;
				}
			}
		}
		break;
	}


	//Collision Handling
	
	////=============Positive X Velocity=============//
	//if(collisionChecking(Vector3(Position + Vector3(10.f, 10.f, 0.f)), m_charList, m_GOList, false) || collisionChecking(Vector3(Position + Vector3(10.f, 30.f, 0.f)), m_charList, m_GOList, false))
	//{
	//	//First if statement is to check against Character Object - AI
	//	Position.x = Position.x +  offset * static_cast<float>(dt);
	//	/*if(Velocity.x > 0)
	//	{
	//	Velocity.x = 0;
	//	}*/

	//	SensorUpdate(dt, collisionChecking(Position + L , m_charList, m_GOList), collisionChecking(Position + R, m_charList, m_GOList), collisionChecking(Position + C, m_charList, m_GOList));
	//}
	//else if(collisionChecking(Vector3(Position + Vector3(10.f, 10.f, 0.f)), m_charList, m_GOList) || collisionChecking(Vector3(Position + Vector3(10.f, 50.f, 0.f)), m_charList, m_GOList))
	//{
	//	//Else if is to check against GameObject - Walls
	//	/*if(Velocity.x > 0)
	//	{
	//	Velocity.x = 0;
	//	}*/
	//	
	//	SensorUpdate(dt, collisionChecking(Position + L , m_charList, m_GOList), collisionChecking(Position + R, m_charList, m_GOList), collisionChecking(Position + C, m_charList, m_GOList));
	//}

	////=============Positive Z Velocity=============//
	//if(collisionChecking(Vector3(Position + Vector3(0.f, 10.f, 10.f)), m_charList, m_GOList, false) || collisionChecking(Vector3(Position + Vector3(0.f, 30.f, 10.f)), m_charList, m_GOList, false))
	//{
	//	Position.z = Position.z +  offset * static_cast<float>(dt);
	//	/*if(Velocity.z > 0)
	//	{
	//	Velocity.z = 0;
	//	}*/

	//	SensorUpdate(dt, collisionChecking(Position + L , m_charList, m_GOList), collisionChecking(Position + R, m_charList, m_GOList), collisionChecking(Position + C, m_charList, m_GOList));
	//}
	//else if(collisionChecking(Vector3(Position + Vector3(0.f, 10.f, 10.f)), m_charList, m_GOList) || collisionChecking(Vector3(Position + Vector3(0.f, 50.f, 10.f)), m_charList, m_GOList))
	//{
	//	/*if(Velocity.z > 0)
	//	{
	//	Velocity.z = 0;
	//	}*/

	//	SensorUpdate(dt, collisionChecking(Position + L , m_charList, m_GOList), collisionChecking(Position + R, m_charList, m_GOList), collisionChecking(Position + C, m_charList, m_GOList));
	//}

	////=============Negative X Velocity=============//
	//if(collisionChecking(Vector3(Position + Vector3(-10.f, 10.f, 0.f)), m_charList, m_GOList, false) || collisionChecking(Vector3(Position + Vector3(-10.f, 30.f, 0.f)), m_charList, m_GOList, false))
	//{
	//	Position.x = Position.x - offset * static_cast<float>(dt);
	//	if(Velocity.x < 0)
	//	{
	//		Velocity.x = 0;
	//	}
	//}
	//else if(collisionChecking(Vector3(Position + Vector3(-10.f, 10.f, 0.f)), m_charList, m_GOList) || collisionChecking(Vector3(Position + Vector3(-10.f, 50.f, 0.f)), m_charList, m_GOList))
	//{
	//	if(Velocity.x < 0)
	//	{
	//		Velocity.x = 0;
	//	}
	//}

	////=============Negative Z Velocity=============//
	//if(collisionChecking(Vector3(Position + Vector3(0.f, 10.f, -10.f)), m_charList, m_GOList, false) || collisionChecking(Vector3(Position + Vector3(0.f, 30.f, -10.f)), m_charList, m_GOList, false))
	//{
	//	Position.z = Position.z - offset * static_cast<float>(dt);
	//	if(Velocity.z < 0)
	//	{
	//		Velocity.z = 0;
	//	}
	//}
	//else if(collisionChecking(Vector3(Position + Vector3(0.f, 10.f, -10.f)), m_charList, m_GOList) || collisionChecking(Vector3(Position + Vector3(0.f, 50.f, -10.f)), m_charList, m_GOList))
	//{
	//	if(Velocity.z < 0)
	//	{
	//		Velocity.z = 0;
	//	}
	//}

		

		if (Velocity.x != 0)
	{
		float SForceX = 0 - Velocity.x;
		Velocity.x += SForceX * 0.1f;
	}

	if (Velocity.z != 0)
	{
		float SForceZ = 0 - Velocity.z;
		Velocity.z += SForceZ * 0.1f;
	}

		//if((Position - playerPos).LengthSquared() > 400)
		Animation.Update(dt, Velocity.LengthSquared() * 0.03);
		Lookat += Velocity * static_cast<float>(dt);
		Position += Velocity * static_cast<float>(dt);
	//else
		//Velocity = 0;
}

/******************************************************************************/
/*!
\brief
To detect collision
\param dt
Vector3 pos
CharacterObject vector list - To check collision
GameObject vector list - To check collision
bool checkGO - 
*/
/******************************************************************************/
bool AI::collisionChecking(Vector3 &pos, std::vector<CharacterObject *> &m_charList, std::vector<GameObject *> &m_GOList, bool checkGO)
{
	//Check against GameObject
	if(checkGO)
	{
		for(std::vector<GameObject*>::iterator it = m_GOList.begin(); it != m_GOList.end(); it++)
		{
			GameObject* go = (GameObject*)*it;
			if(go->active && go->colEnable && go->pos != Position)
			{
				if(intersect(go->pos + go->ColBox, go->pos - go->ColBox, pos))
				{
					return true;
				}
			}
		}
	}
	//Check against Character Object
	else
	{
		for(std::vector<CharacterObject*>::iterator it = m_charList.begin(); it != m_charList.end(); it++)
		{
			CharacterObject * CO = (CharacterObject*)*it;
			if(CO->getPosition() != Position)
			{
				static Vector3 test = Vector3(20, 20, 20);
				if(intersect(CO->getPosition() + test,CO->getPosition() - test, Position))
				{
					return true;
				}
			}
		}
	}
	return false;
}