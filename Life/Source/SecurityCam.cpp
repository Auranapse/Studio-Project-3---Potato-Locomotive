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
	prevRotation = NULL;
	f_currentRotation = 0;
	b_cameraRotating = true;
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
				f_cameraRange = (SCPos - go->pos).LengthSquared();
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
					alerttimer += static_cast<float>(dt);
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
			if(prevRotation == NULL)
			{
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
				//rotationState = false;
				//Check whether enemy is within security camera's FOV
				if (((isVisible(SCPos, SCLookat, static_cast<float>(f_cameraFOV), playerPos)) && (SCPos - playerPos).LengthSquared() < f_cameraRange))
				{
					c_State = SPOTTED;
					prevRotation.x = Lookat.x;
					prevRotation.z = Lookat.z;
				}
			}
			else
			{
				if (b_cameraRotating)
				{
					/*static float rotationSpeed = 50;

					if(Lookat.x > prevRotation.x)
					{
						Lookat.x -= rotationSpeed * dt;
					}
					else if (Lookat.x < prevRotation.x)
					{
						Lookat.x += rotationSpeed * dt;
					}

					if(Lookat.z > prevRotation.z)
					{
						Lookat.z -= rotationSpeed * dt;
					}
					else if (Lookat.z < prevRotation.z)
					{
						Lookat.z += rotationSpeed * dt;
					}

					Vector3 LookatA = Lookat;
					LookatA.y = 0;
					std::cout << (LookatA - prevRotation).LengthSquared() << std::endl;
					if((LookatA - prevRotation).LengthSquared() < 26)
					{
						b_cameraRotating = false;
					}*/

				/*	Vector3 LookatA = Lookat;
					LookatA.y = 0;
					std::cout << (LookatA - prevRotation)- pos << std::endl;
					if((LookatA - prevRotation).Length() < 26)
					{
						b_cameraRotating = false;
					}*/
					float angle = CalAnglefromPosition(Vector3(defaultLookat.x, 0, defaultLookat.z), Vector3(Lookat.x, 0, Lookat.z), true);
					std::cout << angle << std::endl;
					
					if(angle < offsetY + 10)
					{
						std::cout << "Orange" << std::endl;
						Mtx44 rotation;
						Lookat = Lookat - pos;
						rotation.SetToRotation(static_cast<float>(-20 * dt), 0, 1, 0);
						Lookat = rotation * Lookat;
						Lookat = Lookat + pos;
					}
					else if (angle > offsetY - 10)
					{
						std::cout << "Apple" << std::endl;
						Mtx44 rotation;
						Lookat = Lookat - pos;
						rotation.SetToRotation(static_cast<float>(20 * dt), 0, 1, 0);
						Lookat = rotation * Lookat;
						Lookat = Lookat + pos;
					}
					else
					{
						std::cout << "Senpai no notice me" << std::endl;
						b_cameraRotating = false;
					}
				}
				else
				{
				/*	Lookat.x = prevRotation.x;
					Lookat.z = prevRotation.z;*/
					f_rotationLimiter = 0.f;
					rotationState = false;
					f_currentRotation = 0.f;
					prevRotation = NULL;
					b_cameraRotating = true;
				}
			}
		}
		break;
	}
}