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

	switch(e_State)
	{
	case ATTACK:
		{
			std::cout << "Attack" << std::endl;
			this->Lookat = playerPos;
			//Enemy would only move towards player if the difference between them is 15
			if((playerPos - Position).LengthSquared() > 225)
			{
				Vector3 diff = playerPos - Position;
				diff.Normalize();
				Position.x += diff.x * f_movementSpeed * static_cast<float>(dt);
				Position.z += diff.z * f_movementSpeed * static_cast<float>(dt);
			}

			//Change enemy state to walking if distance from player is more than 50
			if((playerPos - Position).LengthSquared() > 10000)
			{
				e_State = WALKING;
			}
		}
		break;

	case WALKING:
		{
			if(isVisible(Position, Lookat, 65, playerPos) && (Position - playerPos).LengthSquared() < 10000)
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

				Position.x += diff.x * f_movementSpeed * static_cast<float>(dt);
				Position.z += diff.z * f_movementSpeed * static_cast<float>(dt);

				if(Collision::Length(Position, Vector3(0, 0, 0), 2, false))
				{
					Position.SetZero();
				}

			}

			else
				Lookat = defaultLookat;
		}
		break;

	case ALERT:
		{
			Lookat = destination;

			//Make the enemy move towards the destination
			if(Position.x != destination.x && Position.z != destination.z)
			{
				Vector3 diff = destination - Position;
				diff.Normalize();

				Position.x += diff.x * f_movementSpeed * static_cast<float>(dt);
				Position.z += diff.z * f_movementSpeed * static_cast<float>(dt);

				if((Position - destination).LengthSquared() < 30)
				{
					Position.x = destination.x;
					Position.z = destination.z;
				}
			}
			//If the enemy is at the destination
			else
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

	//Position += Velocity * static_cast<float>(dt);

}