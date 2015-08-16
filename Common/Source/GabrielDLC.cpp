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
bool intersect2D(Vector3 &TopLeft, Vector3 &BottomRight, Vector3 &Position)
{
	if(Position.x <= TopLeft.x && Position.x >= BottomRight.x)
	{
		if(Position.y <= TopLeft.y && Position.y >= BottomRight.y)
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
bool intersect(Vector3 &TopLeft, Vector3 &BottomRight, Vector3 &Position)
{
	if(Position.x <= TopLeft.x && Position.x >= BottomRight.x)
	{
		if(Position.y <= TopLeft.y && Position.y >= BottomRight.y)
		{
			if(Position.z <= TopLeft.z && Position.z >= BottomRight.z)
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
	if(XZ == true)
	{
		return Math::RadianToDegree(atan2(Target.x - Origin.x, Target.z - Origin.z));
	}
	else
	{	
		float O = Target.y - Origin.y;

		Target.y = Origin.y = 0;

		float A = (Target - Origin).Length();
		return (atan(O/A) * 180 / Math::PI);
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
float CalAnglefromPosition2D(Vector3 &Target, Vector3 &Origin)
{
	return Math::RadianToDegree(atan2(Target.y - Origin.y, Target.x - Origin.x));
}