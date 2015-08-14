#ifndef GAMEOBJECTTRIGGER_H
#define GAMEOBJECTTRIGGER_H

#include "GameObject.h"

class GameObjectTrigger : public GameObject
{
public:
	enum Triggers
	{
		T_LASER,
		T_GRILL,
	};

	Triggers TRIGGER_TYPE;
	char ID;
	bool triggerActive;

	GameObjectTrigger();
	~GameObjectTrigger();
};

#endif
