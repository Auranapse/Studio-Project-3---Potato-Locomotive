#include "Collision.h"
#include <valarray>


Collision::Collision()
{
}


Collision::~Collision()
{
}

bool Collision::raySphere(Vector3 spherePos,float sphereScale, Vector3 rayDirection, Vector3 rayPos)
{
/*{
		double u = (-b - sqrt(test) / (2.0 * a));
		result = rayPos + u * (rayDirection - rayPos);
		return true;
	}*/
	/**/
	double x1 = rayPos.x;
	double y1 = rayPos.y;
	double z1 = rayPos.z;

	double x2 = rayDirection.x;
	double y2 = rayDirection.y;
	double z2 = rayDirection.z;

	double x3 = spherePos.x;
	double y3 = spherePos.y;
	double z3 = spherePos.z;

	double dx = x2 - x1;
	double dy = y2 - y1;
	double dz = z2 - z1;

	double a = dx * dx + dy * dy + dz * dz;
	double b = 2 * (dx * (x1 - x3) + dy * (y1 - y3) + dz * (z1 * z3));
	double c = x3*x3 + y3*y3 + z3*z3 + x1*x1 + y1*y1 + z1*z1 - 2.0 * (x3*x1 + y3*y1 + z3*z1) - 1;

	double test = b*b - 4.0*a*c;

	if (test >= 0)
		return true;
	else
		return false;
}

bool Collision::AABB(Vector3 pos1, Vector3 pos2, Vector3 pos2Max, Vector3 pos2Min)
{
	Vector3 max = pos2 + pos2Max;
	Vector3 min = pos2 - pos2Min;

	if (pos1.x > min.x && pos1.x < max.x && pos1.y > min.y && pos1.y < max.x && pos1.z > min.z && pos1.z < max.x)
		return true;
	else
		return false;

}

bool Collision::SphereSphere(Vector3 pos1, float scale1, Vector3 pos2, float scale2)
{
	float dist = Vector3((pos1 - pos2)).LengthSquared();
	if (dist <= (scale1 + scale2) * (scale1 + scale2))
		return true;
	else
		return false;
}

bool Collision::Length(Vector3 pos1, Vector3 pos2, float range, bool isMore)
{
	if (isMore == true)
	{
		if ((pos1 - pos2).Length() > range)
			return true;
		else
			return false;
	}
	else
	{
		if ((pos1 - pos2).Length() < range)
			return true;
		else
			return false;
	}
}

bool Collision::LengthSquared(Vector3 pos1, Vector3 pos2, float range, bool isMore)
{
	if (isMore == true)
	{
		if ((pos1 - pos2).LengthSquared() > range)
			return true;
		else
			return false;
	}
	else
	{
		if ((pos1 - pos2).LengthSquared() < range)
			return true;
		else
			return false;
	}
}