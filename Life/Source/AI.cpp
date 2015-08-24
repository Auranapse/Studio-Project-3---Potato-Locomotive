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
	//x_axis = z_axis = false;
	destination(NULL);
	b_aiCooldown = false;
	f_cooldownTime = 0.f;
	positiveX = false, positiveZ = true, negativeX = false, negativeZ = false;
	diff = (0, 0, 1);
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
	if (forward)
	{
		Velocity += (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);
	}
	else
	{
		Velocity -= (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);
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
void AI::movementLR(double &dt, bool left)
{
	Mtx44 rotation;
	if (left == true)
	{
		rotation.SetToRotation(-90.f, 0.f, 1.f, 0.f);
		Lookat = rotation * Lookat * dt;
		Velocity += (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);

	}

	else
	{
		rotation.SetToRotation(90.f, 0.f, 1.f, 0.f);
		Lookat = rotation * Lookat * dt;
		Velocity += (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);
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
		movementLR(dt, false);
		left = false;
		mid = false;
		right = false;
	}

	//when left has nothing to collide
	else if (left == false && mid == true && right == true)
	{
		movementLR(dt, true);
		left = false;
		mid = false;
		right = false;
	}

	//when middle has nothing to collide
	else if (left == true && mid == false && right == true)
	{
		movementFB(dt, true);
		left = false;
		mid = false;
		right = false;
	}

	else if (left == false && mid == false && right == false)
	{
		movementFB(dt, true);
	}

	else if (left == true && mid == true && right == true)
	{
		movementFB(dt, false);
		left = false;
		mid = false;
		right = false;
	}
	/*else
	{
		movementFB(dt, true);
	}*/
}


/******************************************************************************/
/*!
\brief
Return true if the enemy's direction is positive x
*/
/******************************************************************************/
bool AI::movingByPositive_x()
{
	if (positiveX == true && negativeX == false && negativeZ == false && positiveZ == false)
	{
		return true;
	}
	return false;
}

/******************************************************************************/
/*!
\brief
Return true if the enemy's direction is positive z
*/
/******************************************************************************/
bool AI::movingByPositive_z()
{
	if (positiveX == false && negativeX == false && negativeZ == false && positiveZ == true)
	{
		return true;
	}
	return false;
}

/******************************************************************************/
/*!
\brief
Return true if the enemy's direction is negative x
*/
/******************************************************************************/
bool AI::movingByNegative_x()
{
	if (positiveX == false && negativeX == true && negativeZ == false && positiveZ == false)
	{
		return true;
	}
	return false;
}

/******************************************************************************/
/*!
\brief
Return true if the enemy's direction is negative z
*/
/******************************************************************************/
bool AI::movingByNegative_z()
{
	if (positiveX == false && negativeX == false && negativeZ == true && positiveZ == false)
	{
		return true;
	}
	return false;
}

/******************************************************************************/
/*!
\brief
Set the enemy direction to positive x
*/
/******************************************************************************/
void AI::setPositive_x()
{
	positiveX = true;
	positiveZ = false;
	negativeX = false;
	negativeZ = false;

	diff = Vector3(1, 0, 0);
}

/******************************************************************************/
/*!
\brief
Set the enemy direction to positive z
*/
/******************************************************************************/
void AI::setPositive_z()
{
	positiveX = false;
	positiveZ = true;
	negativeX = false;
	negativeZ = false;

	diff = Vector3(0, 0, 1);
}

/******************************************************************************/
/*!
\brief
Set the enemy direction to negative x
*/
/******************************************************************************/
void AI::setNegative_x()
{
	positiveX = false;
	positiveZ = false;
	negativeX = true;
	negativeZ = false;

	diff = Vector3(-1, 0, 0);
}

/******************************************************************************/
/*!
\brief
Set the enemy direction to negative z
*/
/******************************************************************************/
void AI::setNegative_z()
{
	positiveX = false;
	positiveZ = false;
	negativeX = false;
	negativeZ = true;

	diff = Vector3(0, 0, -1);
}

/******************************************************************************/
/*!
\brief
Rotate the enemy's Lookat in the clockwise direction(-90 degree)
*/
/******************************************************************************/
void AI::rotateAi_Clockwise()
{
	Mtx44 rotation;
	rotation.SetToRotation(-90, 0, 1, 0);

	Lookat = rotation * Lookat;

	Velocity = rotation * Velocity;
}

/******************************************************************************/
/*!
\brief
Rotate the enemy's Lookat in the counter clockwise direction(90 degree)
*/
/******************************************************************************/
void AI::rotateAI_CounterClockWise()
{
	Mtx44 rotation;
	rotation.SetToRotation(90, 0, 1, 0);

	Lookat = rotation * Lookat;

	Velocity = rotation * Velocity;
}

/******************************************************************************/
/*!
\brief
Rotate the enemy's Lookat by 180 degree
*/
/******************************************************************************/
void AI::rotateAI_180()
{
	Mtx44 rotation;
	rotation.SetToRotation(180, 0, 1, 0);

	Lookat = rotation * Lookat;

	Velocity = rotation * Velocity;
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

			if (isVisible(Position, Lookat, 60, playerPos))
			{
				//If player is infront and near player, then ai will switch to attack state
				if ((playerPos - Position).LengthSquared() < 400)
				{
					e_State = ATTACK;
				}
				//if ai saw player but is too far way, the ai will investigate
				else
				{
					destination = playerPos;
					e_State = ALERT;
				}
			}
		}
		break;

	case ALERT:
		{
			//AI will move towards the destination
			if (Position != destination)
			{
				//Move the ai towards the destination
			}
			//If ai is at destination
			else
			{
				static float alertTime = 5.f;

				if (f_alert_timer < alertTime)
				{
					f_alert_timer += dt;
				}
				else
				{
					//Alert enemy within a small radius
					for (std::vector<CharacterObject*>::iterator it = m_charList.begin(); it != m_charList.end(); it++)
					{
						CharacterObject * CO = (CharacterObject*)*it;
						AI *ai = dynamic_cast<AI*>(CO);
						if (ai->getPosition() != Position)
						{
							if ((Position - ai->getPosition()).LengthSquared() < 100)
							{
								ai->e_State = ATTACK;
							}
						}
					}
					e_State = ATTACK;
					f_alert_timer = 0;
				}
			}

			//If player is infront and near player, then ai will switch to attack state
			if (isVisible(Position, Lookat, 60, playerPos) && (playerPos - Position).LengthSquared() < 400)
			{
				e_State = ATTACK;
			}

			//if player have escaped ai, cooldown first before returning to walking
			if (b_aiCooldown)
			{
				static float cooldownTiming = 5.f;

				if (f_cooldownTime < 5.f)
				{
					f_cooldownTime += dt;
				}
				else
				{
					e_State = WALKING;
					f_cooldownTime = 0.f;
					b_aiCooldown = false;
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
				//Enemy is holding a gun
				if (holding->isGun)
				{
					//Make enemy move a certain distance away from the enemy before shooting
				}
			}
			//Enemy is not holding a weapon
			else
			{
				if ((playerPos - Position).LengthSquared() > 400)
				{
					//make the enemy move closer to the nemy before attacking
				}
			}

			//AI return to alert state if player have avoided enemy
			if ((Position - playerPos).LengthSquared() > 10000)
			{
				b_aiCooldown = true;
				e_State = ALERT;
			}
		}
		break;

	default:
		break;
	}

	Mtx44 rotation;
	rotation.SetToRotation(CalAnglefromPosition(Lookat, Position, true), 0.f, 1.f, 0.f);
	Vector3 L, R, C;
	C = rotation * Vector3(0.f, 10.f, 10.f);
	L = rotation * Vector3(5.f, 10.f, 5.f);
	R = rotation * Vector3(5.f, 10.f, 5.f);

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

	Animation.Update(dt, Velocity.LengthSquared());
	Lookat += Velocity * 10 * static_cast<float>(dt);
	Position += Velocity * 10 * static_cast<float>(dt);
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