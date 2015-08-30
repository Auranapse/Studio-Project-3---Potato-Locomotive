#include "SecurityCam.h"

float SecurityCam::f_cameraFOV = 30;
float SecurityCam::f_cameraRange = 48000;

SecurityCam::SecurityCam(void) : Lookat(0, 0, -1), c_State(NOTFOUND)
{
	alerttimer = 0.f;
	f_rotationAngle = 0.f;
	f_rotationLimiter = 0.f;
	rotationState = false;
	b_alertAI = true;
}


SecurityCam::~SecurityCam(void)
{
}

SecurityCam::CAMERA_STATE SecurityCam::getState()
{
	return c_State;
}

float SecurityCam::getCameraFOV()
{
	return f_cameraFOV;
}

float SecurityCam::getCameraRange()
{
	return f_cameraRange;
}

void SecurityCam::setRotationAngle(float f_rotationAngle)
{
	this->f_rotationAngle = f_rotationAngle;
}

void SecurityCam::CollisionChecking(std::vector<GameObject *> &m_goList)
{
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); it++)
	{
		GameObject * go = (GameObject*)*it;
		if (go->active && go->pos != pos)
		{
			Vector3 SCPos = pos;
			Vector3 SCLookat = Lookat;

			SCPos.y = SCLookat.y = 0;
			if (isVisible(SCPos, SCLookat, f_cameraFOV, go->pos))
			{
				f_cameraRange = (SCPos - go->pos).Length();
			}
			else
			{
				f_cameraRange = 48000;
			}
		}
	}
}

void SecurityCam::update(const double &dt, Vector3 &playerPos, std::vector<GameObject*> m_goList)
{

	Vector3 SCPos = pos;
	Vector3 SCLookat = Lookat;

	SCPos.y = SCLookat.y = 0;

	switch(c_State)
	{
	case SPOTTED:
		{
			Lookat = playerPos;
			//Check whether player is still in the fov of security camera
			if (((isVisible(SCPos, SCLookat, static_cast<float>(f_cameraFOV), playerPos)) && (SCPos - playerPos).LengthSquared() < f_cameraRange))
			{
				//Give player 1 sec to prevent detection by the security camera
				if (alerttimer < 1)
				{
					alerttimer += dt;
				}
				else
				{
					c_State = FOUND;
					alerttimer = 0;
				}
			}
			else
			{
				//if player is no longer in the fov of the security camera
				c_State = NOTFOUND;
				alerttimer = 0;
			}
		}
		break;

	case FOUND:
		{
			Lookat = playerPos;

			//Alert all the ai to the camera's position
			if (b_alertAI)
			{
				for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); it++)
				{
					GameObject *go = (GameObject *)*it;
					AI *ai = dynamic_cast<AI*>(go);
					if (ai != NULL)
					{
						if (ai->getState() == AI::WALKING)
						{
							ai->setState(AI::ALERT);
							Vector3 cameraPos = pos;
							cameraPos.y = 0;
							ai->setDestination(cameraPos);
						}
					}
				}
				b_alertAI = false;
			}
			else
			{
				if (isVisible(pos, Lookat, f_cameraFOV, playerPos) && (playerPos - pos).LengthSquared() > f_cameraRange)
				{
					c_State = NOTFOUND;
				}
			}
		}
		break;

	case NOTFOUND:
		{
			//Check whether enemy is within security camera's FOV
			if (((isVisible(SCPos, SCLookat, static_cast<float>(f_cameraFOV), playerPos)) && (SCPos - playerPos).LengthSquared() < f_cameraRange))
			{
				c_State = SPOTTED;
			}
		}
		break;
	}
	
	if (c_State == NOTFOUND)
	{
		float f_currentRotation = 0;

		if (rotationState)
		{
			f_currentRotation = 20;

			if (f_rotationLimiter > f_rotationAngle)
			{
				rotationState = false;
			}
		}
		else
		{
			f_currentRotation = -20;

			if (f_rotationLimiter < -f_rotationAngle)
			{
				rotationState = true;
			}
		}

		if (f_currentRotation != 0)
		{
			Mtx44 rotation;
			Lookat = Lookat - pos;
			f_rotationLimiter += f_currentRotation * static_cast<float>(dt);
			rotation.SetToRotation(f_currentRotation * static_cast<float>(dt), 0, 1, 0);
			Lookat = rotation * Lookat;
			Lookat = Lookat + pos;
		}
	}
	//CollisionChecking(m_goList);
}