#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include "GabrielDLC.h"

class CollisionBox
{
public:
	enum COLLISION_TYPE
	{
		CT_AABB,
		CT_SPHERE,
		CT_POINT,
		CT_RAY,
		CT_PLANE,
	};

	COLLISION_TYPE Type;
	Vector3 Position;

	//For AABB
	Vector3 ColBox;
	Vector3 ColOffset;

	//For Sphere
	float radius;

	//For Ray(Smit's Method)
	Vector3 Direction;//Normalized
	Vector3 end;//Another Point on Ray(Preferably the end of the ray)
	float t1, t2; //Intersection Interval(Length before Ray/AABB can be tested again, t1 = Min, t2 = Max)
	
	//For Plane
	Vector3 planeNormal;//Normalized
	float offset;//How far it is from the origin in the direction of normal

	/*Finding Normal Example
	v1,v2,v0 = Any Points of Plane
		Vector3 vecAB = v1 - v0;
		Vector3 vecAC = v2 - v0;
 
		// Cross vecAB and vecAC
		Vector3 normal = vecAB.Cross(vecAC);
		normal.normalize();

	To Find offset...
	Vector3 offset = planeNormal.Dot(anyPointOnPlane);
	*/

	CollisionBox();
	~CollisionBox();

	bool checkCollision(CollisionBox &CB1, CollisionBox &CB2);

private:
	//AABB Collision(s)
	bool AABB_AABB(CollisionBox &CB1, CollisionBox &CB2);
	bool AABB_SPHERE(CollisionBox &CB1, CollisionBox &CB2);
	bool AABB_POINT(CollisionBox &CB1, CollisionBox &CB2);
	bool AABB_RAY(CollisionBox &CB1, CollisionBox &CB2);
	bool AABB_PLANE(CollisionBox &CB1, CollisionBox &CB2);

	//Sphere Collision(s)
	bool SPHERE_SPHERE(CollisionBox &CB1, CollisionBox &CB2);
	bool SPHERE_POINT(CollisionBox &CB1, CollisionBox &CB2);
	bool SPHERE_RAY(CollisionBox &CB1, CollisionBox &CB2);//FORGOT!!
	bool SPHERE_PLANE(CollisionBox &CB1, CollisionBox &CB2);
	
	bool POINT_POINT(CollisionBox &CB1, CollisionBox &CB2);
	bool POINT_RAY(CollisionBox &CB1, CollisionBox &CB2);
	bool POINT_PLANE(CollisionBox &CB1, CollisionBox &CB2);

	//Ray Collision(s)
	bool RAY_RAY(CollisionBox &CB1, CollisionBox &CB2);//FORGOT!!
	bool RAY_PLANE(CollisionBox &CB1, CollisionBox &CB2);

	//Plane Collision(s)
	bool PLANE_PLANE(CollisionBox &CB1, CollisionBox &CB2);//FORGOT!!
};

#endif
