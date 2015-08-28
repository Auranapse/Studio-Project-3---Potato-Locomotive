/****************************************************************************/
/*!
\file Player.cpp
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Handles player position and physics
!*/
/****************************************************************************/
#include "Player.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Player::Player()
{
	CamOffset.Set(0, 25, 0);
}

/******************************************************************************/
/*!
\brief
Default destructor
*/
/******************************************************************************/
Player::~Player()
{

}

/******************************************************************************/
/*!
\brief
makes player run/walk
\param forward
if true will run, else walk
*/
/******************************************************************************/
void Player::movementRW(bool run)
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
moves player forward/back
\param forward
if true will move forward, else back
*/
/******************************************************************************/
void Player::movementFB(double &dt, bool forward)
{
	if (forward)
	{
		vel += (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);
	}
	else
	{
		vel -= (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);
	}
}

/******************************************************************************/
/*!
\brief
moves player left/right
\param left
if true will move left, else right
*/
/******************************************************************************/
void Player::movementLR(double &dt, bool left)
{
	
}

/******************************************************************************/
/*!
\brief
Updates the player
\param dt
delta time
*/
/******************************************************************************/
void Player::Update(double &dt)
{
	Animation.Update(dt, Vector3(vel.x, 0, vel.z).LengthSquared()*0.03f);
	pos += vel * static_cast<float>(dt);
}