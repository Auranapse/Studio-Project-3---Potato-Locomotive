#pragma once
#include "Vector3.h"
class Collision
{
public:
	Collision();
	~Collision();

	static bool raySphere(Vector3 spherePos,float sphereScale, Vector3 rayDirection, Vector3 rayPos);
	static bool AABB(Vector3 pos1, Vector3 pos2, Vector3 pos2Max, Vector3 pos2Min);
	static bool SphereSphere(Vector3 pos1, float scale1, Vector3 pos2, float scale2);
	static bool Length(Vector3 pos1, Vector3 pos2, float range, bool isMore = true);
	static bool LengthSquared(Vector3 pos1, Vector3 pos2, float range, bool isMore = true);
};

