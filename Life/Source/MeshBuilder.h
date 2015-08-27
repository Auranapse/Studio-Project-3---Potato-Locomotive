/******************************************************************************/
/*!
\file	MeshBuilder.h
\author Gabriel Wong Choon Jieh
\par	email: AuraTigital\@gmail.com
\brief
Generate meshes
*/
/******************************************************************************/
#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include <vector>
#include "Mesh.h"
#include "SpriteAnimation.h"
#include "Vertex.h"
#include "LoadHmap.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateLine(const std::string &meshName,Color color, float lengthX = 0, float lengthY = 0, float lengthZ = 0);
	static Mesh* GenerateMatrix(const std::string &meshName, Color color, float size, float gap, float amount);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float lengthX, float lengthY, float patternrepeat = 1);
	static Mesh* GenerateCube(const std::string &meshName, Color color,float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateCubeT2(const std::string &meshName, Color color,float lengthX, float lengthY, float lengthZ, float patternrepeat = 1);
	static Mesh* GenerateCircle(const std::string &meshName, Color color, unsigned numSlice, float radius);
	static Mesh* GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float innerRadius, float outerRadius);
	static Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius);
	static Mesh* GenerateCone(const std::string &meshName, Color color, unsigned numSlice, float radius, float height);
	static Mesh* GeneratePartofSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius, float degreePerSlice, float degreePerStack);
	static Mesh* GenerateCylinder(const std::string &meshName, Color color, unsigned numSlice, float radius, float height);
	static Mesh* GenerateHollowCylinder(const std::string &meshName, Color color, unsigned numStack, float innerR, float outerR, float height);
	static Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path);
	static Mesh* GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol);
	static Mesh* GenerateSkybox(const std::string &meshName, Color color, float length = 1.f);
	static Mesh* GenerateSkyPlane(const std::string &meshName, Color color, int slices, float PlanetRadius, float AtmosphereRadius, float hTile, float vTile);
	static Mesh* GenerateTerrain(const std::string &meshName, const std::string &file_path, std::vector<unsigned char> &heightMap);
	static SpriteAnimation* GenerateSpriteAnimation(const std::string &meshName, unsigned numRow, unsigned numCol);
};

#endif