/****************************************************************************/
/*!
\file GameObject.cpp
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Game Object base class
!*/
/****************************************************************************/
#include "GameObject.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
GameObject::GameObject()
	: scale(1, 1, 1),
	ColBox(0, 0, 0),
	ColBoxOffset(0, 0, 0),
	pos(0, 0, 0),
	active(false),
	enablePhysics(false),
	colEnable(false),
	dynamicRendering(false),
	mass(1.f),
	rotation(0, 0, 0),
	isHeld(false),
	holdable(false),
	gravityEnabled(true),
	mesh(NULL),
	Opacity(100.f)
{

}

/******************************************************************************/
/*!
\brief
Default destructor
*/
/******************************************************************************/
GameObject::~GameObject()
{

}

void GameObject::Update(double &dt)
{

}