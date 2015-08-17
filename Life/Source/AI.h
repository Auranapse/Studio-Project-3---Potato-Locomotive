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
#include "GabrielDLC.h"

class AI : public CharacterObject
{


public:
	enum E_AI_STATE
	{
		WALKING,
		ALERT,
		ATTACK,
		RUN,
	};

	enum E_TYPE
	{
		AI_SECURITY,
		AI_SCIENTIST,
	};

	AI();
	AI(E_AI_STATE, E_TYPE);
	virtual ~AI();

	void movementRW(bool run = true);
	void movementFB(double &dt, bool forward = true);
	void movementLR(double &dt, bool left = true);

	virtual void Update(double &dt, Vector3 playerPos);

private:
	//-------------------------------------Variables------------------------------------------------
	E_AI_STATE e_State;
	E_TYPE e_Type;

	bool alert;
	//Walk Or Run
	bool WOR;
	bool attack;
};

#endif