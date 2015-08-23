/****************************************************************************/
/*!
\file Button.h
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
menu button
!*/
/****************************************************************************/
#ifndef BUTTON_H
#define BUTTON_H

#include "Vector3.h"
#include "Mesh.h"

struct Button
{
	Button();
	~Button();

	Vector3 Position;
	Vector3 Scale;
	int gamestate;

	Mesh *mesh_off;
	Mesh *mesh_on;
};

#endif