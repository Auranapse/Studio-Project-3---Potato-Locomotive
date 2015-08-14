/******************************************************************************/
/*!
\file	Mesh.h
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Mesh stuff
*/
/******************************************************************************/
#ifndef MESH_H
#define MESH_H

#include "Material.h"
#include <string>

/******************************************************************************/
/*!
		Class Mesh:
\brief	To store VBO (vertex & color buffer) and IBO (index buffer)
*/
/******************************************************************************/
class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
	};
	Mesh(const std::string &meshName);
	~Mesh();
	virtual void Render();
	virtual void Render(unsigned offset, unsigned count);
	const static unsigned int NUM_TEXTURES = 2;

	const std::string name;
	DRAW_MODE mode;
	unsigned vertexBuffer;
	unsigned indexBuffer;
	unsigned indexSize;

	Material material;
	unsigned textureID[NUM_TEXTURES];
};

#endif