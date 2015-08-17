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
class Strategy
{
public:

	enum E_AI_TYPE
	{
		AI_SECURITY,
		AI_SCIENTIST,
	};

	Strategy(void);
	void Update(double dt);
	virtual ~Strategy(void);

	void setState(E_AI_TYPE state);

private:
	E_AI_TYPE e_type;
};

