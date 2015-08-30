/****************************************************************************/
/*!
\file AI.cpp
\author I'sa Hong Xi
\par email: dreamwalker56@gmail.com
\brief
Handles AI position and physics
!*/
/****************************************************************************/
#include "AI.h"
#include "Application.h"

double AI::d_detectionAngle = 30;
double AI::d_detectionRange = 6400;
double AI::d_detectionRangeMax = 12800;
double AI::d_playerEscapeRange = 40000;

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
AI::AI() :
f_alert_timer(0.f)
{

}

AI::AI(E_AI_STATE e_State, E_TYPE e_Type) :
f_alert_timer(0.f)
{
	this->e_State = e_State;
	this->e_Type = e_Type;
	rotating.SetToRotation(90.f, 0, 1, 0);
	collided = false;
	prevPosition.SetZero();
	destination.SetZero();
	b_updateAI = true;
	d_totalRotation = 0.0;
	d_enemyRotation = 0.0;
	b_rotateClockwiseFirst = NULL;
	currentLookat = NULL;
	b_goAlert = b_goAttack = false;
	positiveX = false, positiveZ = true, negativeX = false, negativeZ = false;
	diff.Set(0.f, 0.f, 1.f);
	b_aiScanning = false;
}

/******************************************************************************/
/*!
\brief
Default destructor
*/
/******************************************************************************/
AI::~AI()
{

}

/******************************************************************************/
/*!
\brief
makes AI run/walk
\param forward
if true will run, else walk
*/
/******************************************************************************/
void AI::movementRW(bool run)
{
	if (run)
	{
		f_movementSpeed = f_move_run;
	}
	else
	{
		f_movementSpeed = f_move_walk;
	}
}

/******************************************************************************/
/*!
\brief
moves AI forward/back
\param forward
if true will move forward, else back
*/
/******************************************************************************/
void AI::movementFB(double &dt, bool forward)
{
	Mtx44 rotation;

	if (forward)
	{
		vel += (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);
	}
	else
	{
		Lookat = Lookat - pos;
		rotation.SetToRotation(720 * static_cast<float>(dt), 0, 1, 0);
		Lookat = rotation * Lookat;
		Lookat = Lookat + pos;
		//Velocity += (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);
	}
}

/******************************************************************************/
/*!
\brief
moves AI left/right
\param left
if true will move left, else right
*/
/******************************************************************************/
void AI::movementLR(double &dt, bool left, float rotation_speed)
{
	Mtx44 rotation;
	if (left == true)
	{
		Lookat = Lookat - pos;
		rotation.SetToRotation(-rotation_speed * static_cast<float>(dt), 0, 1, 0);
		Lookat = rotation * Lookat;
		Lookat = Lookat + pos;
		//Velocity += (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);

	}

	else
	{
		Lookat = Lookat - pos;
		rotation.SetToRotation(rotation_speed * static_cast<float>(dt), 0, 1, 0);
		Lookat = rotation * Lookat;
		Lookat = Lookat + pos;
		//Velocity += (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);
	}
}

/******************************************************************************/
/*!
\brief
Update the sensors for pathfinding to player
\param left
sensors made to see if there is anything in the way of the AI and also move towards the player
*/
/******************************************************************************/
void AI::SensorUpdate2(double &dt, Vector3 &destination, bool left, bool mid, bool right)
{
	float rotationDiff = CalAnglefromPosition(destination, pos, true) - CalAnglefromPosition(Lookat, pos, true);
	movementLR(dt, false, rotationDiff);

	if(mid || left || right)
	{
		destination = pos;
	}

	//when right has nothing to collide
	if (left == true && mid == true && right == false)
	{
		movementLR(dt, false, 720.f);
	}

	//when left has nothing to collide
	else if (left == false && mid == true && right == true)
	{
		movementLR(dt, true, 720.f);
	}

	//when middle has nothing to collide
	else if (left == true && mid == false && right == true)
	{
		movementFB(dt, true);
	}

	//if none of the sensors are colliding... just move forward
	else if (left == false && mid == false && right == false)
	{
		movementFB(dt, true);
	}

	//set rand inside to do a 50 - 50 chance to go left or right
	else if (left == true && mid == true && right == true)
	{
		movementFB(dt, false);
	}

	//random betwee walking straight and right
	else if(left == true && mid == false && right == false)
	{
		int dothis = Math::RandIntMinMax(1, 2);
		if(dothis == 1)
			movementFB(dt, true);

		else
			movementLR(dt, false, 720.f);
	}

	//random betwee walking straight and left
	else if(left == false && mid == false && right == true)
	{
		int dothis = Math::RandIntMinMax(1, 2);
		if(dothis == 1)
			movementFB(dt, true);

		else
		{
			movementFB(dt, false);
			movementLR(dt, true, 720.f);
		}
	}

	else if(left == false && mid == true && right == false)
	{
		int dothis = Math::RandIntMinMax(1, 2);
		if(dothis == 1)
			movementLR(dt, true, 720.f);

		else
			movementLR(dt, false, 720.f);
	}
	else
	{
		movementFB(dt, true);
	}
}

