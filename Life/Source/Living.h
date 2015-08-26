/******************************************************************************/
/*!
\file	Living.h
\author Quen Ryu Guile
\par	email: ryufiguracion@yahoo.com.sg
\brief
Class to define Living Assets in the Scene
*/
/******************************************************************************/
#include "Asset.h"

#pragma once
/******************************************************************************/
/*!
		Class Living:
\brief	Defines Living Assets in the Scene
*/
/******************************************************************************/
class Living : public Asset
{
public:
	Living(Mesh *mesh, Shape *bound, float mass,
			int health, Vector3 velo, 
			Vector3 acc, Vector3 force) 
			: Asset(mesh, bound, mass)
	{
		this->velo = velo;
		this->acc = acc;
		this->force = force;
		this->health = health;
		this->onMove = false;
		this->isFixed = false;
	}
	Living();
	~Living();

	virtual void update(double dt, float speed) = 0;
	void applyForce(Vector3);

	//Accessors
	Vector3 getVelo(void)const;
	Vector3 getAcc(void)const;
	Vector3 getForce(void)const;
	int getHealth(void)const;
	bool getMove(void)const;

	//Mutators
	void setVelo(Vector3);
	void setAcc(Vector3);
	void setForce(Vector3);
	void setHealth(int);
	void setMove(bool);

protected:
	Vector3 velo;
	Vector3 acc;
	Vector3 force;
	int health;

private:
	bool onMove;
};