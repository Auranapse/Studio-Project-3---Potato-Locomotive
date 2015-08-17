/****************************************************************************/
/*!
\file AI.h
\author I'sa Hong Xi & Ong Swee Seng
\par email: dreamwalker56@gmail.com
\brief
Handles AI behavior and update
!*/
/****************************************************************************/

#pragma once
#include "AI.h"
class Strategy :
	public AI
{
public:

	enum E_AI_TYPE
	{
		AI_SECURITY,
		AI_SCIENTIST,
	};

	Strategy(void);
	Strategy(E_AI_TYPE e_type);
	void Update(double dt);
	virtual ~Strategy(void);

private:

	E_AI_TYPE e_type;
};
