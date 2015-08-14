
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	scale(1, 1, 1),
	ColBox(0, 0, 0),
	ColBoxOffset(0, 0, 0),
	pos(0, 0, 0),
	active(false),
	enablePhysics(false),
	colEnable(false),
	dynamicRendering(false),
	portalble(false),
	mass(1.f),
	rotationX(0.f),
	rotationY(0.f),
	rotationZ(0.f),
	rotationalVelocity(0.f)
{
	mesh = NULL;
}

GameObject::~GameObject()
{

}