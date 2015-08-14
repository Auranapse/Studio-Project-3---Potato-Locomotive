/******************************************************************************/
/*!
\file	Camera5.h
\author Gabriel Wong Choon Jieh
\par	email: AuraTigital\@gmail.com
\brief
Camera code 3
*/
/******************************************************************************/
#ifndef CAMERA_5_H
#define CAMERA_5_H

#include "Camera.h"

class Camera5 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;
	Vector3 PositionOffset;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera5();
	~Camera5();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, bool mouseI, float Sensitivity, Vector3 Target);
	virtual void Reset();
private:
	double mouseX, mouseY;
	double Xaxis, Yaxis;
};

#endif