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
	CamOffset.Set(0, 22, 0);
	//f_move_crawl(10.f), f_move_walk(20.f), f_move_run(40.f), f_movementSpeed(f_move_walk)
	f_move_crawl = 150.f;
	f_move_walk = 700.f;
	f_move_run = 1200.f;
	f_movementSpeed = f_move_walk;
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
	if (left)
	{
		vel -= (getDirection(true).Cross(Vector3(0, 1, 0)).Normalize() * f_movementSpeed) * static_cast<float>(dt);
	}
	else
	{
		vel += (getDirection(true).Cross(Vector3(0, 1, 0)).Normalize() * f_movementSpeed) * static_cast<float>(dt);
	}
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
	Animation.Update(dt*0.5f, Vector3(vel.x, 0, vel.z).LengthSquared()*0.03f);
	pos += vel * static_cast<float>(dt);
}