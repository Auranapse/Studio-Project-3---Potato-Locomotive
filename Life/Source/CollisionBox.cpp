#include "CollisionBox.h"

CollisionBox::CollisionBox()
{

}

CollisionBox::~CollisionBox()
{

}

bool CollisionBox::checkCollision(CollisionBox &CB1, CollisionBox &CB2)
{
	if (CB1.Type != CT_AABB)
	{
		CollisionBox *CB3 = &CB2;
		CB2 = CB1;
		CB1 = *CB3;
	}

	switch (CB1.Type)
	{
	default:
	case CT_AABB:
	{
		switch (CB2.Type)
		{
		default:
		case CT_AABB:
		{
			if (intersect((CB1.Position + CB1.ColBox + CB2.ColBox), (CB1.Position - CB1.ColBox - CB2.ColBox), CB2.Position))
			{
				return true;
			}
			break;
		}//case CT_AABB
		case CT_POINT:
		{
			if (intersect((CB1.Position + CB1.ColBox + CB1.ColOffset), (CB1.Position - CB1.ColBox + CB1.ColOffset), CB2.Position))
			{
				return true;
			}
			break;
		}
		}//CB2.Type SWITCH
		break;
	}//case CT_AABB
	}

	return false;
}