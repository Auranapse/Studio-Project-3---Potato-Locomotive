#include "BulletInfo.h"


BulletInfo::BulletInfo(void) : gravitationalforce(-9.82f)
{
	b_Status = false;
	position = 0.f;
	direction = 0.f;
	speed = 0.f;
	lifetime = 0.f;
	damage = 0.f;
}


BulletInfo::~BulletInfo(void)
{

}

void BulletInfo::Init(Vector3 pos, Vector3 Dir, float speed, float lifetime, float dmg)
{
	setStatus(true);
	setPosition(pos);
	setDirection(Dir);
	setSpeed(speed);
	setLife(lifetime);
	damage = dmg;
}

void BulletInfo::Update(double dt)
{
	if(b_Status)
	{
		position += direction * static_cast<float>(dt) * speed;
		lifetime -= static_cast<float>(dt);

		if(lifetime < 0.f)
		{
			b_Status = false;
		}
	}
}

Vector3 BulletInfo::getPosition(void)
{
	return position;
}

void BulletInfo::setPosition(Vector3 pos)
{
	position = pos;
}

Vector3 BulletInfo::getDirection(void)
{
	return direction;
}

void BulletInfo::setDirection(Vector3 Dir)
{
	direction = Dir;
}

float BulletInfo::getLife(void)
{
	return lifetime;
}

void BulletInfo::setLife(float L)
{
	lifetime = L;
}

bool BulletInfo::getStatus(void)
{
	return b_Status;
}

void BulletInfo::setStatus(bool S)
{
	b_Status = S;
}

float BulletInfo::getSpeed(void)
{
	return speed;
}

void BulletInfo::setSpeed(float S)
{
	speed = S;
}