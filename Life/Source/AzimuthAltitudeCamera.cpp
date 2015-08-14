/******************************************************************************/
/*!
\file	Camera5.cpp
\author Gabriel Wong Choon Jieh
\par	email: AuraTigital\@gmail.com
\brief
Camera code V5
*/
/******************************************************************************/
#include "AzimuthAltitudeCamera.h"
#include "Application.h"
#include "Mtx44.h"
/******************************************************************************/
/*!
\brief
default constructor
*/
/******************************************************************************/
AzimuthAltitudeCamera::AzimuthAltitudeCamera()
{

}
/******************************************************************************/
/*!
\brief
destructor
*/
/******************************************************************************/
AzimuthAltitudeCamera::~AzimuthAltitudeCamera()
{

}

/******************************************************************************/
/*!
\brief
Initialize default camera position, target, and tilt
*/
/******************************************************************************/
void AzimuthAltitudeCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

/******************************************************************************/
/*!
\brief
Camera controls

\param dt time since last frame
\param mouseI if using mouseInput
\param mouse sensitivity
\param camera target
*/
/******************************************************************************/
void AzimuthAltitudeCamera::Update(double dt, bool mouseI, float Sensitivity, Vector3 Target)
{
	static const float CAMERA_SPEED = 50.f;
	target = Target;
	if(mouseI == true)
	{
		Xaxis = (Application::GetWindowWidth()/2);
		Yaxis = (Application::GetWindowHeight()/2);
		//std::cout << Xaxis;
		double mousePosX;
		double mousePosY;

		
		Application::GetMousePos(mousePosX, mousePosY);

		mouseX = (Xaxis - mousePosX) /32 * Sensitivity;
		mouseY = (Yaxis - mousePosY) /18 * Sensitivity;

		if(mouseX != 0 && !Application::IsKeyPressed(VK_MBUTTON))
		{
			float yaw = (float)(mouseX);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			position -= target;
			position = rotation * position;
			position +=target;
			up = rotation * up;

		}
		if(mouseY != 0 && !Application::IsKeyPressed(VK_MBUTTON))
		{
			float pitch = (float)(mouseY);
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			position -=target;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			position = rotation * position;
			position +=target;
		}

		if(Application::IsKeyPressed(VK_MBUTTON) && mouseY != 0)
		{
			Vector3 direction = target - position;
			if(direction.Length() > 15)
			{
				Vector3 view = (target - position).Normalized();
				position += view * (float)(mouseY);
			}

			if(direction.Length() <= 15)
			{
				if(mouseY < 0)
				{
					Vector3 view = (target - position).Normalized();
					position += view * (float)(-100*dt);
				}
			}
		}

		if(mouseX != 0 || mouseY !=0)
		{
			Application::SetMouseinput(Xaxis, Yaxis);
		}
	}
	else
	{
		if(Application::IsKeyPressed(VK_LEFT))
		{
			float yaw = (float)(-CAMERA_SPEED * dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			position -=target;
			position = rotation * position;
			position +=target;
			up = rotation * up;

		}
		if(Application::IsKeyPressed(VK_RIGHT))
		{
			float yaw = (float)(CAMERA_SPEED * dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			position -=target;
			position = rotation * position;
			position +=target;
			up = rotation * up;

		}
		if(Application::IsKeyPressed(VK_UP))
		{
			float pitch = (float)(-CAMERA_SPEED * dt);
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			position -=target;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			position = rotation * position;
			position +=target;
		}
		if(Application::IsKeyPressed(VK_DOWN))
		{
			float pitch = (float)(CAMERA_SPEED * dt);
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			position -=target;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			position = rotation * position;
			position +=target;
		}

		if(Application::IsKeyPressed('N'))
		{
			Vector3 direction = target - position;
			if(direction.Length() > 5)
			{
				Vector3 view = (target - position).Normalized();
				position += view * (float)(10.f * dt);
			}
		}
		if(Application::IsKeyPressed('M'))
		{
			Vector3 view = (target - position).Normalized();
			position -= view * (float)(10.f * dt);
		}
	}
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();


}

/******************************************************************************/
/*!
\brief
Resets camera to default camera position
*/
/******************************************************************************/
void AzimuthAltitudeCamera::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}