#ifndef BULLETOBJECT_H
#define BULLETOBJECT_H

#include "GameObject.h"

class BulletObject : public GameObject
{
public:
	BulletObject();
	virtual ~BulletObject();

	float life;
};
#endif
