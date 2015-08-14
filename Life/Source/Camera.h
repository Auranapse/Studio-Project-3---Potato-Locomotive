/******************************************************************************/
/*!
\file	Camera.h
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Camera code 1
*/
/******************************************************************************/
#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

class Camera
{
public:
/******************************************************************************/
/*!
		Vector3 position:
\brief	The camera's position
*/
/******************************************************************************/
	Vector3 position;
/******************************************************************************/
/*!
		Vector3 target:
\brief	The camera's target
*/
/******************************************************************************/
	Vector3 target;
/******************************************************************************/
/*!
		Vector3 up:
\brief	The direction that is "up" to the camera
*/
/******************************************************************************/
	Vector3 up;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
	Vector3 GetView();
};

#endif