/******************************************************************************/
/*!
\file	Camera4.h
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Camera code 2
*/
/******************************************************************************/
#ifndef CAMERA_4_H
#define CAMERA_4_H

#include "Camera.h"

class Camera4 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera4();
	~Camera4();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, bool mouseI, float Sensitivity);
	virtual void Reset();
	bool LookingAt(Vector3 XZ, Vector3 SXZ);
private:
	double mouseX, mouseY;
};

#endif