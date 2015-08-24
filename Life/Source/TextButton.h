/****************************************************************************/
/*!
\file TextButton.h
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
menu button for text
!*/
/****************************************************************************/
#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include "Vector3.h"
#include "Vertex.h"
#include <string>
#include <vector>

struct TextButton
{
	TextButton()
	{
		pos.SetZero();
		scale.Set(1.f, 1.f, 1.f);
		color.Set(1.f, 1.f, 1.f);
		active = false;
	}

	bool active;
	Vector3 pos;
	Vector3 scale;
	std::string text;
	Color color;
	int gamestate;
};

#endif