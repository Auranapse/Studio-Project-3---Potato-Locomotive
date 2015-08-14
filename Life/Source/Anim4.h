/******************************************************************************/
/*!
\file	Anim4.h
\author Gabriel Wong Choon Jieh
\par	email: AuraTigital\@gmail.com
\brief
Struct to store arms and legs rotation
*/
/******************************************************************************/

#ifndef ANIM4_H
#define ANIM4_H

#include "MyMath.h"
#include <iostream>

/******************************************************************************/
/*!
Struct Anim4:
\brief	stores values of the player arms and legs rotation
*/
/******************************************************************************/
struct Anim4
{

	enum ANIM_STATE
	{
		AS_WALK_1,
		AS_WALK_2,
	};

	ANIM_STATE ASTATE;

/******************************************************************************/
/*!
		LEFT_ARM
\brief	Controls rotation of the left arm
*/
/******************************************************************************/
	float LEFT_ARM;

/******************************************************************************/
/*!
		RIGHT_ARM
\brief	Controls rotation of the right arm
*/
/******************************************************************************/
	float RIGHT_ARM;

/******************************************************************************/
/*!
		LEFT_LEG
\brief	Controls rotation of the left leg
*/
/******************************************************************************/	
	float LEFT_LEG;

/******************************************************************************/
/*!
		RIGHT_LEG
\brief	Controls rotation of the right leg
*/
/******************************************************************************/	
	float RIGHT_LEG;

	Anim4(float a = 0.f, float b = 0.f, float c = 0.f, float d = 0.f);

	~Anim4();

	void Set( float a = 0.f, float b = 0.f, float c = 0.f, float d = 0.f); //Set all data

	Anim4 operator+( const Anim4& rhs ) const; //addition
	Anim4& operator+=( const Anim4& rhs ); 

	Anim4 operator-( const Anim4& rhs ) const; //subtraction
	Anim4& operator-=( const Anim4& rhs );

	Anim4 operator-( void ) const; //Unary negation

	void Update(double dt, float MOVE_SPEED);
};

#endif
