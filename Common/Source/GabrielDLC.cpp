#include "GabrielDLC.h"

/******************************************************************************/
/*!
\brief
Check if position intersects with another in 2D
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
*/
/******************************************************************************/
bool intersect(Vector3 &TopLeft, Vector3 &BottomRight, Vector3 &PosTCF)
{
	if(PosTCF.x <= TopLeft.x && PosTCF.x >= BottomRight.x)
	{
		if(PosTCF.y <= TopLeft.y && PosTCF.y >= BottomRight.y)
		{
			if(PosTCF.z <= TopLeft.z && PosTCF.z >= BottomRight.z)
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
*/
/******************************************************************************/
float CalAnglefromPosition(Vector3 Target, Vector3 Origin, bool XZ)
{
	if(XZ == true)
	{
		float deltaX = Target.x - Origin.x;
		float deltaZ = Target.z - Origin.z;
		return (atan2(deltaX, deltaZ) * 180 / Math::PI);
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
Calculates angle between vectors using their position
*/
/******************************************************************************/
float CalAnglefromPosition2D(Vector3 &Target, Vector3 &Origin)
{
	return Math::RadianToDegree(atan2(Target.y - Origin.y, Target.x - Origin.x));
}