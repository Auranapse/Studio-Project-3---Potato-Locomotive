#ifndef GABRIELDLC_H
#define GABRIELDLC_H

#include "Vector3.h"
#include "MyMath.h"
#include <iostream>

bool intersect2D(Vector3 &TopLeft, Vector3 &BottomRight, Vector3 &Position);
bool intersect(Vector3 &TopLeft, Vector3 &BottomRight, Vector3 &PosTCF);
float CalAnglefromPosition(Vector3 Target, Vector3 Origin, bool XZ);
float CalAnglefromPosition2D(Vector3 &Target, Vector3 &Origin);


#endif