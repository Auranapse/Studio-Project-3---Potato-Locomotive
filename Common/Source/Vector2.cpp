/******************************************************************************/
/*!
\file	Vector2.cpp
\author Gabriel Wong Choon Jieh
\par	email: AuraTigital\@gmail.com
\brief
Struct to define a 2D vector
*/
/******************************************************************************/
#include "Vector2.h"
#include <cmath>

/******************************************************************************/
/*!
\brief Vector2 constructor

\param	a
	x value
\param	b
	y value
*/
/******************************************************************************/
Vector2::Vector2(float a, float b) : x(a), y(b)
{

}

/******************************************************************************/
/*!
\brief Vector2 copy constructor

\param	&rhs
Copy that vector
*/
/******************************************************************************/
Vector2::Vector2( const Vector2 &rhs ) : x(rhs.x), y(rhs.y)
{

}

/******************************************************************************/
/*!
\brief set Vector2 X and Y values

\param	a
	x value
\param	b
	y value
*/
/******************************************************************************/
void Vector2::Set(float a, float b)
{
	x = a;
	y = b;
}

/******************************************************************************/
/*!
\brief Handles addition between 2 vectors

\param	rhs
Vector to add with
*/
/******************************************************************************/
Vector2 Vector2::operator+( const Vector2& rhs ) const
{
	return Vector2(x + rhs.x, y + rhs.y);
}

/******************************************************************************/
/*!
\brief Handles subtraction between 2 vectors

\param	rhs
Vector to subtract with
*/
/******************************************************************************/
Vector2 Vector2::operator-( const Vector2& rhs ) const
{
	return Vector2(x - rhs.x, y - rhs.y);
}

/******************************************************************************/
/*!
\brief Multiplies -1 to X and Y
*/
/******************************************************************************/
Vector2 Vector2::operator-( void ) const
{
	return Vector2(-x, -y);
}

/******************************************************************************/
/*!
\brief Multiplies Vector with value

\param	scalar
Multiplies X and Y axis with scalar value
*/
/******************************************************************************/
Vector2 Vector2::operator*( float scalar ) const
{
	return Vector2(scalar * x, scalar * y);
}

/******************************************************************************/
/*!
\brief Calculates length
*/
/******************************************************************************/
float Vector2::Length( void ) const
{
	return sqrt(x * x + y * y);
}

/******************************************************************************/
/*!
\brief Calculate dot product of two vectors

\param	rhs
Vector calculate dot product with
*/
/******************************************************************************/
float Vector2::Dot( const Vector2& rhs ) const
{
	return (x * rhs.x + y * rhs.y);
}

/******************************************************************************/
/*!
\brief Normalises the vector
*/
/******************************************************************************/
Vector2 Vector2::Normalized( void )
{
	float d = Length();
	return Vector2(x / d, y / d);
}
