/****************************************************************************/
/*!
\file GabrielDLC.h
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Useful functions that I use often in my work
!*/
/****************************************************************************/
#ifndef GABRIELDLC_H
#define GABRIELDLC_H

#include "Vector3.h"
#include "MyMath.h"
#include <iostream>

bool intersect2D(const Vector3 &TopLeft, const Vector3 &BottomRight, const Vector3 &Position);
bool intersect(const Vector3 &TopLeft, const Vector3 &BottomRight, const Vector3 &Position);
float CalAnglefromPosition(Vector3 Target, Vector3 Origin, bool XZ);
float CalAnglefromPosition2D(const Vector3 &Target, const Vector3 &Origin);
float minDistance(const Vector3 &point, const Vector3 &pointPlane1, const Vector3 &pointPlane2);
bool isVisible(const Vector3 &Position, const Vector3 &Target, float FOV, const Vector3 &ObjectPosition);
bool isVisible2(const Vector3 &Position, const Vector3 &Target, float FOV, const Vector3 &ObjectPosition);

#endif