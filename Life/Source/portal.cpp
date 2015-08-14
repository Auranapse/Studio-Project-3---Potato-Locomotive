#include "portal.h"

Portal::Portal()
{
	BluePortal = new GameObject(GameObject::GO_PORTAL);
	OrangePortal = new GameObject(GameObject::GO_PORTAL);
	BluePortal->active = false;
	OrangePortal->active = false;
	BluePortal->scale.Set(2.25, 2.25, 2.25);
	OrangePortal->scale.Set(2.25, 2.25, 2.25);
	BluePortal->ColBox.Set(2, 2, 2);
	OrangePortal->ColBox.Set(2, 2, 2);
	BP_AttachedTo = NULL;
	OP_AttachedTo = NULL;
}

Portal::~Portal()
{
	if(BluePortal)
	{
		delete BluePortal;
		BluePortal = NULL;
	}
	if(OrangePortal)
	{
		delete OrangePortal;
		OrangePortal = NULL;
	}
}

bool Portal::pVelChange(Vector3 &Vel, bool o2b)
{
	if(BluePortal->rotationZ - OrangePortal->rotationZ == 0)
	{
		if(BluePortal->rotationZ != 0)
			Vel.x = -Vel.x;

		Vel.y = -Vel.y;
	}
	
	else if(BluePortal->rotationZ - OrangePortal->rotationZ == -90 || BluePortal->rotationZ - OrangePortal->rotationZ == 270)
	{
		float tempX, tempY;
		tempX = Vel.x;
		tempY = Vel.y;
		if(o2b)
		{
			Vel.x = -tempY;
			Vel.y = tempX;
		}
		else
		{
			Vel.x = tempY;
			Vel.y = -tempX;
		}
	}

	else if(BluePortal->rotationZ - OrangePortal->rotationZ == 90 || BluePortal->rotationZ - OrangePortal->rotationZ == -270)
	{
		float tempX, tempY;
		tempX = Vel.x;
		tempY = Vel.y;
		if(o2b)
		{
			Vel.x = tempY;
			Vel.y = -tempX;
		}
		else
		{
			Vel.x = -tempY;
			Vel.y = tempX;
		}
	}

	if(o2b)
	{
		if(BluePortal->rotationZ == 0)
		{
			if(Vel.y < 30)
			{
				Vel.y = 30;
			}
		}
		else if (BluePortal->rotationZ == -90)
		{
			if (Vel.x < 10)
			{
				Vel.x = 10;
			}
		}
		else if (BluePortal->rotationZ == 90)
		{
			if (Vel.x > -10)
			{
				Vel.x = -10;
			}
		}
	}
	else
	{
		if(OrangePortal->rotationZ == 0)
		{
			if(Vel.y < 30)
			{
				Vel.y = 30;
			}
			else if (OrangePortal->rotationZ == -90)
			{
				if (Vel.x < 10)
				{
					Vel.x = 10;
				}
			}
			else if (OrangePortal->rotationZ == 90)
			{
				if (Vel.x > -10)
				{
					Vel.x = -10;
				}
			}
		}
	}
	return true;
}

void Portal::enterPortal(GameObject *go, bool o2b)
{
	pVelChange(go->vel, o2b);
	float tempZ = go->pos.z;
	
	if (o2b)
	{
		Mtx44 rotation;
		rotation.SetToRotation(BluePortal->rotationZ, 0, 0, 1);
		go->pos = BluePortal->pos + rotation*Vector3(0, BluePortal->ColBox.y, 0);
	}
	else
	{	
		Mtx44 rotation;
		rotation.SetToRotation(OrangePortal->rotationZ, 0, 0, 1);
		go->pos = OrangePortal->pos + rotation*Vector3(0, OrangePortal->ColBox.y, 0);
	}
	go->pos.z = tempZ;
}

void Portal::update(double dt)
{
	
}