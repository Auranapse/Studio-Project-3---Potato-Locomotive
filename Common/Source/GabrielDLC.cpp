/****************************************************************************/
/*!
\file GabrielDLC.cpp
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Useful functions that I use often in my work
!*/
/****************************************************************************/
#include "GabrielDLC.h"

/******************************************************************************/
/*!
\brief
Check if position intersects with another in 2D
\param TopLeft
top left of box
\param BottomRight
bottom right of box
\param Position
the position to check if it's inside the box
*/
/******************************************************************************/
bool intersect2D(const Vector3 &TopLeft, const Vector3 &BottomRight, const Vector3 &Position)
{
	if (Position.x <= TopLeft.x && Position.x >= BottomRight.x)
	{
		if (Position.y <= TopLeft.y && Position.y >= BottomRight.y)
		{
			return true;
		}
	}
	return false;
}

/******************************************************************************/
/*!
\brief
Check if position intersects with another in 3D
\param TopLeft
top left of box
\param BottomRight
bottom right of box
\param Position
the position to check if it's inside the box
*/
/******************************************************************************/
bool intersect(const Vector3 &TopLeft, const Vector3 &BottomRight, const Vector3 &Position)
{
	if (Position.x <= TopLeft.x && Position.x >= BottomRight.x)
	{
		if (Position.y <= TopLeft.y && Position.y >= BottomRight.y)
		{
			if (Position.z <= TopLeft.z && Position.z >= BottomRight.z)
			{
				return true;
			}
		}
	}
	return false;
}

/******************************************************************************/
/*!
\brief
Check shortest distance from point to a line/plane
\param point
point away from plane
\param pointPlane1
point in Plane
\param pointPlane2
another point in Plane
*/
/******************************************************************************/
float minDistance(const Vector3 &point, const Vector3 &pointPlane1, const Vector3 &pointPlane2)
{
	return point.Dot(pointPlane1.Cross(pointPlane2));
}

/******************************************************************************/
/*!
\brief
Calculates angle between vectors using their position
\param Target
the target object
\param Origin
the orgin of the angle
\param XZ
if true, checks for yaw rotation, else will check pitch rotation
*/
/******************************************************************************/
float CalAnglefromPosition(Vector3 Target, Vector3 Origin, bool XZ)
{
	if (XZ == true)
	{
		return Math::RadianToDegree(atan2(Target.x - Origin.x, Target.z - Origin.z));
	}
	else
	{
		float O = Target.y - Origin.y;
		//O *= O;
		Target.y = Origin.y = 0;

		//float A = (Target - Origin).LengthSquared();
		float A = (Target - Origin).Length();
		return (atan(O / A) * 180 / Math::PI);
	}
}

/******************************************************************************/
/*!
\brief
Calculates angle between vectors using their position on Z axis
\param Target
the target object
\param Origin
the origin of the angle
*/
/******************************************************************************/
float CalAnglefromPosition2D(const Vector3 &Target, const Vector3 &Origin)
{
	return Math::RadianToDegree(atan2(Target.y - Origin.y, Target.x - Origin.x));
}

/******************************************************************************/
/*!
\brief
Calculates if something is within horizontal FOV
\param Position
the origin to calculate from
\param Target
the position the object is looking at
\param FOV
the field of view of the object
\return
returns true if object is within the horizontal FOV, if not returns false
*/
/******************************************************************************/
bool isVisible(const Vector3 &Position, const Vector3 &Target, float FOV, const Vector3 &ObjectPosition)
{
	float lookingOBJ = CalAnglefromPosition(ObjectPosition, Position, true);
	float cameraRotation = CalAnglefromPosition(Target, Position, true);
	bool LO = false, CR = false;

	if (lookingOBJ < 0.f)
	{
		lookingOBJ += 360;
		LO = true;
	}

	if (cameraRotation < 0.f)
	{
		cameraRotation += 360;
		CR = true;
	}


	if (cameraRotation < 0.f)
	{
		cameraRotation = -cameraRotation;
	}

	if ((lookingOBJ + FOV > cameraRotation && lookingOBJ - FOV < cameraRotation))
	{
		return true;
	}
	else
	{
		if (CR)
		{
			cameraRotation -= 360;
		}
		if (LO)
		{
			lookingOBJ -= 360;
		}
		if ((lookingOBJ + FOV > cameraRotation && lookingOBJ - FOV < cameraRotation))
		{
			return true;
		}
	}

	return false;
}

/******************************************************************************/
/*!
\brief
Calculates if something is within Vertical FOV
\param Position
the origin to calculate from
\param Target
the position the object is looking at
\param FOV
the field of view of the object
\return
returns true if object is within the horizontal FOV, if not returns false
*/
/******************************************************************************/
bool isVisible2(const Vector3 &Position, const Vector3 &Target, float FOV, const Vector3 &ObjectPosition)
{
	float lookingOBJ = CalAnglefromPosition(ObjectPosition, Position, false);
	float cameraRotation = CalAnglefromPosition(Target, Position, false);
	
	if ((lookingOBJ + FOV > cameraRotation && lookingOBJ - FOV < cameraRotation))
	{
		return true;
	}
	else
	{
		if ((lookingOBJ + FOV > cameraRotation && lookingOBJ - FOV < cameraRotation))
		{
			return true;
		}
	}

	return false;
}