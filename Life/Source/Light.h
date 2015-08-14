/****************************************************************************/
/*!
\file Light.h
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Light header file
*/
/****************************************************************************/
#ifndef LIGHT_H
#define LIGHT_H

#include "Vertex.h"

/******************************************************************************/
/*!
		enum LIGHT_TYPE:
\brief defines the types of light source
*/
/******************************************************************************/
enum LIGHT_TYPE
{
	LIGHT_POINT = 0,
	LIGHT_DIRECTIONAL,
	LIGHT_SPOT,
};

/******************************************************************************/
/*!
		struct Light:
\brief defines the properties of the light source
*/
/******************************************************************************/
struct Light
{
/******************************************************************************/
/*!
		Position position:
\brief	Position object to store light position information
*/
/******************************************************************************/
	Position position;

/******************************************************************************/
/*!
		Color color:
\brief	Color object to store the colour of the light
*/
/******************************************************************************/
	Color color;

/******************************************************************************/
/*!
		float power:
\brief	The brightness of the light
*/
/******************************************************************************/
	float power;

/******************************************************************************/
/*!
		float kC:
\brief	Constant for constant attenuation
*/
/******************************************************************************/
	float kC;

/******************************************************************************/
/*!
		float kL:
\brief	Constant for linear attenuation
*/
/******************************************************************************/
	float kL;
/******************************************************************************/
/*!
		float kQ:
\brief	Constant for quadratic attenuation
*/
/******************************************************************************/
	float kQ;
/******************************************************************************/
/*!
		LIGHT_TYPE type
\brief	The type of light to shine
*/
/******************************************************************************/
	LIGHT_TYPE type;
/******************************************************************************/
/*!
		Vector3 spotDirection
\brief	The direction of the spot light
*/
/******************************************************************************/
	Vector3 spotDirection;
/******************************************************************************/
/*!
		float cosCutoff
\brief	The cosine cutoff
*/
/******************************************************************************/
	float cosCutoff;
/******************************************************************************/
/*!
		float cosInner
\brief	The cosine inner
*/
/******************************************************************************/
	float cosInner;
/******************************************************************************/
/*!
		float exponent
\brief	The exponent
*/
/******************************************************************************/
	float exponent;
/******************************************************************************/
/*!
\brief	Default constructor
*/
/******************************************************************************/
	Light()
	{

	}
};



#endif