/******************************************************************************/
/*!
\brief
Update the sensors for pathfinding
\param left
sensors made to see if there is anything in the way of the AI
*/
/******************************************************************************/
void AI::SensorUpdate(double &dt, bool left, bool mid, bool right)
{
	//when right has nothing to collide
	if (left == true && mid == true && right == false)
	{
		movementLR(dt, false, 720.f);
	}

	//when left has nothing to collide
	else if (left == false && mid == true && right == true)
	{
		movementLR(dt, true, 720.f);
	}

	//when middle has nothing to collide
	else if (left == true && mid == false && right == true)
	{
		movementFB(dt, true);
	}

	//if none of the sensors are colliding... just move forward
	else if (left == false && mid == false && right == false)
	{
		movementFB(dt, true);
	}

	//set rand inside to do a 50 - 50 chance to go left or right
	else if (left == true && mid == true && right == true)
	{
		movementFB(dt, false);
	}

	//random betwee walking straight and right
	else if(left == true && mid == false && right == false)
	{
		int dothis = Math::RandIntMinMax(1, 2);
		if(dothis == 1)
			movementFB(dt, true);

		else
			movementLR(dt, false, 720.f);
	}

	//random betwee walking straight and left
	else if(left == false && mid == false && right == true)
	{
		int dothis = Math::RandIntMinMax(1, 2);
		if(dothis == 1)
			movementFB(dt, true);

		else
		{
			movementFB(dt, false);
			movementLR(dt, true, 720.f);
		}
	}

	else if(left == false && mid == true && right == false)
	{
		int dothis = Math::RandIntMinMax(1, 2);
		if(dothis == 1)
			movementLR(dt, true, 720.f);

		else
			movementLR(dt, false, 720.f);
	}
	else
	{
		movementFB(dt, true);
	}
}


/******************************************************************************/
/*!
\brief
AI will scan the area for the player
\param dt
use to update the rotation of the ai
*/
/******************************************************************************/
void AI::ai_ScanArea(const double &dt)
{
	b_updateAI = false;
	b_aiScanning = true;

	static double rotationSpeed = 50.f;

	if(b_rotateClockwiseFirst == NULL)
	{
		b_rotateClockwiseFirst = static_cast<bool>(Math::RandIntMinMax(0, 1));
	}

	d_totalRotation += rotationSpeed * dt;

	//Scan 
	if(d_totalRotation < 90)
	{
		if(b_rotateClockwiseFirst)
			d_enemyRotation = -rotationSpeed * dt;
		else
			d_enemyRotation = rotationSpeed * dt;
	}
	else if (d_totalRotation >= 90 && d_totalRotation < 270)
	{
		if(b_rotateClockwiseFirst)
			d_enemyRotation = rotationSpeed * dt;
		else
			d_enemyRotation = -rotationSpeed * dt;
	}
	else
	{
		e_State = WALKING;
		d_totalRotation = 0.f;
		b_updateAI = true;
		b_rotateClockwiseFirst = NULL;
		b_aiScanning = false;
	}

	Mtx44 rotation;
	Lookat = Lookat - pos;
	rotation.SetToRotation(static_cast<float>(d_enemyRotation), 0, 1, 0);
	Lookat = rotation * Lookat;
	Lookat = Lookat + pos;
}

/******************************************************************************/
/*!
\brief
Return the player escape range

\param 
		d_playerEscapeRange

\return 
		d_playerEscapeRange
*/
/******************************************************************************/
double AI::getPlayerEscapeRange()
{
	return d_playerEscapeRange;
}

/******************************************************************************/
/*!
\brief
Return the AI detection angle range

\param 
		d_detectionAngle

\return 
		d_detectionAngle
*/
/******************************************************************************/
double AI::getDetectionAngle()
{
	return d_detectionAngle;
}

/******************************************************************************/
/*!
\brief
Return the AI detection range

\param 
		d_detectionRange

\return 
		d_detectionRange
*/
/******************************************************************************/
double AI::getDetectionRange()
{
	return d_detectionRange;
}

/******************************************************************************/
/*!
\brief
Return the AI detection maximum range

\param 
		d_detectionRangeMax

\return 
		d_detectionRangeMax
*/
/******************************************************************************/
double AI::getDetectionRange_Max()
{
	return d_detectionRangeMax;
}


