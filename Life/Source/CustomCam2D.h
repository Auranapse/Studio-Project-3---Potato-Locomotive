/******************************************************************************/
/*!
\file	CustomCam2D.h
\author Gabriel Wong Choon Jieh
\par	email: AuraTigital\@gmail.com
\brief
Custom made camera
*/
/******************************************************************************/
#ifndef CUSTOMCAM2D_H
#define CUSTOMCAM2D_H

#include "Camera.h"

class CustomCam2D
{
public:
	Vector3 Position;

	CustomCam2D();
	~CustomCam2D();
	virtual void Init(Vector3 Position);
	virtual void Update(double dt);

private:
	friend CustomCam2D operator+(CustomCam2D C1, const Vector3 Delta);
};

#endif