/****************************************************************************/
/*!
\file PressurePlate.h
\author Quen Ryu Guile 
\par email: ryufiguracion@yahoo.com
\brief
Pressure Plate to give Effect at an Area for Player
!*/
/****************************************************************************/
#include "CollisionBox.h"
#include "Player.h"
#include <string>
#include <vector>
#pragma once

class PressurePlate
{
public:
	//This gon be inefficient ya'll! YOLO!!!
	PressurePlate(CollisionBox&, double);
	~PressurePlate();

	std::string inEffect(CharacterObject*, int type);
	std::string effect(CharacterObject*, int type);

	//Accessors
	CollisionBox getAOE(void)const;
	double getTimer(void)const;
	std::vector<std::string> getDialogue(void)const;

	//Mutators
	void setAOE(CollisionBox&);
	void setTimer(double);
	void setDialogue(std::vector<std::string>, std::vector<double> Sequence);
private:
	CollisionBox AOE;//Area of Effect LOL
	double timer;
	std::vector <std::string> Dialogue;
	std::vector <double> Sequence;
};