#ifndef BUTTON_H
#define BUTTON_H

#include "Vector3.h"
#include "Vertex.h"

struct S_BUTTON
{
	S_BUTTON()
	{
		pos.SetZero();
		scale.Set(1.f, 1.f, 1.f);
		color.Set(1.f, 1.f, 1.f);
		active = false;
		menubypass = false;
	}

	bool active;
	bool menubypass;
	Vector3 pos;
	Vector3 scale;
	std::string text;
	Color color;
	int gamestate;
};

#endif