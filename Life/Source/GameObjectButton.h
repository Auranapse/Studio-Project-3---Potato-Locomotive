#ifndef GAMEOBJECTBUTTON_H
#define GAMEOBJECTBUTTON_H

#include "GameObject.h"

class GameObjectButton : public GameObject
{
public:
	GameObjectButton();
	~GameObjectButton();

	char ID;
	bool Triggered;
};

#endif