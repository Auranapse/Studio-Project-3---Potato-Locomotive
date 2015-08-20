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
f_alert_timer(0.f),
	CollideWithAI(true)
{
	this->e_State = ATTACK;
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
		Velocity += (getDirection().Normalize() * f_movementSpeed) * static_cast<float>(dt);
	}
	else
	{
		Velocity -= (getDirection().Normalize() * f_movementSpeed) * static_cast<float>(dt);
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

}

/******************************************************************************/
/*!
\brief
Updates the AI
\param dt
delta time
*/
/******************************************************************************/
void AI::Update(double &dt, Vector3 playerPos)
{
	static Vector3 destination;

	if(isVisible(Position, Lookat, 50, playerPos) && (Position - playerPos).LengthSquared() <= 144)
	{
		e_State = ATTACK;
	}
	f_movementSpeed = 1000;
	switch(e_State)
	{
	case ATTACK:
		{
			std::cout << "Attack" << std::endl;
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
				if((playerPos - Position).LengthSquared() > 400)
				{
					Vector3 diff = playerPos - Position;
					diff.Normalize();
					Velocity.x = diff.x * f_movementSpeed * static_cast<float>(dt);
					Velocity.z = diff.z * f_movementSpeed * static_cast<float>(dt);
				}
				else
				{
					std::cout << "Velocity = 0" << std::endl;
					Velocity = 0;
				}
			}

			//Change enemy state to walking if distance from player is more than 50
			/*if((playerPos - Position).LengthSquared() > 10000)
			{
				e_State = WALKING;
			}*/
		}
		break;

	//case WALKING:
	//	{
	//		std::cout << "Walking" << std::endl;
	//		if(isVisible(Position, Lookat, 65, playerPos) && (Position - playerPos).LengthSquared() < 10000)
	//		{
	//			e_State = ALERT;
	//			destination = playerPos;
	//		}

	//		//Ensure that the enemy would move to its default pos for now
	//		if(Position != Vector3(0, 0, 0))
	//		{
	//			Lookat = Vector3(0, 0, 0);
	//			Vector3 diff = Vector3(0, 0, 0) - Position;
	//			diff.Normalize();

	//			Velocity.x = diff.x * f_movementSpeed * static_cast<float>(dt);
	//			Velocity.z = diff.z * f_movementSpeed * static_cast<float>(dt);

	//			if(Collision::Length(Position, Vector3(0, 0, 0), 2, false))
	//			{
	//				Position.SetZero();
	//			}

	//		}
	//		else
	//			Lookat = defaultLookat;
	//	}
	//	break;

	//case ALERT:
	//	{
	//		std::cout << "Alert" << std::endl;
	//		Lookat = destination;

	//		//Make the enemy move towards the destination
	//		if(Position.x != destination.x && Position.z != destination.z)
	//		{
	//			Vector3 diff = destination - Position;
	//			diff.Normalize();

	//			Velocity.x = diff.x * f_movementSpeed * static_cast<float>(dt);
	//			Velocity.z = diff.z * f_movementSpeed * static_cast<float>(dt);

	//			if((Position - destination).LengthSquared() < 30)
	//			{
	//				Position.x = destination.x;
	//				Position.z = destination.z;
	//			}
	//		}
	//		//If the enemy is at the destination
	//		else
	//		{
	//			f_alert_timer += (float)dt;

	//			if(f_alert_timer >= 5.f)
	//			{
	//				e_State = ATTACK;
	//				f_alert_timer = 0.f;
	//			}
	//		}
	//	}
	//	break;
}


	/*if(collisionChecking(Vector3(Position + Vector3(10.f, 10.f, 0.f))) || collisionChecking(Vector3(Position + Vector3(10.f, 50.f, 0.f))))
	{
	if(Velocity.x > 0)
	{
	Velocity.x = 0;
	}
	}

	if(collisionChecking(Vector3(Position + Vector3(0.f, 10.f, 10.f))) || collisionChecking(Vector3(Position + Vector3(0.f, 50.f, 10.f))))
	{
	if(Velocity.z > 0)
	{
	Velocity.z = 0;
	}
	}

	if(collisionChecking(Vector3(Position + Vector3(-10.f, 10.f, 0.f))) || collisionChecking(Vector3(Position + Vector3(-10.f, 50.f, 0.f))))
	{
	if(Velocity.x < 0)
	{
	Velocity.x = 0;
	}
	}

	if(collisionChecking(Vector3(Position + Vector3(0.f, 10.f, -10.f))) || collisionChecking(Vector3(Position + Vector3(0.f, 50.f, -10.f))))
	{
	if(Velocity.z < 0)
	{
	Velocity.z = 0;
	}
	}*/
	static float offset = 0.1f;
	if(collisionChecking(Vector3(Position + Vector3(10.f, 10.f, 0.f))) || collisionChecking(Vector3(Position + Vector3(10.f, 30.f, 0.f))))
	{
		if(Velocity.x > 0)
		{
			//if((Position - playerPos).LengthSquared() > 100)
				Position.x += offset;
			Velocity.x = 0;
		}
	}

	if(collisionChecking(Vector3(Position + Vector3(0.f, 10.f, 10.f))) || collisionChecking(Vector3(Position + Vector3(0.f, 30.f, 10.f))))
	{
		if(Velocity.z > 0)
		{
			//if((Position - playerPos).LengthSquared() > 100)
			Position.z += offset;
			Velocity.z = 0;
		}
	}

	if(collisionChecking(Vector3(Position + Vector3(-10.f, 10.f, 0.f))) || collisionChecking(Vector3(Position + Vector3(-10.f, 30.f, 0.f))))
	{
		if(Velocity.x < 0)
		{
			//if((Position - playerPos).LengthSquared() > 100)
			Position.x -= offset;
			Velocity.x = 0;
		}
	}

	if(collisionChecking(Vector3(Position + Vector3(0.f, 10.f, -10.f))) || collisionChecking(Vector3(Position + Vector3(0.f, 30.f, -10.f))))
	{
		if(Velocity.z < 0)
		{
			//if((Position - playerPos).LengthSquared() > 100)
			Position.z -= offset;
			Velocity.z = 0;
		}
	}

	/*
	//Collision handling

	if (collide(Vector3(P_Player.getPosition() + Vector3(-10.f, 10.f, 0.f))) || collide(Vector3(P_Player.getPosition() + Vector3(-10.f, 50.f, 0.f))))
	{
	if (P_Player.Velocity.x < 0)
	{
	P_Player.Velocity.x = 0;
	}
	}

	if (collide(Vector3(P_Player.getPosition() + Vector3(0.f, 10.f, -10.f))) || collide(Vector3(P_Player.getPosition() + Vector3(0.f, 50.f, -10.f))))
	{
	if (P_Player.Velocity.z < 0)
	{
	P_Player.Velocity.z = 0;
	}
	}*/

	if((Position - playerPos).LengthSquared() > 100)
		Position += Velocity * static_cast<float>(dt);
	else
		Velocity = 0;
}

bool AI::collisionChecking(Vector3 &pos)
{
	/*for(std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); it++)
	{
	GameObject* go = (GameObject*)*it;
	if(go->active && go->colEnable && go->pos != Position)
	{
	if(intersect(go->pos + go->ColBox, go->pos - go->ColBox, pos))
	{
	return true;
	}
	}
	}*/

	for(std::vector<CharacterObject*>::iterator it = m_charList.begin(); it != m_charList.end(); it++)
	{
		CharacterObject * CO = (CharacterObject*)*it;
		if(CO->getPosition() != Position)
		{
			static Vector3 test = Vector3(25, 25, 25);
			static Vector3 test2 = Vector3(15, 15, 15);
			if(intersect(CO->getPosition() + test,CO->getPosition() - test, Position))
			{
				return true;
			}
		}
	}
	return false;
}
