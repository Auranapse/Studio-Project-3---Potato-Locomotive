/****************************************************************************/
/*!
\file Player.h
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Handles player position and physics
!*/
/****************************************************************************/
#ifndef PLAYER_H
#define PLAYER_H

#include "CharacterObject.h"

class Player : public CharacterObject
{
private:

public:
	Player();
	virtual ~Player();

	void movementFB(double &dt, bool forward = true);
	void movementLR(double &dt, bool left = true);

	virtual void Update(double &dt);
};

#endif