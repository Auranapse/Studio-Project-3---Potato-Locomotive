/****************************************************************************/
/*!
\file AI.cpp
\author I'sa Hong Xi & Ong Swee Seng
\par email: dreamwalker56@gmail.com
			sweeseng123@gmail.com
\brief
Handles AI position and physics
!*/
/****************************************************************************/
#include "AI.h"
#include "Application.h"

double AI::d_detectionAngle = 30;
double AI::d_detectionRange = 6400 * 10;
double AI::d_detectionRangeMax = d_detectionRange * 2;
double AI::d_playerEscapeRange = 40000;

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
AI::AI() :
f_alert_timer(0.f),
	attackrate(0.f)
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
	positiveX = false, positiveZ = true, negativeX = false, negativeZ = false;
	diff.Set(0.f, 0.f, 1.f);
	b_aiScanning = false;
	b_aiRotating = false;
	b_SHOOTLA = false;
	b_isDestinationVisible = false;
	b_isDestinationWithinFOV = false;
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
		if (e_State == ATTACK)
		{
			f_movementSpeed = f_move_run;
		}
		else
		{
			f_movementSpeed = f_move_walk;
		}

		vel += (getDirection(true).Normalize() * (f_movementSpeed)) * static_cast<float>(dt);
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
void AI::SensorUpdate2(double &dt, bool left, bool mid, bool right)
{
	float rotationDiff = CalAnglefromPosition(destination, pos, true) - CalAnglefromPosition(Lookat, pos, true);
	movementLR(dt, false, rotationDiff * 0.5f);

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
		movementFB(dt, true);
	}

	//random betwee walking straight and left
	else if(left == false && mid == false && right == true)
	{
		
		movementFB(dt, true);
	}

	else if(left == false && mid == true && right == false)
	{
		rotationDiff = CalAnglefromPosition(destination, pos, true) - CalAnglefromPosition(Lookat, pos, true);
		movementLR(dt, false, rotationDiff);
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
		movementLR(dt, false, 360.f);
	}

	//random betwee walking straight and left
	else if(left == false && mid == false && right == true)
	{
		movementLR(dt, true, 360.f);
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
void AI::ai_ScanArea(double &dt)
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
Return the AI's LookAt destination

\return
currentLookAt
*/
/******************************************************************************/
Vector3 AI::getCurrentLookAt()
{
	return this->currentLookat;
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
void AI::setDestination(Vector3 &destination)
{
	currentLookat = destination;
	this->destination = destination;
}

/******************************************************************************/
/*!
\brief
Set the AI lookat

\param 
lookat
*/
/******************************************************************************/

void AI::setcurrentLookat(Vector3 &currentLookat)
{
	this->currentLookat = currentLookat;
}
/******************************************************************************/
/*!
\brief
Return the bool to determine whether the AI shud shoot or not

\param 
Shoot Or Not to shoot
*/
/******************************************************************************/
bool AI::getShootGun()
{
	return b_SHOOTLA;
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
void AI::aiStateHandling(double &dt, Vector3 &playerPos, std::vector<GameObject*> &m_GOList)
{
	Mtx44 rotation;
	rotation.SetToRotation(CalAnglefromPosition(Lookat, pos, true), 0.f, 1.f, 0.f);
	Vector3 L, R, C;
	C = rotation * Vector3(0.f, ModelPos.y, 50.f);
	L = rotation * Vector3(-15.f, ModelPos.y, 15.f);
	R = rotation * Vector3(15.f, ModelPos.y, 15.f);

	switch (e_State)
	{
	case WALKING:
		{
			//Have the AI partol a certain area
			//Need Pathfinding i think
			destination = playerPos;
			if (b_isDestinationWithinFOV && b_isDestinationVisible)
			{
				//If player is infront and near player, then ai will switch to attack state
				if ((playerPos - pos).LengthSquared() < d_detectionRange)
				{
					currentLookat.x = playerPos.x;
					currentLookat.z = playerPos.z;
					destination = playerPos;
					prevPosition = pos;
					e_State = ATTACK;
					break;
				}
				//if ai saw player but is too far way, the ai will investigate
				else if ((playerPos - pos).LengthSquared() >= d_detectionRange && (playerPos - pos).LengthSquared() <= d_detectionRangeMax)
				{
					destination.x = playerPos.x;
					destination.z = playerPos.z;
					destination = playerPos;
					prevPosition = pos;
					e_State = ALERT;
					break;
				}
			}

			SensorUpdate(dt, collisionChecking(pos + L, m_GOList), collisionChecking(pos + C, m_GOList), collisionChecking(pos + R, m_GOList));
		}
		break;

	case ALERT:
		{					
			//If destination is not visible
			if (!b_isDestinationVisible)
			{
				//use sensor update 2 to get to the destination
				SensorUpdate2(dt, collisionChecking(pos + L, m_GOList), collisionChecking(pos + C, m_GOList), collisionChecking(pos + R, m_GOList));
			}
			//If player is infront and near player, then ai will switch to attack state
			if (isVisible(pos, Lookat, getDetectionAngle(), playerPos) && (playerPos - pos).LengthSquared() < d_detectionRange)
			{
				currentLookat.x = playerPos.x;
				currentLookat.z = playerPos.z;
				destination = playerPos;
			}

			if (b_isDestinationVisible && b_isDestinationWithinFOV && destination == playerPos)
			{
				e_State = ATTACK;
			}
				
			if(b_isDestinationVisible && b_isDestinationWithinFOV)
			{
				moveToDestination(dt);
			}

			if((pos - destination).LengthSquared() < 300)
			{
				e_State = WALKING;
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
					//Make enemy stay at the same pos 
					if (b_isDestinationVisible && b_isDestinationWithinFOV && (playerPos - pos).LengthSquared() < d_detectionRange)
					{
						currentLookat.x = playerPos.x;
						currentLookat.z = playerPos.z;
						b_SHOOTLA = true;
					}
					else
					{
						b_SHOOTLA = false;
						if (!b_isDestinationVisible)
						{
							SensorUpdate2(dt, collisionChecking(pos + L, m_GOList), collisionChecking(pos + C, m_GOList), collisionChecking(pos + R, m_GOList));
						}
						else
						{
							moveToDestination(dt);
						}
					}
				}
				else
				{
					if (!b_isDestinationVisible)
					{
						SensorUpdate2(dt, collisionChecking(pos + L, m_GOList), collisionChecking(pos + C, m_GOList), collisionChecking(pos + R, m_GOList));
					}
					else
					{
						moveToDestination(dt);
					}
				}
			}
			else
			{
				if (!b_isDestinationVisible)
				{
					SensorUpdate2(dt, collisionChecking(pos + L, m_GOList), collisionChecking(pos + C, m_GOList), collisionChecking(pos + R, m_GOList));
				}
				else
				{
					moveToDestination(dt);
				}
			}
		}
		//Enemy is not holding a weapon
		else
		{
			if (!b_isDestinationVisible)
			{
				SensorUpdate2(dt, collisionChecking(pos + L, m_GOList), collisionChecking(pos + C, m_GOList), collisionChecking(pos + R, m_GOList));
			}
			else
			{
				moveToDestination(dt);
			}
		}
		break;
	}
	default:
		break;
	}
}

/******************************************************************************/
/*!
\brief
AI moves directly towards the destination
WARNING: Do not use if wall between AI and destination

\param dt
delta time
*/
/******************************************************************************/
void AI::moveToDestination(double &dt)
{
	currentLookat = destination;
	movementFB(dt, true);
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
void AI::AiLookatRotation(double &dt)
{
	if(currentLookat != 0)
	{
		static float rotationSpeed = 4;

		if(e_State == ATTACK)
		{
			rotationSpeed = 10;
		}
		else
		{
			rotationSpeed = 5;
		}

		if (b_aiRotating == false)
		{
			float rotationdiff = (CalAnglefromPosition(currentLookat, pos, true) - CalAnglefromPosition(Lookat, pos, true));
			
			if (rotationdiff >= 180)
			{
				Mtx44 rotation;
				Lookat = Lookat - pos;
				rotation.SetToRotation(-360, 0, 1, 0);
				Lookat = rotation * Lookat;
				Lookat = Lookat + pos;
				rotationdiff -= 360;
			}
			else if (rotationdiff <= -180)
			{
				Mtx44 rotation;
				Lookat = Lookat - pos;
				rotation.SetToRotation(360, 0, 1, 0);
				Lookat = rotation * Lookat;
				Lookat = Lookat + pos;
				rotationdiff += 360;
			}

			Mtx44 rotation;
			Lookat = Lookat - pos;
			rotation.SetToRotation(rotationdiff * rotationSpeed * static_cast<float>(dt), 0, 1, 0);
			Lookat = rotation * Lookat;
			Lookat = Lookat + pos;

			if(rotationdiff < 0.1 && rotationdiff > -0.1)
			{
				b_aiRotating = true;
			}
		}
		else
		{
			if (e_State == ATTACK)
			{
				prevPosition = pos;
			}
			else if (e_State == ALERT)
			{
				destination = currentLookat;
				prevPosition = pos;
			}

			currentLookat = 0;
			b_aiRotating = false;
		}
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
void AI::Update(double &dt, Vector3 &playerPos, std::vector<GameObject*> &m_GOList)
{
	aiStateHandling(dt, playerPos, m_GOList);
	if(currentLookat != 0 || e_State == ATTACK)
	{
		AiLookatRotation(dt);
	}

	collisionHandler(m_GOList);

	if (vel.x != 0)
	{
		float SForceX = 0 - vel.x;
		vel.x += SForceX * static_cast<float>(dt) * 4.f;
	}

	if (vel.z != 0)
	{
		float SForceZ = 0 - vel.z;
		vel.z += SForceZ * static_cast<float>(dt) * 4.f;
	}


	Animation.Update(dt, vel.LengthSquared() * 4);
	Lookat += vel * 10 * static_cast<float>(dt);
	pos += vel * 10 * static_cast<float>(dt);
}

/******************************************************************************/
/*!
\brief
Prevent AI from clipping into walls completely
*/
/******************************************************************************/
void AI::collisionHandler(std::vector<GameObject *> &m_GOList)
{
	if(collisionChecking(pos + Vector3(collisionMesh.ColBox.x, ModelPos.y, 0), m_GOList))
	{
		if(vel.x > 0)
		{
			vel.x = 0;
		}
	}

	if(collisionChecking(pos + Vector3(-collisionMesh.ColBox.x, ModelPos.y, 0), m_GOList))
	{
		if(vel.x < 0)
		{
			vel.x = 0;
		}
	}

	if(collisionChecking(pos + Vector3(0, ModelPos.y, collisionMesh.ColBox.z), m_GOList))
	{
		if(vel.z > 0)
		{
			vel.z = 0;
		}
	}

	if(collisionChecking(pos + Vector3(0, ModelPos.y, -collisionMesh.ColBox.z), m_GOList))
	{
		if(vel.z < 0)
		{
			vel.z = 0;
		}
	}
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
			if (intersect(go->pos + go->collisionMesh.ColBox, go->pos - go->collisionMesh.ColBox, pos))
			{
				return true;
			}
		}
	}
	return false;
}