/****************************************************************************/
/*!
\file AI.h
\author I'sa Hong Xi
\par email: dreamwalker56@gmail.com
\brief
Handles AI position and physics
!*/
/****************************************************************************/
#ifndef AI_H
#define AI_H

#include "CharacterObject.h"

class AI : public CharacterObject
{
private:

public:
	AI();
	virtual ~AI();
	
	void movementRW(bool run = true);
	void movementFB(double &dt, bool forward = true);
	void movementLR(double &dt, bool left = true);

	virtual void Update(double &dt);
};

#endif