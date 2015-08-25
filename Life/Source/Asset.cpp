#include "Asset.h"


/***************************************************************
Constructor/Destructor
****************************************************************/
Asset::Asset(Mesh *mesh, Shape *bound, float mass)
{
	this->mesh = mesh;
	this->bound = bound;
	this->mass = mass;
}

Asset::Asset(){}

Asset::~Asset(){}

/***************************************************************
MUTATOR(S)
****************************************************************/
void Asset::setMesh(Mesh *mesh)
{
	this->mesh = mesh;
}
void Asset::setBound(Shape *bound)
{
	this->bound = bound;
}
void Asset::setMass(float mass)
{
	this->mass = mass;
}

/***************************************************************
ACCESSOR(S)
****************************************************************/
Mesh * Asset::getMesh(void)const
{
	return this->mesh;
}
Shape * Asset::getBound(void)const
{
	return this->bound;
}
float Asset::getMass(void)const
{
	return this->mass;
}
bool Asset::getFixed(void)const
{
	return this->isFixed;
}
int Asset::getType(void)const
{
	return this->type;
}