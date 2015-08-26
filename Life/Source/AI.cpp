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
double AI::d_detectionRangeMax = d_detectionRange * 2;
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
	Velocity = Vector3(0, 0, 10);
	collided = false;
	prevPosition.SetZero();
	destination.SetZero();
	b_aiCooldown = false;
	f_cooldownTime = 0.f;
	b_updateAI = true;
	d_totalRotation = 0.0;
	d_enemyRotation = 0.0;
	b_rotateClockwiseFirst = NULL;
	currentLookat = NULL;
	positiveX = false, positiveZ = true, negativeX = false, negativeZ = false;
	diff.Set(0.f, 0.f, 1.f);
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
		Velocity += (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);
	}
	else
	{
		Lookat = Lookat - Position;
		rotation.SetToRotation(720 * dt, 0, 1, 0);
		Lookat = rotation * Lookat;
		Lookat = Lookat + Position;
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
		Lookat = Lookat - Position;
		rotation.SetToRotation(-rotation_speed * dt, 0, 1, 0);
		Lookat = rotation * Lookat;
		Lookat = Lookat + Position;
		//Velocity += (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);

	}

	else
	{
		Lookat = Lookat - Position;
		rotation.SetToRotation(rotation_speed * dt, 0, 1, 0);
		Lookat = rotation * Lookat;
		Lookat = Lookat + Position;
		//Velocity += (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);
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
		b_aiCooldown = true;
		b_updateAI = true;
		b_rotateClockwiseFirst = NULL;
	}

	Mtx44 rotation;
	Lookat = Lookat - Position;
	rotation.SetToRotation(d_enemyRotation, 0, 1, 0);
	Lookat = rotation * Lookat;
	Lookat = Lookat + Position;
}

/******************************************************************************/
/*!
\brief
Update the sensors for pathfinding
\param left
sensors made to see if there is anything in the way of the AI
*/
/******************************************************************************/
double AI::getPlayerEscapeRange()
{
	return d_playerEscapeRange;
}

double AI::getDetectionAngle()
{
	return d_detectionAngle;
}

double AI::getDetectionRange()
{
	return d_detectionRange;
}

double AI::getDetectionRange_Max()
{
	return d_detectionRangeMax;
}

Vector3 AI::getDestination()
{
	return destination;
}

AI::E_AI_STATE AI::getState()
{
	return e_State;
}

