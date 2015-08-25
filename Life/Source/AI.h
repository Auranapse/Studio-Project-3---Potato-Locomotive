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

#include "Application.h"
#include "CharacterObject.h"
#include "ItemObject.h"
#include "WeaponsObject.h"
#include "GabrielDLC.h"
#include "Collision.h"
#include "Mtx44.h"

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

	void SensorUpdate(double &dt, bool left = false, bool middle = false, bool right = false);
	virtual void Update(double &dt, Vector3 playerPos, std::vector<CharacterObject *> &m_charList, std::vector<GameObject *> &m_GOList);
	bool collisionChecking(Vector3 &Position, std::vector<CharacterObject *> &m_charList, std::vector<GameObject *> &m_GOList, bool checkGO = true);

	bool movingByPositive_x();
	bool movingByPositive_z();
	bool movingByNegative_x();
	bool movingByNegative_z();

	void setPositive_x();
	void setPositive_z();
	void setNegative_x();
	void setNegative_z();

	void rotateAi_Clockwise();
	void rotateAI_CounterClockWise();
	void rotateAI_180();

	void ai_ScanArea(const double &dt);
private:
	//-------------------------------------Variables------------------------------------------------
	E_AI_STATE e_State;
	E_TYPE e_Type;
	Mtx44 rotating;
	float f_alert_timer;
	float f_cooldownTime;
	bool b_aiCooldown;
	bool collided;

	Vector3 prevPosition;
	Vector3 destination;
	bool b_updateAI;
	double d_totalRotation;
	double d_enemyRotation;
	bool b_rotateClockwiseFirst;
	bool positiveX, positiveZ, negativeX, negativeZ;
	Vector3 diff;
};

#endif