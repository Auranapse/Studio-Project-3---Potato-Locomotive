#include "CollisionBox.h"

CollisionBox::CollisionBox() : ColOffset(0.f, 0.f, 0.f), Type(CT_POINT)
{
	
}

CollisionBox::~CollisionBox()
{

}

bool CollisionBox::checkCollision(CollisionBox &CB1, CollisionBox &CB2)
{
	//Swaps it so that c1 always has lower type than c2
	int c1, c2;//c1 = Lower Type, C2 = Higher Type
	c1 = (CB1.Type < CB2.Type) ? (c1 = CB1.Type) :(c1 = CB2.Type);
	c2 = (CB2.Type > CB1.Type) ? (c2 = CB2.Type) :(c2 = CB1.Type);

	switch (c1)
	{
	default:
	case 1:
		{
			switch (c2)
			{
			default:
			case 0://AABB to AABB Collision
				{
					return AABB_AABB(CB1, CB2);
					break;
				}
			case 1://AABB to Sphere Collision
				{
					return AABB_SPHERE(CB1, CB2);
					break;
				}
			case 2://AABB to Point Collision
				{
					return AABB_POINT(CB1, CB2);
					break;
				}
			case 3://AABB to Ray Collision
				{
					return AABB_RAY(CB1, CB2);
					break;
				}
			case 4://AABB to Plane Collision
				{
					return AABB_PLANE(CB1, CB2);
					break;
				}
			}
			break;
		}
	case 2:
		{
			switch (c2)
			{
			default:
			case 1://Sphere To Sphere Collision
				{
					return SPHERE_SPHERE(CB1, CB2);
					break;
				}
			case 2://Sphere To Point Collision
				{
					return SPHERE_POINT(CB1, CB2);
					break;
				}
			case 3://Sphere To Ray Collision
				{
					return SPHERE_RAY(CB1, CB2);
					break;
				}
			case 4://Sphere To Plane Collision
				{
					return SPHERE_PLANE(CB1, CB2);
					break;
				}
			}
			break;
		}
	case 3:
		{
			switch (c2)
			{
			default:
			case 2://Point To Point Collision
				{
					return POINT_POINT(CB1, CB2);
					break;
				}
			case 3://Point To Ray Collision
				{
					return POINT_RAY(CB1, CB2);
					break;
				}
			case 4://Point To Plane Collision
				{
					return POINT_PLANE(CB1, CB2);
					break;
				}
			}
			break;
		}
	case 4:
		{
			switch (c2)
			{
			default:
			case 3://Point To Ray Collision
				{
					return RAY_RAY(CB1, CB2);
					break;
				}
			case 4://Point To Plane Collision
				{
					return RAY_PLANE(CB1, CB2);
					break;
				}
			}
			break;
		}
		case 5:
		{
			switch (c2)
			{
			default:
			case 4://Point To Plane Collision
				{
					return PLANE_PLANE(CB1, CB2);
					break;
				}
			}
			break;
		}
	}
	return false;
}


/*******************************************************************************
******************************AABB COLLISION(S)*********************************
********************************************************************************/
bool CollisionBox::AABB_AABB(CollisionBox &CB1, CollisionBox &CB2)
{
	Vector3 topLeft = CB1.Position + CB1.ColBox + CB2.ColBox;
	Vector3 bottomRight = CB1.Position - CB1.ColBox - CB2.ColBox;
	if (intersect(topLeft, bottomRight, CB2.Position))
	{
		return true;
	}

	return false;
}

bool CollisionBox::AABB_SPHERE(CollisionBox &CB1, CollisionBox &CB2)
{
	float radius = CB2.ColBox.x;
	Vector3 topRight = CB1.Position + CB1.ColBox;
	Vector3 bottomLeft = CB1.Position - CB1.ColBox;
	float DistanceBetween = 0;
	float Temp;

	if (CB2.Position.x < bottomLeft.x)
	{
		Temp = CB2.Position.x - bottomLeft.x;
		DistanceBetween += Temp*Temp;
	}
	else if (CB2.Position.x > topRight.x)
	{
		Temp = CB2.Position.x - topRight.x;
		DistanceBetween += Temp*Temp;
	}


	if (CB2.Position.z < bottomLeft.z)
	{
		Temp = CB2.Position.z - bottomLeft.z;
		DistanceBetween += Temp*Temp;
	}
	else if (CB2.Position.z > topRight.z){
		Temp = CB2.Position.z - topRight.z;
		DistanceBetween += Temp*Temp;
	}

	if (CB2.Position.y < bottomLeft.y)
	{
		Temp = CB2.Position.y - bottomLeft.y;
		DistanceBetween += Temp*Temp;
	}
	else if (CB2.Position.y > topRight.y)
	{
		Temp = CB2.Position.y - topRight.y;
		DistanceBetween += Temp*Temp;
	}

	return DistanceBetween <= (radius * radius);
}

bool CollisionBox::AABB_POINT(CollisionBox &CB1, CollisionBox &CB2)
{
	return (intersect((CB1.Position + CB1.ColBox + CB1.ColOffset), (CB1.Position - CB1.ColBox + CB1.ColOffset), CB2.Position));
}

