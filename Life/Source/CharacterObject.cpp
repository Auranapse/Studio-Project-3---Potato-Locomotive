/****************************************************************************/
/*!
\file CharacterObject.cpp
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Character Object used by AI and player
!*/
/****************************************************************************/
#include "CharacterObject.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
CharacterObject::CharacterObject(void) : Position(0.f, 0.f, 0.f), Lookat(0.f, 0.f, 0.f), Velocity(0.f, 0.f, 0.f), Scale(1.f, 1.f, 1.f), Head(NULL), Chest(NULL), Arms(NULL), Legs(NULL), Animation(0, 0, 0, 0), f_move_crawl(10.f), f_move_walk(20.f), f_move_run(35.f), f_movementSpeed(f_move_walk)
{

}

/******************************************************************************/
/*!
\brief
Default destructor
*/
/******************************************************************************/
CharacterObject::~CharacterObject(void)
{
	if(Head != NULL)
	{
		delete Head;
	}

	if(Chest != NULL)
	{
		delete Chest;
	}

	if(Arms != NULL)
	{
		delete Arms;
	}

	if(Legs != NULL)
	{
		delete Legs;
	}
}

/******************************************************************************/
/*!
\brief
Gets the current animation
\return
current animation
*/
/******************************************************************************/
Anim4 CharacterObject::getAnimation(void)
{
	return Animation;
}

/******************************************************************************/
/*!
\brief
Sets a new position for the character
\param &newpos
the new position to set to

I recommend not touching position, go touch velocity instead
*/
/******************************************************************************/
void CharacterObject::setPosition(Vector3 &newpos)
{
	Position = newpos;
}

/******************************************************************************/
/*!
\brief
Gets the current position of the character
\return
current position
*/
/******************************************************************************/
Vector3 CharacterObject::getPosition(void)
{
	return Position;
}

/******************************************************************************/
/*!
\brief
Gets the current direction character is looking at
\return
direction vector
*/
/******************************************************************************/
Vector3 CharacterObject::getDirection(void)
{
	return (Lookat - Position);
}

/******************************************************************************/
/*!
\brief
Updates the charater
\param dt
delta time
*/
/******************************************************************************/
void CharacterObject::Update(double &dt)
{
	Position += Velocity * static_cast<float>(dt);
}