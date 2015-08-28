#include "SecurityCam.h"

double SecurityCam::d_cameraRange_Angle = 30;
double SecurityCam::d_cameraRange = 48000;

SecurityCam::SecurityCam(void) : Lookat(0, 0, -1), c_State(NOTFOUND)
{
	d_totalRotation = 0.f;
	d_currentRotation = 0.f;
	f_rotationAngle = 0;
}


SecurityCam::~SecurityCam(void)
{
}

SecurityCam::CAMERA_STATE SecurityCam::getState()
{
	return c_State;
}

double SecurityCam::getCameraRange_Angle()
{
	return d_cameraRange_Angle;
}

double SecurityCam::getCameraRange()
{
	return d_cameraRange;
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

	if((isVisible(SCPos, Lookat, static_cast<float>(d_cameraRange_Angle), playerPos)) && (SCPos - playerPos).LengthSquared() < d_cameraRange)
	{
		std::cout << "Spotted" << std::endl;
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

	case NOTFOUND:
		{
			d_totalRotation += abs(d_currentRotation);

			if(d_totalRotation < f_rotationAngle)
			{
				d_currentRotation = 10 * dt;
			}
			else if (d_totalRotation > f_rotationAngle && d_totalRotation <= f_rotationAngle * 2)
			{
				d_currentRotation = -10 * dt;
			}
			else
			{
				d_totalRotation = 0.0;
			}

			Mtx44 rotation;
			Lookat = Lookat - pos;
			rotation.SetToRotation(d_currentRotation, 0, 1, 0);
			Lookat = rotation * Lookat;
			Lookat = Lookat + pos;
		}
		break;
	};
}