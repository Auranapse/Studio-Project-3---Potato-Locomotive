/******************************************************************************/
/*!
\file	Material.h
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Material properties of meshes
*/
/******************************************************************************/
#ifndef MATERIAL_H
#define MATERIAL_H

/******************************************************************************/
/*!
		struct Component:
\brief
Component of material
*/
/******************************************************************************/
struct Component
{
	float r, g, b;
	Component(float r = 0.1f, float g = 0.1f, float b = 0.1f) 
	{Set(r, g, b);}
	void Set(float r, float g, float b) 
	{this->r = r; this->g = g; this->b = b;}
};

/******************************************************************************/
/*!
		struct Material:
\brief
Material properties when exposed to light
*/
/******************************************************************************/
struct Material
{
	Component kAmbient;
	Component kDiffuse;
	Component kSpecular;
	Component kEmissive;
	float kShininess;

	Material()
	{
		kAmbient.Set(0.2f, 0.2f, 0.2f);
		kDiffuse.Set(0.7f, 0.7f, 0.7f);
		kSpecular.Set(0.4f, 0.4f, 0.4f);
		kEmissive.Set(0.f, 0.f, 0.f);
		float kShininess = 1.f;
	}
};

#endif