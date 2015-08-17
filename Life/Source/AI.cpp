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
void AI::Update(double &dt)
{
	
	if(attack == true)
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
	}

	Position += Velocity * static_cast<float>(dt);

}