void AI::UpdateLookat(const double &dt)
{
	if(currentLookat != NULL)
	{
		b_updateAI = false;

		if(Lookat != currentLookat)
		{
			if(Lookat.x != currentLookat.x)
			{
				if(Lookat.x < currentLookat.x)
				{
					Lookat.x += 50 * dt;

					if(Lookat.x > currentLookat.x)
					{
						Lookat.x = currentLookat.x;
					}
				}
				else
				{
					Lookat.x -= 50 * dt;

					if(Lookat.x < currentLookat.x)
					{
						Lookat.x = currentLookat.x;
					}
				}
			}

			if(Lookat.z != currentLookat.z)
			{
				if(Lookat.z < currentLookat.z)
				{
					Lookat.z += 50 * dt;

					if(Lookat.z > currentLookat.z)
					{
						Lookat.z = currentLookat.z;
					}
				}
				else
				{
					Lookat.z -= 50 * dt;

					if(Lookat.z < currentLookat.z)
					{
						Lookat.z = currentLookat.z;
					}
				}
			}
		}
		else
		{
			destination = currentLookat;
			prevPosition = Position;
			e_State = ALERT;
			b_aiCooldown = false;
			b_updateAI = true;
			currentLookat = NULL;
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
void AI::Update(double &dt, Vector3 playerPos, std::vector<CharacterObject *> &m_charList, std::vector<GameObject*> &m_GOList)
{
	switch (e_State)
	{
	case WALKING:
		{
			//Have the AI partol a certain area
			//Need Pathfinding i think

			if (isVisible(Position, Lookat, d_detectionAngle, playerPos))
			{
				//If player is infront and near player, then ai will switch to attack state
				if ((playerPos - Position).LengthSquared() < d_detectionRange)
				{
					prevPosition = Position;
					e_State = ATTACK;
					b_aiCooldown = false;
				}
				//if ai saw player but is too far way, the ai will investigate
				else if ((playerPos - Position).LengthSquared() > d_detectionRange && (playerPos - Position).LengthSquared() < d_detectionRangeMax) 
				{
					/*destination.x = playerPos.x;
					destination.z = playerPos.z;
					currentLookat = destination;*/
					currentLookat.x = playerPos.x;
					currentLookat.z = playerPos.z;
				}
			}

			if(b_aiCooldown)
			{
				//Ai will move towards prev position
				if ((Position - prevPosition).LengthSquared() > 2)
				{
					Lookat = prevPosition;
				}
				//AI is at the destination
				else
				{
					b_aiCooldown = false;
				}
			}
		}
		break;

	case ALERT:
		{
			//AI will move towards the destination
			if ((Position - destination).LengthSquared() > 2)
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
			if (isVisible(Position, Lookat, d_detectionAngle, playerPos) && (playerPos - Position).LengthSquared() < d_detectionRange)
			{
				e_State = ATTACK;
				b_updateAI = true;
				b_rotateClockwiseFirst = NULL;
			}

			////If ai is at destination
			//else
			//{
			//	static float alertTime = 5.f;

			//	if (f_alert_timer < alertTime)
			//	{
			//		f_alert_timer += dt;
			//	}
			//	else
			//	{
			//		//Alert enemy within a small radius
			//		for (std::vector<CharacterObject*>::iterator it = m_charList.begin(); it != m_charList.end(); it++)
			//		{
			//			AI *ai = dynamic_cast<AI*>((CharacterObject*)*it);
			//			if (ai->getPosition() != Position)
			//			{
			//				if ((Position - ai->getPosition()).LengthSquared() < 100)
			//				{
			//					ai->e_State = ATTACK;
			//				}
			//			}
			//		}
			//		e_State = ATTACK;
			//		f_alert_timer = 0;
			//	}
			//}

			//if player have escaped ai, cooldown first before returning to walking
			if (b_aiCooldown)
			{
				static float cooldownTiming = 2.f;

				if (f_cooldownTime < cooldownTiming)
				{
					f_cooldownTime += dt;
				}
				else
				{
					e_State = WALKING;
					f_cooldownTime = 0.f;
				}
			}
		}
		break;

	case ATTACK:
		{
			Lookat = playerPos;

			//if enemy is holding a weapon
			if (holding != NULL)
			{
				if(holding->isWeapon)
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

	UpdateLookat(dt);

	Mtx44 rotation;
	rotation.SetToRotation(CalAnglefromPosition(Lookat, Position, true), 0.f, 1.f, 0.f);
	Vector3 L, R, C;
	C = rotation * Vector3(0.f, ModelPos.y, 60.f);
	L = rotation * Vector3(-20.f, ModelPos.y, 15.f);
	R = rotation * Vector3(20.f, ModelPos.y, 15.f);

	SensorUpdate(dt, collisionChecking(Position + L, m_charList, m_GOList), collisionChecking(Position + C, m_charList, m_GOList), collisionChecking(Position + R, m_charList, m_GOList));

	if (Velocity.x != 0)
	{
		float SForceX = 0 - Velocity.x;
		Velocity.x += SForceX * 0.1f;
	}

	if (Velocity.z != 0)
	{	
		float SForceZ = 0 - Velocity.z;
		Velocity.z += SForceZ * 0.1f;
	}

	if(b_updateAI)
	{
		Animation.Update(dt, Velocity.LengthSquared());
		Lookat += Velocity * 10 * static_cast<float>(dt);
		Position += Velocity * 10 * static_cast<float>(dt);
	}
}

/******************************************************************************/
/*!
\brief
To detect collision
\param dt
Vector3 pos
CharacterObject vector list - To check collision
GameObject vector list - To check collision
bool checkGO -
*/
/******************************************************************************/
bool AI::collisionChecking(Vector3 &pos, std::vector<CharacterObject *> &m_charList, std::vector<GameObject *> &m_GOList, bool checkGO)
{
	//Check against GameObject
	if (checkGO)
	{
		for (std::vector<GameObject*>::iterator it = m_GOList.begin(); it != m_GOList.end(); it++)
		{
			GameObject* go = (GameObject*)*it;
			if (go->active && go->colEnable && go->pos != Position)
			{
				if (intersect(go->pos + go->ColBox, go->pos - go->ColBox, pos))
				{
					return true;
				}
			}
		}
	}
	//Check against Character Object
	else
	{
		for (std::vector<CharacterObject*>::iterator it = m_charList.begin(); it != m_charList.end(); it++)
		{
			CharacterObject * CO = (CharacterObject*)*it;
			if (CO->getPosition() != Position)
			{
				if (intersect(CO->getPosition() + Vector3(5, 5, 5), CO->getPosition() - Vector3(5, 5, 5), Position))
				{
					return true;
				}

			}
		}
	}
	return false;
}