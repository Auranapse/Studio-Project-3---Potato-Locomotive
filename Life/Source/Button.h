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
#include "Vertex.h"

struct Button
{
	enum E_LABEL_TYPE
	{
		LT_NONE,
		LT_PRE,
		LT_BUTTON,
	};
	Button();
	~Button();

	bool active;

	Color color;
	E_LABEL_TYPE labeltype;
	std::string label;

	Vector3 Position;
	Vector3 Scale;
	int gamestate;
	int ID;

	Mesh *mesh;
};

#endif