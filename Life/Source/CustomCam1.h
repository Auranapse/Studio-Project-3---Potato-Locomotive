/******************************************************************************/
/*!
\file	CustomCam1.h
\author Gabriel Wong Choon Jieh
\par	email: AuraTigital\@gmail.com
\brief
Custom made camera
*/
/******************************************************************************/
#ifndef CUSTOMCAM1_H
#define CUSTOMCAM1_H

#include "Camera.h"
#include "GabrielDLC.h"

class CustomCam1 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;
	float mouseSensitivity;
	float f_currentPitch;
	float f_pitchLimit;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	CustomCam1();
	~CustomCam1();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float mouseSensitivity);
	virtual void Update(double dt);
	void rotateCamVertical(float degrees);
private:
	double mouseX, mouseY;
	double Xaxis, Yaxis;
	
	friend CustomCam1 operator+(CustomCam1 C1, const Vector3 Delta);
};

#endif