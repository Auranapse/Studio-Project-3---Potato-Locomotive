#ifndef PORTAL_H
#define PORTAL_H

#include "GameObject.h"
#include "Mtx44.h"
#include "GabrielDLC.h"

class Portal
{
private:

public:
	Portal();
	~Portal();

	GameObject *BluePortal;
	GameObject *OrangePortal;

	GameObject *BP_AttachedTo;
	GameObject *OP_AttachedTo;

	bool pVelChange(Vector3 &Vel, bool o2b);
	void enterPortal(GameObject *go, bool o2b);
	void update(double dt);
};

#endif