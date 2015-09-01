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
Initialize default camera position, target, and up vector
\param pos
position of camera
\param target
where camera is looking at
\param up
the up direction of the camera
\param mouseSensitivity
the sensitivity of the mouse control
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

	f_currentPitch = CalAnglefromPosition(target, position, false);
	f_pitchLimit = 80.f;
}

/******************************************************************************/
/*!
\brief
updates camera rotation
\param dt
delta time
*/
/******************************************************************************/
void CustomCam1::Update(double dt)
{
	double Xaxis = (Application::GetWindowWidth() * 0.5);
	double Yaxis = (Application::GetWindowHeight() * 0.5);
	Application::GetMousePos(mouseX, mouseY);
	mouseX = Xaxis - mouseX;
	mouseY = Yaxis - mouseY;
	mouseX *= 0.1;
	mouseY *= 0.1;
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
		if (mouseY + f_currentPitch > f_pitchLimit && mouseY > 0)
		{
			mouseY = f_pitchLimit - f_currentPitch;

			if (f_currentPitch >= f_pitchLimit)
			{
				mouseY = 0.f;
			}
		}

		else if (mouseY + f_currentPitch < -f_pitchLimit && mouseY < 0)
		{
			mouseY = -f_pitchLimit - f_currentPitch;

			if (f_currentPitch <= -f_pitchLimit)
			{
				mouseY = 0.f;
			}
		}

		if (mouseY != 0)
		{
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			target -= position;
			f_currentPitch += mouseY;
			rotation.SetToRotation(static_cast<float>(mouseY), right.x, right.y, right.z);
			target = rotation * target;
			target += position;
		}
	}

	//if (mouseY != 0 || mouseX != 0)
	{
		Application::SetMouseinput(Xaxis, Yaxis);
	}
}

/******************************************************************************/
/*!
\brief
rotates camera pitch
\param degrees
the degrees to rotate by
*/
/******************************************************************************/
void CustomCam1::rotateCamVertical(float degrees)
{
	if (degrees + f_currentPitch > f_pitchLimit && degrees > 0)
	{
		degrees = f_pitchLimit - f_currentPitch;

		if (f_currentPitch >= f_pitchLimit)
		{
			degrees = 0.f;
		}
	}

	else if (degrees + f_currentPitch < -f_pitchLimit && degrees < 0)
	{
		degrees = -f_pitchLimit - f_currentPitch;

		if (f_currentPitch <= -f_pitchLimit)
		{
			degrees = 0.f;
		}
	}

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	target -= position;
	f_currentPitch += degrees;
	rotation.SetToRotation(static_cast<float>(degrees), right.x, right.y, right.z);
	target = rotation * target;
	target += position;
}

CustomCam1 operator+(CustomCam1 C1, const Vector3 Delta)
{
	C1.target = C1.target + Delta;
	C1.position = C1.position + Delta;
	return C1;
}