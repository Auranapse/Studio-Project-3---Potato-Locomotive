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

bool intersect2D(Vector3 &TopLeft, Vector3 &BottomRight, Vector3 &Position);
bool intersect(Vector3 &TopLeft, Vector3 &BottomRight, Vector3 &Position);
float CalAnglefromPosition(Vector3 Target, Vector3 Origin, bool XZ);
float CalAnglefromPosition2D(Vector3 &Target, Vector3 &Origin);
float minDistance(Vector3 &point, Vector3 &pointPlane1, Vector3 &pointPlane2);
bool isVisible(Vector3 &Position, Vector3 &Target, float FOV, Vector3 &ObjectPosition);
bool isVisible2(Vector3 &Position, Vector3 &Target, float FOV, Vector3 &ObjectPosition);

#endif