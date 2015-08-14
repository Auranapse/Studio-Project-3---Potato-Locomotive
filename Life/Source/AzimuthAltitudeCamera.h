/******************************************************************************/
/*!
\file	Camera5.h
\author Gabriel Wong Choon Jieh
\par	email: AuraTigital\@gmail.com
\brief
Camera code 3
*/
/******************************************************************************/
#ifndef AZIMUTH_ALTITUDE_CAMERA_H
#define AZIMUTH_ALTITUDE_CAMERA_H

#include "Camera.h"


class AzimuthAltitudeCamera : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;
	/******************************************************************************/
	/*!
	Vector3 PositionOffset:
	\brief	Camera offset
	*/
	/******************************************************************************/
	Vector3 PositionOffset;
	/******************************************************************************/
	/*!
	Vector3 defaultPosition:
	\brief	Default camera position
	*/
	/******************************************************************************/
	Vector3 defaultPosition;
	/******************************************************************************/
	/*!
	Vector3 defaultTarget:
	\brief	Default target position
	*/
	/******************************************************************************/
	Vector3 defaultTarget;
	/******************************************************************************/
	/*!
	Vector3 defaultUp:
	\brief	default up position
	*/
	/******************************************************************************/
	Vector3 defaultUp;

	/******************************************************************************/
	/*!
	AzimuthAltitudeCamera():
	\brief	Default constructor
	*/
	/******************************************************************************/
	AzimuthAltitudeCamera();
	/******************************************************************************/
	/*!
	~AzimuthAltitudeCamera():
	\brief	Destructor
	*/
	/******************************************************************************/
	~AzimuthAltitudeCamera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, bool mouseI, float Sensitivity, Vector3 Target);
	virtual void Reset();
private:
	/******************************************************************************/
	/*!
			double mouseX:
	\brief	Horizontal position of the mouse on the screen
	*/
	/******************************************************************************/
	double mouseX;
	/******************************************************************************/
	/*!
			double mouseY:
	\brief	Vertical position of the mouse on the screen
	*/
	/******************************************************************************/
	double mouseY;
	/******************************************************************************/
	/*!
			double Xaxis:
	\brief	Horizontal position of the middle of the screen
	*/
	/******************************************************************************/
	double Xaxis;
	/******************************************************************************/
	/*!
			double Yaxis:
	\brief	Vertical position of the middle of the screen
	*/
	/******************************************************************************/
	double Yaxis;
};

#endif