/******************************************************************************/
/*!
\brief
Return the AI destination

\param 
		destination

\return 
		destination
*/
/******************************************************************************/
Vector3 AI::getDestination()
{
	return destination;
}

/******************************************************************************/
/*!
\brief
Return the AI state

\param 
		e_State

\return 
		e_State
*/
/******************************************************************************/
AI::E_AI_STATE AI::getState()
{
	return e_State;
}


/******************************************************************************/
/*!
\brief
Set the AI state

\param 
		e_State
*/
/******************************************************************************/
void AI::setState(E_AI_STATE e_State)
{
	this->e_State = e_State;
}

/******************************************************************************/
/*!
\brief
Set the AI destination

\param 
		destination
*/
/******************************************************************************/
void AI::setDestination(const Vector3 &destination)
{
	this->destination = destination;
}

/******************************************************************************/
/*!
\brief
Update the AI Lookat based on its current Lookat

\param 
		delta time
		Player Position
		AI Lookat
		AI Current Lookat
*/
/******************************************************************************/
void AI::UpdateLookat(const double &dt, const Vector3 &playerPos)
{
	if(currentLookat != NULL)
	{
		b_updateAI = false;

		if(Lookat != currentLookat)
		{
			float theta = Math::RadianToDegree(acos((Lookat.Dot(currentLookat)) / (Lookat.Length() * currentLookat.Length())));

			//if(theta >= 5)
			{
				if(Lookat.x > currentLookat.x)
					Lookat.x -= 50 * static_cast<float>(dt);
				else
					Lookat.x += 50 * static_cast<float>(dt);

				if(Lookat.z > currentLookat.z)
					Lookat.z -= 50 * static_cast<float>(dt);
				else
					Lookat.z += 50 * static_cast<float>(dt);
			}
			if (theta < 1)
			{
				Lookat = currentLookat;
			}
		}
		else
		{
			//if(e_State == WALKING)
			//{
			//	if ((playerPos - Position).LengthSquared() < d_detectionRange)
			//	{
			//		prevPosition = Position;
			//		e_State = ATTACK;
			//		b_aiCooldown = false;
			//	}
			//	//if ai saw player but is too far way, the ai will investigate
			//	else if ((playerPos - Position).LengthSquared() > d_detectionRange && (playerPos - Position).LengthSquared() < d_detectionRangeMax) 
			//	{
			//		destination = currentLookat;
			//		prevPosition = Position;
			//		e_State = ALERT;
			//		b_aiCooldown = false;

			//	}
			//}

			if(b_goAttack)
			{
				prevPosition = pos;
				e_State = ATTACK;
				b_goAttack = false;
			}
			else if (b_goAlert)
			{
				destination = currentLookat;
				prevPosition = pos;
				e_State = ALERT;
				b_goAlert = false;
			}
			
			b_updateAI = true;
			currentLookat = NULL;
		}
	}
}

/******************************************************************************/
/*!
\brief
Update AI base on its state

\param 
		delta time
		Player Position
		Enemy state
*/
/******************************************************************************/
void AI::aiStateHandling(const double &dt, const Vector3 &playerPos)
{
	switch (e_State)
	{
	case WALKING:
	{
		//Have the AI partol a certain area
		//Need Pathfinding i think

		if (isVisible(pos, Lookat, static_cast<float>(d_detectionAngle), playerPos))
		{
			//If player is infront and near player, then ai will switch to attack state
			if ((playerPos - pos).LengthSquared() < d_detectionRange)
			{
				currentLookat.x = playerPos.x;
				currentLookat.z = playerPos.z;
				prevPosition = pos;
				b_goAttack = true;
			}
			//if ai saw player but is too far way, the ai will investigate
			else if ((playerPos - pos).LengthSquared() >= d_detectionRange && (playerPos - pos).LengthSquared() <= d_detectionRangeMax)
			{
				destination.x = playerPos.x;
				destination.z = playerPos.z;
				currentLookat.x = playerPos.x;
				currentLookat.z = playerPos.z;
				prevPosition = pos;
				b_goAlert = true;
			}
		}
	}
	break;

	case ALERT:
	{
		//AI will move towards the destination
		if ((pos - destination).LengthSquared() > 2)
		{
			//Move the ai towards the destination
			Lookat = destination;
		}
		//if ai is at the destination
		else
		{
			ai_ScanArea(dt);
		} 

		//If player is infront and near player, then ai will switch to attack state
		if (isVisible(pos, Lookat, static_cast<float>(d_detectionAngle), playerPos) && (playerPos - pos).LengthSquared() < d_detectionRange)
		{
			e_State = ATTACK;
			b_updateAI = true;
			b_rotateClockwiseFirst = NULL;
		}
	}
	break;

	case ATTACK:
	{
		destination = playerPos;

		//if enemy is holding a weapon
		if (holding != NULL)
		{
			if (holding->isWeapon)
			{
				WeaponsObject *WO = dynamic_cast<WeaponsObject*>(holding);

				//Enemy is holding a gun
				if (WO->isGun)
				{
					//Make enemy move a certain distance away from the enemy before shooting
				}
				//Enemy is holding a melee weapon
				else
				{

				}
			}
		}
		//Enemy is not holding a weapon
		else
		{
			//make the enemy move closer to the enemy before attacking
		}

		//AI return to alert state if player have avoided enemy
		/*if ((Position - playerPos).LengthSquared() > d_playerEscapeRange)
		{
		b_aiCooldown = true;
		e_State = ALERT;
		}*/
	}
	break;

	default:
		break;
	}
}

