#include "Polygon.h"


CPolygon::CPolygon(Vertex vertex1, Vertex vertex2, Vertex vertex3)
{
	this->vertex1 = vertex1;
	this->vertex2 = vertex2;
	this->vertex3 = vertex3;

	NormalCalulation();
}


CPolygon::~CPolygon(void)
{
}

void CPolygon::Move(Mtx44 test)
{
	vertex1.pos = test * vertex1.pos;
	vertex2.pos = test * vertex2.pos;
	vertex3.pos = test * vertex3.pos;

	NormalCalulation();
}

Vector3 CPolygon::GetNormal()
{
	return normal;
}
Vector3 CPolygon::GetEdge1()
{
	return vertex1.pos - vertex2.pos;
}
Vector3 CPolygon::GetEdge2()
{
	return vertex2.pos - vertex3.pos;
}
Vector3 CPolygon::GetEdge3()
{
	return vertex3.pos - vertex1.pos;
}
bool CPolygon::Intersection(CPolygon &p)
{
	if(p.Intersection(vertex2.pos, GetEdge1()) || p.Intersection(vertex3.pos, GetEdge2()) || p.Intersection(vertex1.pos, GetEdge3()) ||
		(Intersection(p.vertex2.pos, p.GetEdge1()) || Intersection(p.vertex3.pos, p.GetEdge2()) || Intersection(p.vertex1.pos, p.GetEdge3())))
	{
		return true;
	}

	else
		return false;
	
}
bool CPolygon::Intersection(Vector3 origin , Vector3 line)
{
	Vector3 pointofIntersect;
	int count = 0;

	if(Math::IsEqual(line.Dot(normal), 0))
	{
		return false;
	}

	float distance = normal.Dot(vertex1.pos); //point on the plane

	float eq = -(normal.Dot(origin) + distance) / normal.Dot(line);

	if(eq < 1 || eq > 0)
	{
		return false;
	}

	pointofIntersect = line * eq + origin;

	Vector3 vector1 = vertex1.pos - pointofIntersect;
	Vector3 vector2 = vertex2.pos - pointofIntersect;
	Vector3 vector3 = vertex3.pos - pointofIntersect;

	if(vector1.GetAngle(vector2) > Math::PI || vector2.GetAngle(vector3) > Math::PI || vector3.GetAngle(vector1) > Math::PI)
	{
		return false;
	}

	else 
		return true;

	return false;
}

void CPolygon::NormalCalulation()
{
	normal = GetEdge1().Cross(GetEdge2());
}