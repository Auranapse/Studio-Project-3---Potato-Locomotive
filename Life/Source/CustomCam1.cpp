/******************************************************************************/
/*!
\file	CustomCam1.cpp
\author Gabriel Wong Choon Jieh
\par	email: AuraTigital\@gmail.com
\brief
Custom made camera
*/
/******************************************************************************/
#include "CustomCam1.h"
#include "Application.h"
#include "Mtx44.h"

CustomCam1::CustomCam1()
{

}

CustomCam1::~CustomCam1()
{

}

/******************************************************************************/
/*!
\brief
Initialize default camera position, target, and tilt
*/
/******************************************************************************/
void CustomCam1::Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float mouseSensitivity)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	this->mouseSensitivity = mouseSensitivity;
	mouseX = 0.0;
	mouseY = 0.0;
}

/******************************************************************************/
/*!
\brief
Camera controls
*/
/******************************************************************************/
void CustomCam1::Update(double dt)
{
	double Xaxis = (Application::GetWindowWidth() / 2);
	double Yaxis = (Application::GetWindowHeight() / 2);
	Application::GetMousePos(mouseX, mouseY);
	mouseX = Xaxis - mouseX;
	mouseY = Yaxis - mouseY;
	mouseX /= 10.0;
	mouseY /= 10.0;
	mouseX *= static_cast<double>(mouseSensitivity);
	mouseY *= static_cast<double>(mouseSensitivity);

	if (mouseX != 0)
	{
		Mtx44 rotation;
		target -= position;
		rotation.SetToRotation(static_cast<float>(mouseX), 0, 1, 0);
		target = rotation * target;
		up = rotation * up;
		target += position;
	}

	if (mouseY != 0)
	{
		if (target.y - position.y < 0.97f && mouseY > 0 || target.y - position.y > -0.97f && mouseY < 0)
		{
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			target -= position;
			rotation.SetToRotation(static_cast<float>(mouseY), right.x, right.y, right.z);
			target = rotation * target;
			target += position;
		}
	}

	if (target.y - position.y < -0.975f)
	{
		rotateCamVertical(100.f * static_cast<float>(dt));
	}

	if (target.y - position.y > 0.975f)
	{
		rotateCamVertical(-100.f * static_cast<float>(dt));
	}

	if (mouseY != 0 || mouseX != 0)
	{
		Application::SetMouseinput(Xaxis, Yaxis);
	}
}

void CustomCam1::rotateCamVertical(float degrees)
{
	if (target.y - position.y < 0.97f && degrees > 0 || target.y - position.y > -0.97f && degrees < 0)
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		target -= position;
		rotation.SetToRotation(static_cast<float>(degrees), right.x, right.y, right.z);
		target = rotation * target;
		target += position;
	}
}

/******************************************************************************/
/*!
\brief
Resets camera to default camera position
*/
/******************************************************************************/
void CustomCam1::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

CustomCam1 operator+(CustomCam1 C1, const Vector3 Delta)
{
	C1.target = C1.target + Delta;
	C1.position = C1.position + Delta;
	return C1;
}