#include "SecurityCam.h"

double SecurityCam::d_cameraRange_Angle = 30;
double SecurityCam::d_cameraRange = 48000;

SecurityCam::SecurityCam(void) : Lookat(0, 0, -1), c_State(NOTFOUND)
{

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

void SecurityCam::update(const double &dt, Vector3 &playerPos, std::vector<GameObject*> m_goList)
{
	static double test = 0;
	if((isVisible(pos, Lookat, static_cast<float>(d_cameraRange_Angle), playerPos)) && (pos - playerPos).LengthSquared() < d_cameraRange)
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
	};
}