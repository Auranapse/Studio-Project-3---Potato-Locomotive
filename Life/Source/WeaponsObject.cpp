/****************************************************************************/
/*!
\file WeaponsObject.cpp
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Class to handle weapons
!*/
/****************************************************************************/
#include "WeaponsObject.h"

/******************************************************************************/
/*!
\brief
Default Constructor
*/
/******************************************************************************/
WeaponsObject::WeaponsObject()
{
	isWeapon = true;
	shootvelocity = 60.f;
	recoilEffect = 10.f;
	name = "SHIT";
	attackRate = 0.2f;
	reloadTime = 0.f;
	adsZoom = 2.f;
	bulletSpread = 0.f;
	numBullet = 1;
	CurrentClip = 10;
	animState = true;
	Rotation1.SetZero();
	Rotation2.SetZero();
	AnimSpeed = 10.f;
	animComplete = false;
	timer = 0.f;
	range = 5.f;
}

/******************************************************************************/
/*!
\brief
Default Destructor
*/
/******************************************************************************/
WeaponsObject::~WeaponsObject()
{
	
}

/******************************************************************************/
/*!
\brief
Toggles the animation state
*/
/******************************************************************************/
void WeaponsObject::toggleAnimation()
{
	if (animState)
	{
		animState = false;
	}
	else
	{
		animState = true;
	}
	animComplete = false;
}

/******************************************************************************/
/*!
\brief
Updates the weapon animtations
\param dt
delta time
*/
/******************************************************************************/
void WeaponsObject::Update(double &dt)
{
	if (isHeld)
	{
		if (animState)
		{
			if (pos != pos1)
			{
				Vector3 v3_temp1 = pos1 - pos;
				pos += v3_temp1 * static_cast<float>(dt) * AnimSpeed;
				animComplete = false;
			}

			if (rotation != Rotation1)
			{
				Vector3 v3_temp1 = Rotation1 - rotation;
				rotation += v3_temp1 * static_cast<float>(dt) * AnimSpeed;
				animComplete = false;
			}

			if (!animComplete)
			{
				if (pos.x < pos1.x + 0.01f && pos.x > pos1.x - 0.01f && pos.y < pos1.y + 0.01f && pos.y > pos1.y - 0.01f && pos.z < pos1.z + 0.01f && pos.z > pos1.z - 0.01f)
				{
					pos = pos1;

					if (rotation.x < Rotation1.x + 0.01f && rotation.x > Rotation1.x - 0.01f && rotation.y < Rotation1.y + 0.01f && rotation.y > Rotation1.y - 0.01f && rotation.z < Rotation1.z + 0.01f && rotation.z > Rotation1.z - 0.01f)
					{
						rotation = Rotation1;
						animComplete = true;
					}
				}
			}
		}
		else
		{

			if (isGun)
			{
				if (pos != pos2)
				{
					Vector3 v3_temp1 = pos2 - pos;
					pos += v3_temp1 * static_cast<float>(dt) * AnimSpeed;
					animComplete = false;
				}

				if (rotation != Rotation2)
				{
					Vector3 v3_temp1 = Rotation2 - rotation;
					rotation += v3_temp1 * static_cast<float>(dt) * AnimSpeed;
					animComplete = false;
				}

				if (!animComplete)
				{
					if (pos.x < pos2.x + 0.01f && pos.x > pos2.x - 0.01f && pos.y < pos2.y + 0.01f && pos.y > pos2.y - 0.01f && pos.z < pos2.z + 0.01f && pos.z > pos2.z - 0.01f)
					{
						pos = pos2;

						if (rotation.x < Rotation2.x + 0.01f && rotation.x > Rotation2.x - 0.01f && rotation.y < Rotation2.y + 0.01f && rotation.y > Rotation2.y - 0.01f && rotation.z < Rotation2.z + 0.01f && rotation.z > Rotation2.z - 0.01f)
						{
							rotation = Rotation2;
							animComplete = true;
						}
					}
				}
			}

			else
			{
				if (pos != pos2)
				{
					Vector3 v3_temp1 = pos2 - pos;
					pos += v3_temp1 * static_cast<float>(dt) * AnimSpeed;
					animComplete = false;
				}

				if (rotation != Rotation2)
				{
					Vector3 v3_temp1 = Rotation2 - rotation;
					rotation += v3_temp1 * static_cast<float>(dt) * AnimSpeed;
					animComplete = false;
				}

				if (timer > attackRate)
				{
					timer = 0.f;
					animState = true;
				}
				else
				{
					timer += static_cast<float>(dt);
				}
			}
		}
	}
}

/******************************************************************************/
/*!
\brief
check if animation completed
\return
returns true if animation is completed
*/
/******************************************************************************/
bool WeaponsObject::isAnimationComplete(void)
{
	return animComplete;
}

/******************************************************************************/
/*!
\brief
force sets the animation completion
\param isitreally
if true will set animComplete to true
*/
/******************************************************************************/
void WeaponsObject::isAnimationComplete(bool isitreally)
{
	animComplete = isitreally;
}