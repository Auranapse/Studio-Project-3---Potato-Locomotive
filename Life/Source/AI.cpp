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
	x_axis = z_axis = false;

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
Updates the AI
\param dt
delta time
CharacterObject vector list - To check collision
GameObject vector list - To check collision
*/
/******************************************************************************/
void AI::Update(double &dt, Vector3 playerPos, std::vector<CharacterObject *> &m_charList, std::vector<GameObject*> &m_GOList)
{
	//if (collided)
	//{
	//	int ai_nextMove = Math::RandIntMinMax(1, 3);
	//	//1 = clodkwise
	//	//2 = counter clock wise
	//	//3 = 180 degree

	//	//Positive Z
	//	if (movingByPositive_z())
	//	{

	//		if (ai_nextMove == 1)
	//		{
	//			setNegative_x();
	//		}
	//		else if (ai_nextMove == 2)
	//		{
	//			setPositive_x();
	//		}
	//		else
	//		{
	//			setNegative_z();
	//		}
	//	}
	//	//Negative X
	//	else if (movingByNegative_x())
	//	{
	//		if (ai_nextMove == 1)
	//		{
	//			setNegative_z();
	//		}
	//		else if (ai_nextMove == 2)
	//		{
	//			setPositive_z();
	//		}
	//		else
	//		{
	//			setPositive_x();
	//		}
	//	}
	//	//Negative Z
	//	else if (movingByNegative_z())
	//	{
	//		if (ai_nextMove == 1)
	//		{
	//			setPositive_x();
	//		}
	//		else if (ai_nextMove == 2)
	//		{
	//			setNegative_x();
	//		}
	//		else
	//		{
	//			setPositive_z();
	//		}
	//	}
	//	//Positive X
	//	else if (movingByPositive_x())
	//	{
	//		if (ai_nextMove == 1)
	//		{
	//			setPositive_z();
	//		}
	//		else if (ai_nextMove == 2)
	//		{
	//			setNegative_z();
	//		}
	//		else
	//		{
	//			setNegative_x();
	//		}
	//	}

	//	if (ai_nextMove == 1)
	//	{
	//		rotateAi_Clockwise();
	//	}
	//	else if (ai_nextMove == 2)
	//	{
	//		rotateAI_CounterClockWise();
	//	}
	//	else
	//	{
	//		rotateAI_180();
	//	}

	//	collided = false;
	//}
	//else
	//{

	//	//Positive Z
	//	if (positiveZ)
	//	{
	//		if (collisionChecking(Vector3(Position + Vector3(0.f, 10.f, diff.z * 10)), m_charList, m_GOList) || collisionChecking(Vector3(Position + Vector3(0.f, 10.f, diff.z * 10)), m_charList, m_GOList, false))
	//		{
	//			collided = true;
	//		}
	//	}
	//	//Negative Z
	//	else if(negativeZ)
	//	{
	//		if (collisionChecking(Vector3(Position + Vector3(0.f, 10.f, diff.z * 10)), m_charList, m_GOList) || collisionChecking(Vector3(Position + Vector3(0.f, 10.f, diff.z * 10)), m_charList, m_GOList, false))
	//		{
	//			collided = true;
	//		}
	//	}

	//	//Positive X
	//	if (positiveX)
	//	{
	//		if (collisionChecking(Vector3(Position + Vector3(diff.x * 10, 10.f, 0.f)), m_charList, m_GOList) || collisionChecking(Vector3(Position + Vector3(diff.x * 10, 10.f, 0.f)), m_charList, m_GOList, false))
	//		{
	//			collided = true;
	//		}
	//	}
	//	//Negative X
	//	else if(negativeX)
	//	{
	//		if (collisionChecking(Vector3(Position + Vector3(diff.x * 10, 10.f, 0.f)), m_charList, m_GOList) || collisionChecking(Vector3(Position + Vector3(diff.x * 10, 10.f, 0.f)), m_charList, m_GOList, false))
	//		{
	//			collided = true;
	//		}
	//	}

		/*if (Velocity.x != 0)
		{
		float SForceX = 0 - Velocity.x;
		Velocity.x += SForceX * 0.1f;
		}

		if (Velocity.z != 0)
		{
		float SForceZ = 0 - Velocity.z;
		Velocity.z += SForceZ * 0.1f;
		}*/


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

	Animation.Update(dt, Velocity.LengthSquared());
	Lookat += Velocity * 10 * static_cast<float>(dt);
	Position += Velocity * 10 * static_cast<float>(dt);
	
	////=============Positive X Velocity=============//
	//if (collided == false)
	//{
	//	if (collisionChecking(Vector3(Position + Vector3(10.f, 10.f, 0.f)), m_charList, m_GOList, false) || collisionChecking(Vector3(Position + Vector3(10.f, 30.f, 0.f)), m_charList, m_GOList, false))
	//	{
	//		//First if statement is to check against Character Object - AI
	//		//Position.x = Position.x +  offset * static_cast<float>(dt);
	//		if (Velocity.x > 0)
	//		{
	//			Velocity.x = 0;
	//		}

	//		//SensorUpdate(dt, collisionChecking(Position + L, m_charList, m_GOList), collisionChecking(Position + R, m_charList, m_GOList), collisionChecking(Position + C, m_charList, m_GOList));
	//	}
	//	else if (collisionChecking(Vector3(Position + Vector3(10.f, 10.f, 0.f)), m_charList, m_GOList) || collisionChecking(Vector3(Position + Vector3(10.f, 50.f, 0.f)), m_charList, m_GOList))
	//	{
	//		//Else if is to check against GameObject - Walls
	//		if (Velocity.x > 0)
	//		{
	//			Velocity.x = 0;
	//		}
	//		std::cout << "Positive X" << std::endl;

	//	}

	//	//=============Positive Z Velocity=============//
	//	if (collisionChecking(Vector3(Position + Vector3(0.f, 10.f, 10.f)), m_charList, m_GOList, false) || collisionChecking(Vector3(Position + Vector3(0.f, 30.f, 10.f)), m_charList, m_GOList, false))
	//	{
	//		//Position.z = Position.z +  offset * static_cast<float>(dt);
	//		if (Velocity.z > 0)
	//		{
	//			Velocity.z = 0;
	//		}
	//	}
	//	else if (collisionChecking(Vector3(Position + Vector3(0.f, 10.f, 10.f)), m_charList, m_GOList) || collisionChecking(Vector3(Position + Vector3(0.f, 50.f, 10.f)), m_charList, m_GOList))
	//	{
	//		if (Velocity.z > 0)
	//		{
	//			Velocity.z = 0;

	//			Lookat = rotating * Lookat;
	//			Velocity += (getDirection(true).Normalize() * f_movementSpeed) * static_cast<float>(dt);
	//		}
	//		std::cout << "Positive Z" << std::endl;
	//	}

	//	//=============Negative X Velocity=============//
	//	if (collisionChecking(Vector3(Position + Vector3(-10.f, 10.f, 0.f)), m_charList, m_GOList, false) || collisionChecking(Vector3(Position + Vector3(-10.f, 30.f, 0.f)), m_charList, m_GOList, false))
	//	{
	//		//Position.x = Position.x - offset * static_cast<float>(dt);
	//		if (Velocity.x < 0)
	//		{
	//			Velocity.x = 0;
	//		}
	//	}
	//	else if (collisionChecking(Vector3(Position + Vector3(-10.f, 10.f, 0.f)), m_charList, m_GOList) || collisionChecking(Vector3(Position + Vector3(-10.f, 50.f, 0.f)), m_charList, m_GOList))
	//	{
	//		if (Velocity.x < 0)
	//		{
	//			Velocity.x = 0;
	//		}
	//		std::cout << "Negative X" << std::endl;
	//	}

	//	//=============Negative Z Velocity=============//
	//	if (collisionChecking(Vector3(Position + Vector3(0.f, 10.f, -10.f)), m_charList, m_GOList, false) || collisionChecking(Vector3(Position + Vector3(0.f, 30.f, -10.f)), m_charList, m_GOList, false))
	//	{
	//		//Position.z = Position.z - offset * static_cast<float>(dt);
	//		if (Velocity.z < 0)
	//		{
	//			Velocity.z = 0;
	//		}
	//	}
	//	else if (collisionChecking(Vector3(Position + Vector3(0.f, 10.f, -10.f)), m_charList, m_GOList) || collisionChecking(Vector3(Position + Vector3(0.f, 50.f, -10.f)), m_charList, m_GOList))
	//	{
	//		if (Velocity.z < 0)
	//		{
	//			Velocity.z = 0;
	//		}
	//		std::cout << "Negative Z" << std::endl;
	//	}
	//}

	//	

	/*static float offset = 40.f;
	Mtx44 rotation;
	rotation.SetToRotation(CalAnglefromPosition(Lookat, Position, true), 0.f, 1.f, 0.f);
	Vector3 L, R, C;
	C = rotation * Vector3(0.f, 0.f, 200.f);
	L = rotation * Vector3(-20.f, 0.f, 150.f);
	R = rotation * Vector3(20.f, 0.f, 150.f);*/
	/*static float test = 0.f;
	if (test < 1.f)
	test += dt;
	else
	test = 1.f;
	if (Application::IsKeyPressed(VK_UP) && test >= 1.f)
	{
	std::cout << Lookat << std::endl;
	Lookat = rotating * Lookat;
	std::cout << Lookat << std::endl;
	test = 0.f;
	}*/


	//if (Velocity.x != 0)
	//{
	//	float SForceX = 0 - Velocity.x;
	//	Velocity.x += SForceX * 0.1f;
	//}

	//if (Velocity.z != 0)
	//{
	//	float SForceZ = 0 - Velocity.z;
	//	Velocity.z += SForceZ * 0.1f;
	//}


	////if((Position - playerPos).LengthSquared() > 400)
	//Animation.Update(dt, Velocity.LengthSquared() * 0.03);
	//Lookat += Velocity * static_cast<float>(dt);
	//Position += Velocity * static_cast<float>(dt);
	//else
	//Velocity = 0;
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