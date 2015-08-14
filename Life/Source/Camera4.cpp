/******************************************************************************/
/*!
\file	Camera4.cpp
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Camera code 2
*/
/******************************************************************************/
#include "Camera4.h"
#include "Application.h"
#include "Mtx44.h"

Camera4::Camera4()
{

}

Camera4::~Camera4()
{

}

/******************************************************************************/
/*!
\brief
Initialize default camera position, target, and tilt
*/
/******************************************************************************/
void Camera4::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
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
*/
/******************************************************************************/
void Camera4::Update(double dt, bool mouseI, float Sensitivity)
{
	static float CAMERA_RSPEED = 80.f;
	static float CAMERA_SPEED = 100.f;

	if(Application::IsKeyPressed(VK_SHIFT))
	{
		CAMERA_SPEED = 300.f;
	}
	else if(Application::IsKeyPressed(VK_CONTROL))
	{
		CAMERA_SPEED = 50.f;
		CAMERA_RSPEED = 5.f;
	}

	else
	{
		CAMERA_SPEED = 100.f;
		CAMERA_RSPEED = 80.f;
	}

	if(Application::IsKeyPressed('A'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();

		position -= right * CAMERA_SPEED * static_cast<float>(dt);
		target -= right * CAMERA_SPEED * static_cast<float>(dt);
	}
	if(Application::IsKeyPressed('D'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();

		position += right * CAMERA_SPEED * static_cast<float>(dt);
		target += right * CAMERA_SPEED * static_cast<float>(dt);
	}

	if(Application::IsKeyPressed('W'))
	{
		Vector3 view = (target - position).Normalized();
		position += view * CAMERA_SPEED * static_cast<float>(dt);
		target += view * CAMERA_SPEED * static_cast<float>(dt);
	}

	if(Application::IsKeyPressed('S'))
	{
		Vector3 view = (target - position).Normalized();
		position -= view * CAMERA_SPEED * static_cast<float>(dt);
		target -= view * CAMERA_SPEED * static_cast<float>(dt);
	}

	
	if(mouseI == true)
	{
		double Xaxis = (Application::GetWindowWidth()/2);
		double Yaxis = (Application::GetWindowHeight()/2);
		Application::GetMousePos(mouseX, mouseY);
		mouseX = Xaxis - mouseX;
		mouseY = Yaxis - mouseY;
		mouseX /= 10;
		mouseY /= 10;


		if(mouseX != 0)
		{
			Mtx44 rotation;
			target -= position;
			rotation.SetToRotation(static_cast<float>(mouseX), 0, 1, 0);
			target = rotation * target;
			up = rotation * up;
			target += position;
		}

		if(mouseY != 0)
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

		//if(mouseX != 0 || mouseY !=0)
		//{
			Application::SetMouseinput(Xaxis, Yaxis);
		//}
	}
	else
	{
		if(Application::IsKeyPressed(VK_LEFT))
		{
			float yaw;
			yaw = (float)(CAMERA_RSPEED * dt);
			Mtx44 rotation;
			position -= target;
			rotation.SetToRotation(yaw, 0, 1, 0);
			position = rotation * position;
			up = rotation * up;
			position += target;
		}
		if(Application::IsKeyPressed(VK_RIGHT))
		{
			float yaw;
			yaw = (float)(-CAMERA_RSPEED * dt);
			Mtx44 rotation;
			position -= target;
			rotation.SetToRotation(yaw, 0, 1, 0);
			position = rotation * position;
			up = rotation * up;
			position += target;
		}

		if(Application::IsKeyPressed(VK_DOWN))
		{
			float pitch = (float)(-CAMERA_RSPEED * dt);
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

		if(Application::IsKeyPressed(VK_UP))
		{
			float pitch = (float)(CAMERA_RSPEED * dt);
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
	}
	/*if(Application::IsKeyPressed('N'))
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
	}*/
}

/******************************************************************************/
/*!
\brief
Check if camera looking at object
*/
/******************************************************************************/
bool Camera4::LookingAt(Vector3 XZ, Vector3 SXZ)
{
	if((position.x >= SXZ.x && position.x <= XZ.x) && (position.z >= SXZ.z && position.z <= XZ.z))
	{
		return true;
	}
	return false;
}


/******************************************************************************/
/*!
\brief
Resets camera to default camera position
*/
/******************************************************************************/
void Camera4::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}