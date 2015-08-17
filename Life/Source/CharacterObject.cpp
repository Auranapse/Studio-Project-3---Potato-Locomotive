/****************************************************************************/
/*!
\file CharacterObject.cpp
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Character Object used by AI and player
!*/
/****************************************************************************/
#include "CharacterObject.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
CharacterObject::CharacterObject(void) : Position(0.f, 0.f, 0.f), Lookat(0.f, 0.f, 1.f), Velocity(0.f, 0.f, 0.f), Scale(1.f, 1.f, 1.f), Head(NULL), Chest(NULL), Arm_left(NULL), Arm_right(NULL), Leg_left(NULL), Leg_right(NULL), Animation(0, 0, 0, 0), f_move_crawl(20.f), f_move_walk(40.f), f_move_run(80.f), f_movementSpeed(f_move_walk)
{
	HeadPos.Set(0.f, 6.f, 0.f);
	ArmPos.Set(4.f, 4.f, 0.f);
	LegPos.Set(0.f, -6.f, 0.f);
	ModelPos.Set(0.f, 18.f, 0.f);
	CamOffset.Set(0.f, 30.f, 0.f);
	holding = NULL;
}

/******************************************************************************/
/*!
\brief
Default destructor
*/
/******************************************************************************/
CharacterObject::~CharacterObject(void)
{
	if (Head != NULL)
	{
		delete Head;
		Head = NULL;
	}

	if (Chest != NULL)
	{
		delete Chest;
		Chest = NULL;
	}

	if (Arm_left != NULL)
	{
		delete Arm_left;
		Arm_left = NULL;
	}

	if (Arm_right != NULL)
	{
		delete Arm_right;
		Arm_right = NULL;
	}

	if (Leg_left != NULL)
	{
		delete Leg_left;
		Leg_left = NULL;
	}

	if (Leg_right != NULL)
	{
		delete Leg_right;
		Leg_right = NULL;
	}
}

/******************************************************************************/
/*!
\brief
Gets the current animation
\return
current animation
*/
/******************************************************************************/
Anim4 CharacterObject::getAnimation(void)
{
	return Animation;
}

/******************************************************************************/
/*!
\brief
Sets a new position for the character
\param &newpos
the new position to set to

I recommend not touching position, go touch velocity instead
*/
/******************************************************************************/
void CharacterObject::setPosition(Vector3 &newpos)
{
	Position = newpos;
}

/******************************************************************************/
/*!
\brief
Gets the current position of the character
\return
current position
*/
/******************************************************************************/
Vector3 CharacterObject::getPosition(void)
{
	return Position;
}

/******************************************************************************/
/*!
\brief
Gets the current direction character is looking at
\return
direction vector
*/
/******************************************************************************/
Vector3 CharacterObject::getDirection(void)
{
	return (Lookat - (Position+CamOffset));
}

/******************************************************************************/
/*!
\brief
Inits the character
\param Pos
Position of the character
\param Lookat
what character is looking at
\param texturedir
the texture location
*/
/******************************************************************************/
void CharacterObject::Init(Vector3 Pos, Vector3 Lookat, const char* texturedir)
{
	this->Position = Pos;
	this->Lookat = Lookat;

	Head = MeshBuilder::GenerateOBJ("Head", "GameData//OBJ//character//Person_Head.obj");
	Chest = MeshBuilder::GenerateOBJ("Chest", "GameData//OBJ//character//Person_Chest.obj");
	Arm_left = MeshBuilder::GenerateOBJ("Arm Left", "GameData//OBJ//character//Person_Arm_Left.obj");
	Arm_right = MeshBuilder::GenerateOBJ("Arm Right", "GameData//OBJ//character//Person_Arm_Right.obj");
	Leg_left = MeshBuilder::GenerateOBJ("Leg Left", "GameData//OBJ//character//Person_Leg_Left.obj");
	Leg_right = MeshBuilder::GenerateOBJ("Leg Right", "GameData//OBJ//character//Person_Leg_Right.obj");

	Head->textureID[0] = LoadTGA(texturedir, true, false);
	Chest->textureID[0] = Head->textureID[0];
	Arm_left->textureID[0] = Head->textureID[0];
	Arm_right->textureID[0] = Head->textureID[0];
	Leg_left->textureID[0] = Head->textureID[0];
	Leg_right->textureID[0] = Head->textureID[0];
}

void CharacterObject::HoldObject(ItemObject *obj)
{
	if (obj->pickupable)
	{
		holding = obj;
		obj->colEnable = false;
	}
}

void CharacterObject::UpdateHold(double &dt)
{
	if (holding != NULL)
	{
		
	}
	else
	{
		
	}
}

/******************************************************************************/
/*!
\brief
Updates the charater
\param dt
delta time
*/
/******************************************************************************/
void CharacterObject::Update(double &dt)
{
	Animation.Update(dt, Velocity.LengthSquared() * 0.03);
	Lookat += Velocity * static_cast<float>(dt);
	Position += Velocity * static_cast<float>(dt);
}