void AI::AiLookatRotation(const double &dt, const Vector3 &playerPos)
{
	static float rotationSpeed = 50;
	static bool test = false;
	if (test == false)
	{
		if (Lookat.x < currentLookat.x)
		{
			Lookat.x += rotationSpeed * dt;
		}
		else if (Lookat.x > currentLookat.x)
		{
			Lookat.x -= rotationSpeed * dt;
		}

		if (Lookat.z < currentLookat.z)
		{
			Lookat.z += rotationSpeed * dt;
		}
		else if (Lookat.z > currentLookat.z)
		{
			Lookat.z -= rotationSpeed * dt;
		}

		if ((Lookat - currentLookat).LengthSquared() < 26)
		{
			test = true;
		}
	}
	else
	{
		if (b_goAttack)
		{
			prevPosition = pos;
			e_State = ATTACK;
			b_goAttack = false;
		}
		else if (b_goAlert)
		{
			destination = currentLookat;
			prevPosition = pos;
			e_State = ALERT;
			b_goAlert = false;
		}

		Lookat = currentLookat;
		currentLookat = NULL;
		test = false;
		currentLookat = NULL;
	}
}

/******************************************************************************/
/*!
\brief
Updates the AI
\param dt
delta time
CharacterObject vector list - To check collision
GameObject vector list - To check collision
*/
/******************************************************************************/
void AI::Update(double &dt, const Vector3 &playerPos, std::vector<GameObject*> &m_GOList)
{
	if (currentLookat == NULL)
	{
		aiStateHandling(dt, playerPos);

		Mtx44 rotation;
		rotation.SetToRotation(CalAnglefromPosition(Lookat, pos, true), 0.f, 1.f, 0.f);
		Vector3 L, R, C;
		C = rotation * Vector3(0.f, ModelPos.y, 50.f);
		L = rotation * Vector3(-15.f, ModelPos.y, 15.f);
		R = rotation * Vector3(15.f, ModelPos.y, 15.f);

		if (b_aiScanning == false)
		{
			if (e_State == WALKING)
			{
				SensorUpdate(dt, collisionChecking(pos + L, m_GOList), collisionChecking(pos + C, m_GOList), collisionChecking(pos + R, m_GOList));
			}
			else
			{
				SensorUpdate2(dt, destination, collisionChecking(pos + L, m_GOList), collisionChecking(pos + C, m_GOList), collisionChecking(pos + R, m_GOList));
			}
		}
		if (vel.x != 0)
		{
			float SForceX = 0 - vel.x;
			vel.x += SForceX * 0.1f;
		}

		if (vel.z != 0)
		{
			float SForceZ = 0 - vel.z;
			vel.z += SForceZ * 0.1f;
		}

		if (b_updateAI)
		{
			Animation.Update(dt, vel.LengthSquared() * 4);
			Lookat += vel * 10 * static_cast<float>(dt);
			pos += vel * 10 * static_cast<float>(dt);
		}
	}
	else
		AiLookatRotation(dt, playerPos);
}

/******************************************************************************/
/*!
\brief
To detect collision
\param dt
Vector3 pos
GameObject vector list - To check collision
*/
/******************************************************************************/
bool AI::collisionChecking(Vector3 &pos, std::vector<GameObject *> &m_GOList)
{
	for (std::vector<GameObject*>::iterator it = m_GOList.begin(); it != m_GOList.end(); it++)
	{
		GameObject* go = (GameObject*)*it;
		if (go->active && go->colEnable && go->pos != pos)
		{
			if (intersect(go->pos + go->ColBox, go->pos - go->ColBox, pos))
			{
				return true;
			}
		}
	}
	return false;
}