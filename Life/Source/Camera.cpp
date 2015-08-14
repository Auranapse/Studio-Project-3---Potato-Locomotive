/******************************************************************************/
/*!
\file	Camera.cpp
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Camera code 1
*/
/******************************************************************************/
#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"

Camera::Camera()
{
	Reset();
}

Camera::~Camera()
{
}

/******************************************************************************/
/*!
\brief
Initialize default camera position, target, and tilt
*/
/******************************************************************************/
void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}

/******************************************************************************/
/*!
\brief
Resets camera to default camera position
*/
/******************************************************************************/
void Camera::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

/******************************************************************************/
/*!
\brief
Camera controls
\param dt - frame time
*/
/******************************************************************************/
void Camera::Update(double dt)
{
	static const float CAMERA_SPEED = 20.f;
	if(Application::IsKeyPressed(VK_LEFT) || Application::IsKeyPressed('A'))
	{
		position.x -= (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_RIGHT) || Application::IsKeyPressed('D'))
	{
		position.x += (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed('W'))
	{
		position.y += (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_DOWN) || Application::IsKeyPressed('S'))
	{
		position.y -= (float)(CAMERA_SPEED * 0.2 * dt);
	}
}

/******************************************************************************/
/*!
\brief
Calculates and returns a directional unit vector of of the View

\return the directional unit vector which the camera is pointing at
*/
/******************************************************************************/
Vector3 Camera::GetView()
{
	return (target - position).Normalized();
}