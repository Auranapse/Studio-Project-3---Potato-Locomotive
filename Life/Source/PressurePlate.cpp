#include "PressurePlate.h"

/******************************************************************************/
/*!
\brief
Constructor
\param AOE
Area of Effect
\param timer
Time before effect ends(starts when activated)
*/
/******************************************************************************/
PressurePlate::PressurePlate(CollisionBox &AOE, double timer)
{
	this->AOE = AOE;
	this->timer = timer;
}
/******************************************************************************/
/*!
\brief
destructor
*/
/******************************************************************************/
PressurePlate::~PressurePlate()
{

}

/******************************************************************************/
/*!
\brief
Checks if target is under effect and applies it
\target
character going through effect
\type
0 = Speed Change, 1 = Dialogue
*/
/******************************************************************************/
std::string PressurePlate::inEffect(CharacterObject* target, int type)
{
	if (CollisionBox::checkCollision(AOE, target->collisionMesh))
	{		
			return effect(target, type);
	}
	return "";
}
/******************************************************************************/
/*!
\brief
Effect 
\target
character going through effect
\type
type of effect
*/
/******************************************************************************/
std::string PressurePlate::effect(CharacterObject* target, int type)
{
	if (type == 0)
	{
		timer -= 1.0;
		target->vel *= 0.5;
		if (timer <= 0)
			return "TIME";
		return "You Have Been Slowed! ½ Speed!";
	}
	else if (type == 1)
	{
		timer -= 1.0;
		for (int i = 0; i < Sequence.size(); ++i)
		{
			if (Sequence[i] < timer)
				return Dialogue[i];
		}
		if (timer <= 0)
			return "TIME";
	}
	return "";
}
//Accessors
/******************************************************************************/
/*!
\brief
get area of effect
\return
return area of effect
*/
/******************************************************************************/
CollisionBox PressurePlate::getAOE(void)const
{
	return this->AOE;
}
/******************************************************************************/
/*!
\brief
get timer of effect
\return
return timer of effect
*/
/******************************************************************************/
double PressurePlate::getTimer(void)const
{
	return this->timer;
}
/******************************************************************************/
/*!
\brief
get dialogue to play on effect
\return
return dialogue
*/
/******************************************************************************/
std::vector<std::string> PressurePlate::getDialogue(void)const
{
	return this->Dialogue;
}

//Mutators
/******************************************************************************/
/*!
\brief
set area of effect
\param AOE
area of effect
*/
/******************************************************************************/
void PressurePlate::setAOE(CollisionBox &AOE)
{
	this->AOE = AOE;
}
/******************************************************************************/
/*!
\brief
set timer of effect
\param timer
timer of effect
*/
/******************************************************************************/
void PressurePlate::setTimer(double timer)
{
	this->timer = timer;
}
/******************************************************************************/
/*!
\brief
set dialogue to play on effect
\param text
dialogue
\param seq
time before next text
*/
/******************************************************************************/
void PressurePlate::setDialogue(std::vector<std::string> text , std::vector<double> seq)
{
	Dialogue.clear();
	for (int i = 0; i < text.size(); ++i)
	{
		Dialogue.push_back(text[i]);
	}

	Sequence.clear();
	for (int i = 0; i < seq.size(); ++i)
	{
		Sequence.push_back(seq[i]);
	}
}