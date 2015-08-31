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
CharacterObject::CharacterObject(void) : Lookat(0.f, 0.f, 1.f), Head(NULL), Chest(NULL), Arm_left(NULL), Arm_right(NULL), Leg_left(NULL), Leg_right(NULL), Animation(0, 0, 0, 0), f_move_crawl(10.f), f_move_walk(20.f), f_move_run(40.f), f_movementSpeed(f_move_walk)
{
	HeadPos.Set(0.f, 6.f, 0.f);
	ArmPos.Set(4.f, 4.f, 0.f);
	LegPos.Set(0.f, -6.f, 0.f);
	ModelPos.Set(0.f, 18.f, 0.f);
	CamOffset.Set(0.f, 30.f, 0.f);
	holding = NULL;
	active = true;
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
Gets the current direction character is looking at
\return
direction vector
*/
/******************************************************************************/
Vector3 CharacterObject::getDirection(bool XZ)
{
	if (XZ)
	{
		if (Lookat == pos)
		{
			return Vector3(0, 0, 1);
		}
		return Vector3(Lookat.x - pos.x + CamOffset.x, 0, Lookat.z - pos.z + CamOffset.z);
	}

	return (Lookat - (pos + CamOffset));
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
void CharacterObject::Init(const Vector3 &Pos, const Vector3 &Lookat, const char* texturedir)
{
	this->pos = Pos;
	this->Lookat = Lookat;
	this->defaultLookat = Lookat;

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

	Head->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	Head->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	Head->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	Head->material.kShininess = 1.0f;

	Chest->material = Head->material;
	Arm_left->material = Head->material;
	Arm_right->material = Head->material;
	Leg_left->material = Head->material;
	Leg_right->material = Head->material;
}

/******************************************************************************/
/*!
\brief
Set object to hold
\param obj
the object pointer to hold
*/
/******************************************************************************/
void CharacterObject::HoldObject(ItemObject *obj)
{
	if (obj->holdable)
	{
		holding = obj;
		obj->colEnable = false;
		obj->pos = Vector3(0, -10, 0);
		obj->isHeld = true;
	}
}

/******************************************************************************/
/*!
\brief
Set object to hold
\param obj
the object pointer to hold
*/
/******************************************************************************/
void CharacterObject::DropObject(const Vector3 &ThrowVel)
{
	if (holding != NULL)
	{
		Mtx44 tempR;
		tempR.SetToRotation(CalAnglefromPosition(Lookat, pos, true), 0, 1, 0);
		holding->pos = pos + CamOffset + tempR*holding->pos;
		holding->rotation.y = CalAnglefromPosition(Lookat, pos, true);
		holding->colEnable = true;
		holding->enablePhysics = true;
		holding->isHeld = false;
		holding->vel = vel + ThrowVel;
		holding = NULL;
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
void CharacterObject::Update(const double &dt)
{
	Animation.Update(dt, vel.LengthSquared() * 0.03f);
	Lookat += vel * static_cast<float>(dt);
	pos += vel * static_cast<float>(dt);
}