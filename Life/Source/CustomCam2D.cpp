/******************************************************************************/
/*!
\file	CustomCam2D.cpp
\author Gabriel Wong Choon Jieh
\par	email: AuraTigital\@gmail.com
\brief
Custom made camera
*/
/******************************************************************************/
#include "CustomCam2D.h"
#include "Application.h"
#include "Mtx44.h"

CustomCam2D::CustomCam2D()
{

}

CustomCam2D::~CustomCam2D()
{

}

/******************************************************************************/
/*!
\brief
Initialize default camera position, target, and tilt
*/
/******************************************************************************/
void CustomCam2D::Init(Vector3 Position)
{
	this->Position = Position;
}

/******************************************************************************/
/*!
\brief
Camera controls
*/
/******************************************************************************/
void CustomCam2D::Update(double dt)
{
	
}

CustomCam2D operator+(CustomCam2D C1, const Vector3 Delta)
{
	C1.Position = C1.Position + Delta;
	return C1;
}