/******************************************************************************/
/*!
\file	Anim4.cpp
\author Gabriel Wong Choon Jieh
\par	email: AuraTigital\@gmail.com
\brief
Struct to store arms and legs rotation
*/
/******************************************************************************/
#include <cmath>
#include "Anim4.h"

/******************************************************************************/
/*!
\brief
Anim4 default constructor

\param	a
Left Arm
\param	b
Right Arm
\param	c
Left Leg
\param	d
Right Leg
*/
/******************************************************************************/
Anim4::Anim4(float a, float b, float c, float d) : LEFT_ARM(a), RIGHT_ARM(b), LEFT_LEG(c), RIGHT_LEG(d), ASTATE(AS_WALK_1)
{

}

/******************************************************************************/
/*!
\brief
Anim4 default destructor
*/
/******************************************************************************/
Anim4::~Anim4()
{

}

/******************************************************************************/
/*!
\brief	Set the position of arms and legs

\param	a
Left Arm
\param	b
Right Arm
\param	c
Left Leg
\param	d
Right Leg
*/
/******************************************************************************/
void Anim4::Set( float a, float b, float c, float d)
{
	LEFT_ARM = a;
	RIGHT_ARM = b;
	LEFT_LEG = c;
	RIGHT_LEG = d;
}

/******************************************************************************/
/*!
\brief
operator+ overload for animation addition

\param rhs
Anim4 to add with
\return 
Resulting Anim4
*/
/******************************************************************************/
Anim4 Anim4::operator+( const Anim4& rhs) const
{
	return Anim4(LEFT_ARM + rhs.LEFT_ARM, RIGHT_ARM + rhs.RIGHT_ARM, LEFT_LEG + rhs.LEFT_LEG, RIGHT_LEG + rhs.RIGHT_LEG);
}

/******************************************************************************/
/*!
\brief
operator+= overload for animation addition and assignment

\param rhs
Anim4 to add with
\return 
Resulting Anim4
*/
/******************************************************************************/
Anim4& Anim4::operator+=( const Anim4& rhs )
{
	LEFT_ARM += rhs.LEFT_ARM;
	RIGHT_ARM += rhs.RIGHT_ARM;
	LEFT_LEG += rhs.LEFT_LEG;
	RIGHT_LEG += rhs.RIGHT_LEG;
	return *this;
}

/******************************************************************************/
/*!
\brief
operator- overload for Anim4 subtraction

\param rhs
Anim4 to subtract with
\return Resulting Anim4
*/
/******************************************************************************/
Anim4 Anim4::operator-( const Anim4& rhs ) const
{
	return Anim4(LEFT_ARM - rhs.LEFT_ARM, RIGHT_ARM - rhs.RIGHT_ARM, LEFT_LEG - rhs.LEFT_LEG, RIGHT_LEG - rhs.RIGHT_LEG);
}

/******************************************************************************/
/*!
\brief
operator-= overload for Anim4 subtraction and assignment

\param rhs
Anim4 to subtract with
\return 
Resulting Anim4
*/
/******************************************************************************/
Anim4& Anim4::operator-=( const Anim4& rhs )
{
	LEFT_ARM -= rhs.LEFT_ARM;
	RIGHT_ARM -= rhs.RIGHT_ARM;
	LEFT_LEG -= rhs.LEFT_LEG;
	RIGHT_LEG -= rhs.RIGHT_LEG;
	return *this;
}

/******************************************************************************/
/*!
\brief
operator- overload for unary negation

\return 
Resulting Anim4
*/
/******************************************************************************/
Anim4 Anim4::operator-( void ) const
{
	return Anim4(-LEFT_ARM, -RIGHT_ARM, -LEFT_LEG, -RIGHT_LEG);
}

/******************************************************************************/
/*!
\brief
update animations
*/
/******************************************************************************/
void Anim4::Update(double dt, float MOVE_SPEED)
{
	switch (ASTATE)
	{
	case AS_WALK_1:
		if (MOVE_SPEED > 0)
		{
			LEFT_LEG += static_cast<float>(dt)*MOVE_SPEED;
			if (LEFT_LEG > 30)
				ASTATE = AS_WALK_2;
		}
		else
		{
			LEFT_LEG -= static_cast<float>(dt)*MOVE_SPEED;
			if (LEFT_LEG > 30)
				ASTATE = AS_WALK_2;
		}
		break;
	case AS_WALK_2:
		if (MOVE_SPEED > 0)
		{
			LEFT_LEG -= static_cast<float>(dt)*MOVE_SPEED;
			if (LEFT_LEG < -30)
				ASTATE = AS_WALK_1;
		}
		else
		{
			LEFT_LEG += static_cast<float>(dt)*MOVE_SPEED;
			if (LEFT_LEG < -30)
				ASTATE = AS_WALK_1;
		}
		break;
	}

	if (MOVE_SPEED < 0.5)
	{
		if (MOVE_SPEED > -0.5)
		{
			LEFT_LEG += (0 - LEFT_LEG) * static_cast<float>(dt) * 5.f;
		}
	}

	RIGHT_LEG = -LEFT_LEG;
}