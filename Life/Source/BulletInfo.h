#pragma once

#include "Vector3.h"
class BulletInfo
{
private:
	bool b_Status;
	Vector3 position;
	Vector3 direction;
	const float gravitationalforce;

	float speed;
	float lifetime;

public:
	BulletInfo(void);
	~BulletInfo(void);

	float verticalvelocity;
	float damage;

	void Init(Vector3 pos, Vector3 Dir, float speed, float lifetime, float dmg);
	void Update(double dt);

	Vector3 getPosition(void);
	void setPosition(Vector3 pos);

	Vector3 getDirection(void);
	void setDirection(Vector3 Dir);

	float getLife(void);
	void setLife(float L);

	bool getStatus(void);
	void setStatus(bool S);

	float getSpeed(void);
	void setSpeed(float S);

};