bool CollisionBox::AABB_RAY(CollisionBox &CB1, CollisionBox &CB2)
{
	Vector3 topRight = CB1.Position + CB1.ColBox;
	Vector3 bottomLeft = CB1.Position - CB1.ColBox;
	float xmin, xmax, ymin, ymax, zmin, zmax;
	if (CB2.Direction.x >= 0)
	{
		xmin = (bottomLeft.x - CB2.Position.x) / CB2.Direction.x;
		xmax = (topRight.x - CB2.Position.x) / CB2.Direction.x;
	}
	else
	{
		xmin = (topRight.x - CB2.Position.x) / CB2.Direction.x;
		xmax = (bottomLeft.x - CB2.Position.x) / CB2.Direction.x;
	}

	if (CB2.Direction.y >= 0)
	{
		ymin = (bottomLeft.y - CB2.Position.y) / CB2.Direction.y;
		ymax = (topRight.y - CB2.Position.y) / CB2.Direction.y;
	}
	else
	{
		ymin = (topRight.y - CB2.Position.y) / CB2.Direction.y;
		ymax = (bottomLeft.y - CB2.Position.y) / CB2.Direction.y;
	}

	if ((xmin > ymax) || (ymin > xmax))
		return false;
	if (ymin > xmin)
		xmin = ymin;
	if (ymax < xmax)
		xmax = ymax;
	if (CB2.Direction.z >= 0)
	{
		zmin = (bottomLeft.z - CB2.Position.z) / CB2.Direction.z;
		zmax = (topRight.z - CB2.Position.z) / CB2.Direction.z;
	}
	else
	{
		zmin = (topRight.z - CB2.Position.z) / CB2.Direction.z;
		zmax = (bottomLeft.z - CB2.Position.z) / CB2.Direction.z;
	}

	if ((xmin > zmax) || (zmin > xmax))
		return false;
	if (zmin > xmin)
		xmin = zmin;
	if (zmax < xmax)
		xmax = zmax;
	return ((xmin < CB2.t1) && (xmax > CB2.t2));
}

bool CollisionBox::AABB_PLANE(CollisionBox &CB1, CollisionBox &CB2)
{
	Vector3 topRight = CB1.Position + CB1.ColBox;
	Vector3 bottomLeft = CB1.Position - CB1.ColBox;
	Vector3 v1 = (topRight - bottomLeft) * 0.5f;
	
	float rad = abs(CB2.planeNormal.x * v1.x) + abs(CB2.planeNormal.y * v1.y) + abs(CB2.planeNormal.z * v1.z);

	CB1.radius = rad;
	return SPHERE_PLANE(CB1, CB2);
}

/*******************************************************************************
****************************SPHERE COLLISION(S)*********************************
********************************************************************************/
bool CollisionBox::SPHERE_SPHERE(CollisionBox &CB1, CollisionBox &CB2)
{
	float radius1 = CB1.ColBox.x;
	float radius2 = CB2.ColBox.x;

	//Distance Between Their Centres
	float d = pow((CB1.Position.x - CB2.Position.x), 2)
		+ pow((CB1.Position.y - CB2.Position.y), 2)
		+ pow((CB1.Position.z - CB2.Position.z), 2);

	//Combined Radius of Two Bounding Spheres
	float tR = pow((radius1 + radius2), 2);

	if (d <= tR)
		return true;
	else
		return false;
}

bool CollisionBox::SPHERE_POINT(CollisionBox &CB1, CollisionBox &CB2)
{
	float radius1 = CB1.ColBox.x;

	//Distance Between Their Centres
	float d = pow((CB1.Position.x - CB2.Position.x), 2)
		+ pow((CB1.Position.y - CB2.Position.y), 2)
		+ pow((CB1.Position.z - CB2.Position.z), 2);

	//Combined Radius of Two Bounding Spheres
	float tR = pow(radius1, 2);

	if (d <= tR)
		return true;
	else
		return false;
}

bool CollisionBox::SPHERE_RAY(CollisionBox &CB1, CollisionBox &CB2)
{
	return false;
}

bool CollisionBox::SPHERE_PLANE(CollisionBox &CB1, CollisionBox &CB2)
{
	float dot = CB2.planeNormal.Dot(CB1.Position);

	if (dot > CB1.radius)
		return false;
	else if (dot < -CB1.radius)
		return false;
	return true;
}

/*******************************************************************************
*****************************POINT COLLISION(S)*********************************
********************************************************************************/
bool CollisionBox::POINT_POINT(CollisionBox &CB1, CollisionBox &CB2)
{ 
	if (CB1.Position == CB2.Position)
		return true;
	else
		return false;
}

bool CollisionBox::POINT_RAY(CollisionBox &CB1, CollisionBox &CB2)
{
	return false;
}

bool CollisionBox::POINT_PLANE(CollisionBox &CB1, CollisionBox &CB2)
{
	Vector3 pointOnPlane = CB2.Position + CB2.ColBox;
	float d = minDistance(CB1.Position, CB2.Position, pointOnPlane);

	if (d <= 0)
		return true;
	else
		return false;
}

/*******************************************************************************
*******************************RAY COLLISION(S)*********************************
********************************************************************************/
bool CollisionBox::RAY_RAY(CollisionBox &CB1, CollisionBox &CB2)
{
	return false;
}

bool CollisionBox::RAY_PLANE(CollisionBox &CB1, CollisionBox &CB2)
{
	Vector3 ray = CB1.Position - CB1.end;
	if (ray.LengthSquared() == 0)
		return false;
	float dot1 = CB2.planeNormal.Dot(CB1.Position);
	float dot2 = CB2.planeNormal.Dot(ray);

	if (dot1 == 0 || dot2 == 0)
		return false;
	Vector3 intersection = CB1.end + (((CB2.offset - dot1) / dot2) * ray);
	if (intersection.LengthSquared() == 0)
		return false;
	return true;
}

/*******************************************************************************
*****************************PLANE COLLISION(S)*********************************
********************************************************************************/
bool CollisionBox::PLANE_PLANE(CollisionBox &CB1, CollisionBox &CB2)
{
	return false;
}