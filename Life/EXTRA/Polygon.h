#pragma once
#include "Vertex.h"
#include "Mtx44.h"

class CPolygon
{
public:
	CPolygon(Vertex vertex1 = Vertex(), Vertex vertex2 = Vertex(), Vertex vertex3 = Vertex());
	~CPolygon(void);

	Vector3 GetNormal();
	Vector3 GetEdge1();
	Vector3 GetEdge2();
	Vector3 GetEdge3();

	bool Intersection(CPolygon &);
	bool Intersection(Vector3 , Vector3);
	void Move(Mtx44);
	void NormalCalulation();

private:
	Vertex vertex1;
	Vertex vertex2;
	Vertex vertex3;

	Vector3 normal;
};
