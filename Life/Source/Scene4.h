#ifndef SCENE_4_H
#define SCENE_4_H

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"

class Scene4 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_CUBE,
		GEO_AXES,
		GEO_QUAD,
		GEO_CIRCLE,
		GEO_RING,
		GEO_SPHERE,
		GEO_SUN,
		GEO_EARTH,
		GEO_MOON,
		GEO_SATAON,
		GEO_SATAONR,
		GEO_SATAMOON,
		GEO_ORBITRON,
		GEO_ORBITRON_MOON1,
		GEO_ORBITRON_MOON2,
		GEO_MRJOE,
		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_TOTAL,
	};
public:
	Scene4();
	~Scene4();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	unsigned m_vertexArrayID;
	//unsigned m_vertexBuffer[NUM_GEOMETRY];
	//unsigned m_colorBuffer[NUM_GEOMETRY];
	//unsigned m_indexBuffer[NUM_GEOMETRY];
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	
	float rotateA;
	float orbit;

	Camera camera;

	MS modelStack, viewStack, projectionStack;
};

#endif