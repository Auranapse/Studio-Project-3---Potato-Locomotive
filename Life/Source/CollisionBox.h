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
	Vector3 ColBox;
	Vector3 ColOffset;

	CollisionBox();
	~CollisionBox();

	bool checkCollision(CollisionBox &CB1, CollisionBox &CB2);

private:
	//AABB Collision(s)
	bool AABB_AABB(CollisionBox &CB1, CollisionBox &CB2);
	bool AABB_SPHERE(CollisionBox &CB1, CollisionBox &CB2);
	bool AABB_POINT(CollisionBox &CB1, CollisionBox &CB2);
	bool AABB_RAY(CollisionBox &CB1, CollisionBox &CB2);//FORGOT!!
	bool AABB_PLANE(CollisionBox &CB1, CollisionBox &CB2);//FORGOT!!

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
