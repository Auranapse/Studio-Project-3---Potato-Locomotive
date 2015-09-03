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

	bool b_isDestinationVisible;
	bool b_isDestinationWithinFOV;
	void movementRW(bool run = true);
	void movementFB(double &dt, bool forward = true);
	void movementLR(double &dt, bool left = true, float rotation_speed = 90.f);

	void SensorUpdate(double &dt, bool left = false, bool middle = false, bool right = false);
	void SensorUpdate2(double &dt, bool left = false, bool middle = false, bool right = false);
	virtual void Update(double &dt, Vector3 &playerPos, std::vector<GameObject *> &m_GOList);
	bool collisionChecking(Vector3 &Position, std::vector<GameObject *> &m_GOList);
	void collisionHandler(std::vector<GameObject *> &m_GOList);

	void ai_ScanArea(double &dt);
	void aiStateHandling(double &dt, Vector3 &playerPos, std::vector<GameObject*> &m_GOList);
	void AiLookatRotation(double &dt);
	void moveToDestination(double &dt);
	double getPlayerEscapeRange();
	double getDetectionAngle();
	double getDetectionRange();
	double getDetectionRange_Max();
	bool getShootGun();
	Vector3 getDestination();
	Vector3 getCurrentLookAt();
	E_AI_STATE getState();
	void setState(E_AI_STATE e_State);
	void setDestination(Vector3 &destination);
	void setcurrentLookat(Vector3 &currentLookat);
	float attackrate;
private:
	//-------------------------------------Variables------------------------------------------------
	E_AI_STATE e_State;
	E_TYPE e_Type;
	Mtx44 rotating;
	float f_alert_timer;
	bool collided;

	Vector3 prevPosition;
	Vector3 destination;
	Vector3 currentLookat;
	bool b_updateAI;
	bool b_SHOOTLA;
	double d_totalRotation;
	double d_enemyRotation;
	bool b_rotateClockwiseFirst;

	static double d_detectionAngle;
	static double d_detectionRange;
	static double d_detectionRangeMax;
	static double d_playerEscapeRange;
	bool b_aiScanning;
	Vector3 test;

	bool b_aiRotating;
	bool positiveX, positiveZ, negativeX, negativeZ;
	Vector3 diff;
};

#endif