#include "Collision.h"
#include <valarray>


Collision::Collision()
{
}


Collision::~Collision()
{
}

bool Collision::raySphere(Vector3 spherePos,float sphereScale, Vector3 rayDirection, Vector3 rayPos, Vector3 result)
{
	//float xc,float yc,float zc,			float xd,float yd,float zd,			float xs,float ys,float zs
	/*float xc = spherePos.x;
	float yc = spherePos.y;
	float zc = spherePos.z;

	float xd = rayDirection.x;
	float yd = rayDirection.y;
	float zd = rayDirection.z;

	float xs = rayPos.x;
	float ys = rayPos.y;
	float zs = rayPos.z;

	float b = 2 * (xd*(xs - xc) + yd*(ys - yc) + zd*(zs - zc));
	float c = xs*xs - 2 * xs*xc + xc*xc + ys*ys - 2 * ys*yc + yc*yc + zs*zs - 2 * zs*zc + zc*zc - sphereScale * sphereScale;
	float disc = (b*b - 4 * c);

	if (disc < 0)
		return false;
	else
		return true;*/

	float x1 = rayPos.x;
	float y1 = rayPos.y;
	float z1 = rayPos.z;

	float x2 = rayDirection.x;
	float y2 = rayDirection.y;
	float z2 = rayDirection.z;

	float x3 = spherePos.x;
	float y3 = spherePos.y;
	float z3 = spherePos.z;

	float dx = x2 - x1;
	float dy = y2 - y1;
	float dz = z2 - z1;

	float a = dx * dx + dy * dy + dz * dz;
	float b = 2 * (dx * (x1 - x3) + dy * (y1 - y3) + dz * (z1 * z3));
	float c = x3*x3 + y3*y3 + z3*z3 + x1*x1 + y1*y1 + z1*z1 - 2.0 * (x3*x1 + y3*y1 + z3*z1) - sphereScale*sphereScale;

	float test = b*b - 4.0*a*c;

	if (test >= 0)
	{
		float u = (-b - sqrt(test) / (2.0 * a));
		result = rayPos + u * (rayDirection - rayPos);
		return true;
	}
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