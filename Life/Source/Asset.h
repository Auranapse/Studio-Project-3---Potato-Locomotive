#include "Shape.h"
#include "Mesh.h"

#pragma once
/******************************************************************************/
/*!
Class Asset:
\brief	Abstract Base Class of Assets placed in the Scene
*/
/******************************************************************************/
class Asset
{
public:
	Asset(Mesh *mesh, Shape *bound, float mass);
	Asset();
	virtual ~Asset() = 0;


	//Accessors
	Mesh * getMesh(void)const;
	Shape * getBound(void)const;
	float getMass(void)const;
	bool getFixed(void)const;
	int getType(void)const;

	//Mutators
	void setMesh(Mesh *mesh);
	void setBound(Shape *bound);
	void setMass(float);

protected:
	Shape *bound;
	Mesh *mesh;
	float mass;
	bool isFixed;
	int type;
	/*
	0 = Player
	1 = Enemy
	2 = Projectile
	3 = Room
	4 = SoundRange
	*/
	//type = ONLY SET IN CHILD CONSTRUCTORS(No Mutator)
};