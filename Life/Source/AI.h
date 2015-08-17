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


public:
	AI();
	virtual ~AI();
	
	enum E_AI_STATE
	{
		WALKING,
		ALERT,
		ATTACK,
		RUN,
	};

	void movementRW(bool run = true);
	void movementFB(double &dt, bool forward = true);
	void movementLR(double &dt, bool left = true);

	virtual void Update(double &dt);

private:
	//-------------------------------------Variables------------------------------------------------
	E_AI_STATE e_AI;
	bool alert;
	//Walk Or Run
	bool WOR;
	bool attack;
};

#endif