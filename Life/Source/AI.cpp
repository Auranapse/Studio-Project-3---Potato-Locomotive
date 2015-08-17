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
	alert(false),
	WOR(true),
	attack(false)
{

}

AI::AI(E_AI_STATE e_State, E_TYPE e_Type):
	alert(false),
	WOR(true),
	attack(false)
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
	if(isVisible(Position, Lookat, 65, playerPos))
	{
		e_State = ATTACK;
	}

	switch(e_State)
	{
	case ATTACK:
		{
			//Enemy would only move towards player if the difference between them is 15
			if((playerPos - Position).LengthSquared() > 225)
			{
				Vector3 diff = playerPos - Position;
				diff.Normalize();
				Position.x += diff.x * 10 * static_cast<float>(dt);
				Position.z += diff.z * 10 * static_cast<float>(dt);
			}

			//Change enemy state to walking if distance from player is more than 50
			if((playerPos - Position).LengthSquared() > 2500)
			{
				e_State = WALKING;
			}
		}
		break;

	case WALKING:
		{
			static bool patrollingLeft;

			//Ensure that the enemy would move to its default pos for now
			if(Position != defaultPosition)
			{
				Vector3 diff = defaultPosition - Position;
				diff.Normalize();
				Position.x += diff.x * 10 * static_cast<float>(dt);
				Position.z += diff.z * 10 * static_cast<float>(dt);
			}
		}
	}
	/*if(e_State == ATTACK)
		std::cout << "Hello World" << std::endl;
	else
		std::cout << "Bad World" << std::endl;*/
	/*if(attack == true)
	{
		e_AI = ATTACK;
	}
	else
	{
		e_AI = WALKING;
	}

	if(WOR == true)
	{
		e_AI = WALKING;
	}
	else
	{
		e_AI = RUN;
	}

	if(alert == true)
	{
		e_AI = ALERT;
	}
	else
	{
		e_AI = WALKING;
	}

	switch(e_AI)
	{
	case ALERT:
		{
		}
		break;

	case WALKING:
		{
		}
		break;

	case RUN:
		{
		}
		break;

	case ATTACK:
		{
		}
		break;
	}*/

	//Position += Velocity * static_cast<float>(dt);

}