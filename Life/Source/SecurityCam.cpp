#include "SecurityCam.h"

float SecurityCam::f_cameraFOV = 30;
float SecurityCam::f_cameraRange = 48000;

SecurityCam::SecurityCam(void) : Lookat(0, 0, -1), c_State(NOTFOUND)
{
	f_rotationAngle = 0.f;
	f_rotationLimiter = 0.f;
	rotationState = false;
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

void SecurityCam::update(const double &dt, Vector3 &playerPos, std::vector<GameObject*> m_goList)
{
	static double test = 0;

	Vector3 SCPos = pos;
	SCPos.y = 0;

	if((isVisible(SCPos, Lookat, static_cast<float>(f_cameraFOV), playerPos)) && (SCPos - playerPos).LengthSquared() < f_cameraRange)
	{
		c_State = SPOTTED;
	}
	else
	{
		c_State = NOTFOUND;
	}

	switch(c_State)
	{
	case SPOTTED:
		{
			test += dt;

			if(test >= 2)
			{
				c_State = FOUND;
			}
		}
		break;
	case FOUND:
		{
			for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); it++)
			{
				GameObject *go = (GameObject *)*it;
				AI *ai = dynamic_cast<AI*>(go);
				if(ai != NULL)
				{
					if (ai->getState() != AI::ATTACK)
					{
						ai->setState(AI::ALERT);
						Vector3 cameraPos = pos;
						cameraPos.y = 0;
						ai->setDestination(cameraPos);
					}
				}
			}
		}
		break;		
	}
	
	float f_currentRotation = 0;
	
	if(rotationState)
	{
		f_currentRotation = 20;

		if(f_rotationLimiter > f_rotationAngle)
		{
			rotationState = false;
		}
	}
	else
	{
		f_currentRotation = -20;

		if(f_rotationLimiter < -f_rotationAngle)
		{
			rotationState = true;
		}
	}

	if(f_currentRotation != 0)
	{
		Mtx44 rotation;
		Lookat = Lookat - pos;
		f_rotationLimiter += f_currentRotation * static_cast<float>(dt);
		rotation.SetToRotation(f_currentRotation * static_cast<float>(dt), 0, 1, 0);
		Lookat = rotation * Lookat;
		Lookat = Lookat + pos;
	}
}