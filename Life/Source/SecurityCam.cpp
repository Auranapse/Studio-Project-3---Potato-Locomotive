#include "SecurityCam.h"


SecurityCam::SecurityCam(void) : angle(0), Lookat(0, 0, -1), c_State(NOTFOUND)
{

}


SecurityCam::~SecurityCam(void)
{
}


float SecurityCam::getAngle()
{
	return angle;
}

SecurityCam::CAMERA_STATE SecurityCam::getState()
{
	return c_State;
}

void SecurityCam::update(const double &dt, Vector3 &playerPos, std::vector<CharacterObject*> charList)
{
	static double test = 0;
	if((isVisible(pos, Lookat, 40, playerPos)) && Collision::LengthSquared(pos, playerPos, 2500, false))
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
			for (std::vector<CharacterObject*>::iterator it = charList.begin(); it != charList.end(); it++)
			{
				AI * ai = dynamic_cast<AI*>((CharacterObject*)*it);
				ai->setState(AI::ALERT);
			}
		}
		break;
	};
}