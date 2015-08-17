#include "WeaponsObject.h"

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
	ClipSize = 10;
	CurrentClip = ClipSize;
	animState = true;
}

WeaponsObject::~WeaponsObject()
{

}

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
}

void WeaponsObject::Update(double &dt)
{
	if (animState)
	{
		if (pos != pos1)
		{
			Vector3 v3_temp1 = pos1 - pos;
			pos += v3_temp1 * static_cast<float>(dt) * 12.f;
		}

		if (rotation != Rotation1)
		{
			Vector3 v3_temp1 = Rotation1 - rotation;
			rotation += v3_temp1 * static_cast<float>(dt) * 12.f;
		}
	}
	else
	{
		if (pos != pos2)
		{
			Vector3 v3_temp1 = pos2 - pos;
			pos += v3_temp1 * static_cast<float>(dt) * 12.f;
		}

		if (rotation != Rotation2)
		{
			Vector3 v3_temp1 = Rotation2 - pos;
			rotation += v3_temp1 * static_cast<float>(dt) * 12.f;
		}
	}
}