/******************************************************************************/
/*!
\file	MeshBuilder.cpp
\author Gabriel Wong Choon Jieh
\par	email: AuraTigital\@gmail.com
\brief
Generate meshes
*/
/******************************************************************************/
#include "MeshBuilder.h"
#include <GL\glew.h>
#include <vector>
#include "Vertex.h"
#include "MyMath.h"
#include "LoadOBJ.h"
/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;

	v.pos.Set(-lengthX, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(lengthX, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0, -lengthY, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0, lengthY, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0, 0, -lengthZ);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0, 0, lengthZ);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}


/******************************************************************************/
/*!
\brief
Generate the vertices of a Line, the color for the line is defined by the user
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param color - Color of the line
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateLine(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;

	v.pos.Set(0, 0, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(lengthX, 0, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(0, 0, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(0, lengthY, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(0, 0, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(0, 0, lengthZ);
	v.color = color;
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate a matrix style mesh

\param meshName
name of mesh
\param Color
color of the matrix

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateMatrix(const std::string &meshName, Color color, float size, float gap, float amount)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;

	for (unsigned i = 0; i < amount; ++i)
	{
		for (unsigned j = 0; j < amount; ++j)
		{
			v.pos.Set(-size, i * gap, j * gap);
			v.color = color;
			vertex_buffer_data.push_back(v);

			v.pos.Set(size, i * gap, j * gap);
			v.color = color;
			vertex_buffer_data.push_back(v);

			v.pos.Set(i * gap, -size, j * gap);
			v.color = color;
			vertex_buffer_data.push_back(v);

			v.pos.Set(i * gap, size, j * gap);
			v.color = color;
			vertex_buffer_data.push_back(v);

			v.pos.Set(i * gap, j * gap, -size);
			v.color = color;
			vertex_buffer_data.push_back(v);

			v.pos.Set(i * gap, j * gap, size);
			v.color = color;
			vertex_buffer_data.push_back(v);
		}
	}
	

	std::vector<GLuint> index_buffer_data;

	for (unsigned i = 0; i < vertex_buffer_data.size(); ++i)
	{
		index_buffer_data.push_back(i);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad
\param patternrepeat - 1 / the size of the texture

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float lengthX, float lengthY, float patternrepeat)
{
	float U = 1;
	float V = 1;

	if (lengthX > lengthY)
	{
		U *= (lengthX / lengthY);
	}
	else
	{
		V *= (lengthY / lengthX);
	}

	U *= patternrepeat;
	V *= patternrepeat;

	if (patternrepeat == 1 && lengthX != lengthY)
	{
		U = 1;
		V = 1;
	}

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;

	v.pos.Set(lengthX, -lengthY, 0.0f);//Bottom right
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(U, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(lengthX, lengthY, 0.0f);//Top right
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(U, V);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-lengthX, lengthY, 0.0f);//Top left
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, V);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-lengthX, -lengthY, 0.0f);//Bottom Left
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);

	std::vector <GLuint> index_buffer_data;

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;


	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	v.pos.Set(-lengthX, -lengthY, -lengthZ);//Rear bottom left
	v.color = color;
	v.normal.Set(-1, 0, 0);
	v.texCoord.Set(0.009997f, 0.386717f);
	vertex_buffer_data.push_back(v);//0

	v.pos.Set(lengthX, -lengthY, -lengthZ);//Rear bottom right
	v.color = color;
	v.normal.Set(1, 0, 0);
	v.texCoord.Set(0.993279f, 0.379516f);
	vertex_buffer_data.push_back(v);//1

	v.pos.Set(lengthX, lengthY, -lengthZ);//Rear top right
	v.color = color;
	v.normal.Set(1, 0, 0);
	v.texCoord.Set(0.993279f, 0.696533f);
	vertex_buffer_data.push_back(v);//2

	v.pos.Set(-lengthX, lengthY, -lengthZ);//Rear top left
	v.color = color;
	v.normal.Set(-1, 0, 0);
	v.texCoord.Set(0.009997f, 0.703734f);
	vertex_buffer_data.push_back(v);//3

	v.pos.Set(-lengthX, -lengthY, lengthZ);//Front bottom left
	v.color = color;
	v.normal.Set(-1, 0, 0);
	v.texCoord.Set(0.327014f, 0.386717f);
	vertex_buffer_data.push_back(v);//4

	v.pos.Set(lengthX, -lengthY, lengthZ);//Front bottom right
	v.color = color;
	v.normal.Set(1, 0, 0);
	v.texCoord.Set(0.676262f, 0.379516f);
	vertex_buffer_data.push_back(v);//5

	v.pos.Set(lengthX, lengthY, lengthZ);//Front top right
	v.color = color;
	v.normal.Set(1, 0, 0);
	v.texCoord.Set(0.676262f, 0.696533f);
	vertex_buffer_data.push_back(v);//6

	v.pos.Set(-lengthX, lengthY, lengthZ);//Front top left
	v.color = color;
	v.normal.Set(-1, 0, 0);
	v.texCoord.Set(0.327014f, 0.703734f);
	vertex_buffer_data.push_back(v);//7

	//-------------------------------Stage 2

	v.pos.Set(-lengthX, -lengthY, -lengthZ);//Rear bottom left
	v.color = color;
	v.normal.Set(0, -1, 0);
	v.texCoord.Set(0.346072f, 0.040179f);
	vertex_buffer_data.push_back(v);//8

	v.pos.Set(lengthX, -lengthY, -lengthZ);//Rear bottom right
	v.color = color;
	v.normal.Set(0, -1, 0);
	v.texCoord.Set(0.663090f, 0.040179f);
	vertex_buffer_data.push_back(v);//9

	v.pos.Set(lengthX, lengthY, -lengthZ);//Rear top right
	v.color = color;
	v.normal.Set(0, 1, 0);
	v.texCoord.Set(0.655066f, 0.995437f);
	vertex_buffer_data.push_back(v);//10

	v.pos.Set(-lengthX, lengthY, -lengthZ);//Rear top left
	v.color = color;
	v.normal.Set(0, 1, 0);
	v.texCoord.Set(0.338049f, 0.995437f);
	vertex_buffer_data.push_back(v);//11

	v.pos.Set(-lengthX, -lengthY, lengthZ);//Front bottom left
	v.color = color;
	v.normal.Set(0, -1, 0);
	v.texCoord.Set(0.346072f, 0.357197f);
	vertex_buffer_data.push_back(v);//12

	v.pos.Set(lengthX, -lengthY, lengthZ);//Front bottom right
	v.color = color;
	v.normal.Set(0, -1, 0);
	v.texCoord.Set(0.663090f, 0.357197f);
	vertex_buffer_data.push_back(v);//13

	v.pos.Set(lengthX, lengthY, lengthZ);//Front top right
	v.color = color;
	v.normal.Set(0, 1, 0);
	v.texCoord.Set(0.655066f, 0.678420f);
	vertex_buffer_data.push_back(v);//14

	v.pos.Set(-lengthX, lengthY, lengthZ);//Front top left
	v.color = color;
	v.normal.Set(0, 1, 0);
	v.texCoord.Set(0.338049f, 0.678420f);
	vertex_buffer_data.push_back(v);//15


	//-------------------------------Stage 3

	v.pos.Set(-lengthX, -lengthY, -lengthZ);//Rear bottom left
	v.color = color;
	v.normal.Set(0, 0, -1);
	v.texCoord.Set(0.999309f, 0.040441f);
	vertex_buffer_data.push_back(v);//16

	v.pos.Set(lengthX, -lengthY, -lengthZ);//Rear bottom right
	v.color = color;
	v.normal.Set(0, 0, -1);
	v.texCoord.Set(0.682291f, 0.040441f);
	vertex_buffer_data.push_back(v);//17

	v.pos.Set(lengthX, lengthY, -lengthZ);//Rear top right
	v.color = color;
	v.normal.Set(0, 0, -1);
	v.texCoord.Set(0.682291f, 0.357458f);
	vertex_buffer_data.push_back(v);//18

	v.pos.Set(-lengthX, lengthY, -lengthZ);//Rear top left
	v.color = color;
	v.normal.Set(0, 0, -1);
	v.texCoord.Set(0.999309f, 0.357458f);
	vertex_buffer_data.push_back(v);//19

	v.pos.Set(-lengthX, -lengthY, lengthZ);//Front bottom left
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0.012595f, 0.044471f);
	vertex_buffer_data.push_back(v);//20

	v.pos.Set(lengthX, -lengthY, lengthZ);//Front bottom right
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0.329612f, 0.044471f);
	vertex_buffer_data.push_back(v);//21

	v.pos.Set(lengthX, lengthY, lengthZ);//Front top right
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0.329612f, 0.361489f);
	vertex_buffer_data.push_back(v);//22

	v.pos.Set(-lengthX, lengthY, lengthZ);//Front top left
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0.012595f, 0.361489f);
	vertex_buffer_data.push_back(v);//23






	//Index buffer..........................................

	index_buffer_data.push_back(22);//Front face t1
	index_buffer_data.push_back(20);
	index_buffer_data.push_back(21);

	index_buffer_data.push_back(22);//Front face t2
	index_buffer_data.push_back(23);
	index_buffer_data.push_back(20);

	index_buffer_data.push_back(7);//Left face t1
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(4);

	index_buffer_data.push_back(7);//Left face t2
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);

	index_buffer_data.push_back(2);//Right face t1
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(1);

	index_buffer_data.push_back(2);//Right face t2
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);

	index_buffer_data.push_back(19);//Back face t1
	index_buffer_data.push_back(17);
	index_buffer_data.push_back(16);

	index_buffer_data.push_back(19);//Back face t2
	index_buffer_data.push_back(18);
	index_buffer_data.push_back(17);

	index_buffer_data.push_back(10);//Top face t1
	index_buffer_data.push_back(15);
	index_buffer_data.push_back(14);

	index_buffer_data.push_back(10);//Top face t2
	index_buffer_data.push_back(11);
	index_buffer_data.push_back(15);

	index_buffer_data.push_back(13);//Bottom face t1
	index_buffer_data.push_back(8);
	index_buffer_data.push_back(9);

	index_buffer_data.push_back(13);//Bottom face t2
	index_buffer_data.push_back(12);
	index_buffer_data.push_back(8);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object
This is same same with generate cube except that it is using a different way of mapping a texture

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube
\param patternrepeat - 1 / the size of the texture

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCubeT2(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ, float patternrepeat)
{
	float UXY = 1;
	float VXY = 1;
	float UXZ = 1;
	float VXZ = 1;
	float UZY = 1;
	float VZY = 1;


	UXZ *= patternrepeat;
	VXZ *= patternrepeat;
	UZY *= patternrepeat;
	VZY *= patternrepeat;
	UXY *= patternrepeat;
	VXY *= patternrepeat;

	if (lengthX > lengthZ)
	{
		UXZ *= (lengthX / lengthZ);
	}
	else
	{
		VXZ *= (lengthZ / lengthX);
	}

	if (lengthX > lengthY)
	{
		UXY *= (lengthX / lengthY);
	}
	else
	{
		VXY *= (lengthY / lengthX);
	}

	if (lengthZ > lengthY)
	{
		UZY *= (lengthZ / lengthY);
	}
	else
	{
		VZY *= (lengthY / lengthZ);
	}

	if (patternrepeat == 1)
	{
		UXZ = 1;
		VXZ = 1;
		UXY = 1;
		VXY = 1;
		UZY = 1;
		VZY = 1;
	}

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	v.pos.Set(-lengthX, -lengthY, -lengthZ);//Rear bottom left
	v.color = color;
	v.normal.Set(-1, 0, 0);
	v.texCoord.Set(0, 0);//Bottom left
	vertex_buffer_data.push_back(v);//0

	v.pos.Set(lengthX, -lengthY, -lengthZ);//Rear bottom right
	v.color = color;
	v.normal.Set(1, 0, 0);
	v.texCoord.Set(UZY, 0);//Bottom right
	vertex_buffer_data.push_back(v);//1

	v.pos.Set(lengthX, lengthY, -lengthZ);//Rear top right
	v.color = color;
	v.normal.Set(1, 0, 0);
	v.texCoord.Set(UZY, VZY);// Top right
	vertex_buffer_data.push_back(v);//2

	v.pos.Set(-lengthX, lengthY, -lengthZ);//Rear top left
	v.color = color;
	v.normal.Set(-1, 0, 0);
	v.texCoord.Set(0, VZY);//Top left
	vertex_buffer_data.push_back(v);//3

	v.pos.Set(-lengthX, -lengthY, lengthZ);//Front bottom left
	v.color = color;
	v.normal.Set(-1, 0, 0);
	v.texCoord.Set(UZY, 0);//Bottom right
	vertex_buffer_data.push_back(v);//4

	v.pos.Set(lengthX, -lengthY, lengthZ);//Front bottom right
	v.color = color;
	v.normal.Set(1, 0, 0);
	v.texCoord.Set(0, 0);//Bottom left
	vertex_buffer_data.push_back(v);//5

	v.pos.Set(lengthX, lengthY, lengthZ);//Front top right
	v.color = color;
	v.normal.Set(1, 0, 0);
	v.texCoord.Set(0, VZY);//Top left
	vertex_buffer_data.push_back(v);//6

	v.pos.Set(-lengthX, lengthY, lengthZ);//Front top left
	v.color = color;
	v.normal.Set(-1, 0, 0);
	v.texCoord.Set(UZY, VZY);// Top right
	vertex_buffer_data.push_back(v);//7

	//-------------------------------Stage 2

	v.pos.Set(-lengthX, -lengthY, -lengthZ);//Rear bottom left
	v.color = color;
	v.normal.Set(0, -1, 0);
	v.texCoord.Set(0, 0);//Bottom left
	vertex_buffer_data.push_back(v);//8

	v.pos.Set(lengthX, -lengthY, -lengthZ);//Rear bottom right
	v.color = color;
	v.normal.Set(0, -1, 0);
	v.texCoord.Set(UXZ, 0);//Bottom right
	vertex_buffer_data.push_back(v);//9

	v.pos.Set(lengthX, lengthY, -lengthZ);//Rear top right
	v.color = color;
	v.normal.Set(0, 1, 0);
	v.texCoord.Set(UXZ, VXZ);// Top right
	vertex_buffer_data.push_back(v);//10

	v.pos.Set(-lengthX, lengthY, -lengthZ);//Rear top left
	v.color = color;
	v.normal.Set(0, 1, 0);
	v.texCoord.Set(0, VXZ);//Top left
	vertex_buffer_data.push_back(v);//11

	v.pos.Set(-lengthX, -lengthY, lengthZ);//Front bottom left
	v.color = color;
	v.normal.Set(0, -1, 0);
	v.texCoord.Set(0, VXZ);//Top left
	vertex_buffer_data.push_back(v);//12

	v.pos.Set(lengthX, -lengthY, lengthZ);//Front bottom right
	v.color = color;
	v.normal.Set(0, -1, 0);
	v.texCoord.Set(UXZ, VXZ);// Top right
	vertex_buffer_data.push_back(v);//13

	v.pos.Set(lengthX, lengthY, lengthZ);//Front top right
	v.color = color;
	v.normal.Set(0, 1, 0);
	v.texCoord.Set(UXZ, 0);//Bottom right
	vertex_buffer_data.push_back(v);//14

	v.pos.Set(-lengthX, lengthY, lengthZ);//Front top left
	v.color = color;
	v.normal.Set(0, 1, 0);
	v.texCoord.Set(0, 0);//Bottom left
	vertex_buffer_data.push_back(v);//15

	//-------------------------------Stage 3

	v.pos.Set(-lengthX, -lengthY, -lengthZ);//Rear bottom left
	v.color = color;
	v.normal.Set(0, 0, -1);
	v.texCoord.Set(UXY, 0);//Bottom right
	vertex_buffer_data.push_back(v);//16

	v.pos.Set(lengthX, -lengthY, -lengthZ);//Rear bottom right
	v.color = color;
	v.normal.Set(0, 0, -1);
	v.texCoord.Set(0, 0);//Bottom left
	vertex_buffer_data.push_back(v);//17

	v.pos.Set(lengthX, lengthY, -lengthZ);//Rear top right
	v.color = color;
	v.normal.Set(0, 0, -1);
	v.texCoord.Set(0, VXY);//Top left
	vertex_buffer_data.push_back(v);//18

	v.pos.Set(-lengthX, lengthY, -lengthZ);//Rear top left
	v.color = color;
	v.normal.Set(0, 0, -1);
	v.texCoord.Set(UXY, VXY);// Top right
	vertex_buffer_data.push_back(v);//19

	v.pos.Set(-lengthX, -lengthY, lengthZ);//Front bottom left
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 0);//Bottom left
	vertex_buffer_data.push_back(v);//20

	v.pos.Set(lengthX, -lengthY, lengthZ);//Front bottom right
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(UXY, 0);//Bottom right
	vertex_buffer_data.push_back(v);//21

	v.pos.Set(lengthX, lengthY, lengthZ);//Front top right
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(UXY, VXY);// Top right
	vertex_buffer_data.push_back(v);//22

	v.pos.Set(-lengthX, lengthY, lengthZ);//Front top left
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, VXY);//Top left
	vertex_buffer_data.push_back(v);//23

	//Index buffer..........................................

	index_buffer_data.push_back(22);//Front face t1
	index_buffer_data.push_back(20);
	index_buffer_data.push_back(21);

	index_buffer_data.push_back(22);//Front face t2
	index_buffer_data.push_back(23);
	index_buffer_data.push_back(20);

	index_buffer_data.push_back(7);//Left face t1
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(4);

	index_buffer_data.push_back(7);//Left face t2
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);

	index_buffer_data.push_back(2);//Right face t1
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(1);

	index_buffer_data.push_back(2);//Right face t2
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);

	index_buffer_data.push_back(19);//Back face t1
	index_buffer_data.push_back(17);
	index_buffer_data.push_back(16);

	index_buffer_data.push_back(19);//Back face t2
	index_buffer_data.push_back(18);
	index_buffer_data.push_back(17);

	index_buffer_data.push_back(10);//Top face t1
	index_buffer_data.push_back(15);
	index_buffer_data.push_back(14);

	index_buffer_data.push_back(10);//Top face t2
	index_buffer_data.push_back(11);
	index_buffer_data.push_back(15);

	index_buffer_data.push_back(13);//Bottom face t1
	index_buffer_data.push_back(8);
	index_buffer_data.push_back(9);

	index_buffer_data.push_back(13);//Bottom face t2
	index_buffer_data.push_back(12);
	index_buffer_data.push_back(8);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a circle; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param radius - radius of circle
\param numSlice - Number of slices used to generate the circle

\return Pointer to mesh storing VBO/IBO of circle
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCircle(const std::string &meshName, Color color, unsigned numSlice, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerSlice = 360.f / numSlice;

	for (unsigned slice = 0; slice < numSlice; slice++)
	{
		//v0
		float theta = slice * degreePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

		//v1
		v.pos.Set(0, 0, 0);
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

		//v2
		float theta2 = (slice + 1) * degreePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta2)), 0, radius * sin(Math::DegreeToRadian(theta2)));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (unsigned slice = 0; slice < numSlice; slice++)
	{
		index_buffer_data.push_back(slice * 3 + 0);
		index_buffer_data.push_back(slice * 3 + 1);
		index_buffer_data.push_back(slice * 3 + 2);
	}


	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a ring; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param innerRadius - inner radius of ring
\param outerRadius - outer radius of ring
\param numSlice - Number of slices used to generate the ring

\return Pointer to mesh storing VBO/IBO of ring
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float innerRadius, float outerRadius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerSlice = 360.f / numSlice;

	for (unsigned slice = 0; slice < numSlice; slice++)
	{
		//v0
		float theta = slice * degreePerSlice;
		v.pos.Set(outerRadius * cos(Math::DegreeToRadian(theta)), 0, outerRadius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

		//v1
		v.pos.Set(innerRadius * cos(Math::DegreeToRadian(theta)), 0, innerRadius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

		//v2
		float theta2 = (slice + 1) * degreePerSlice;
		v.pos.Set(outerRadius * cos(Math::DegreeToRadian(theta2)), 0, outerRadius * sin(Math::DegreeToRadian(theta2)));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

		//v3
		v.pos.Set(innerRadius * cos(Math::DegreeToRadian(theta2)), 0, innerRadius * sin(Math::DegreeToRadian(theta2)));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (unsigned slice = 0; slice < numSlice; slice++)
	{
		index_buffer_data.push_back(slice * 4 + 0);
		index_buffer_data.push_back(slice * 4 + 1);
		index_buffer_data.push_back(slice * 4 + 2);
		index_buffer_data.push_back(slice * 4 + 3);
		index_buffer_data.push_back(slice * 4 + 2);
		index_buffer_data.push_back(slice * 4 + 1);
	}


	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

float sphereX(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta));
}

float sphereY(float phi, float theta)
{
	return sin(Math::DegreeToRadian(phi));
}

float sphereZ(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta));
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a sphere; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param radius - radius of sphere
\param numSlice - Number of slices used to generate the sphere
\param numStack - Number of stacks used to generate the sphere

\return Pointer to mesh storing VBO/IBO of sphere
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerSlice = 360.f / numSlice;
	float degreePerStack = 180.f / numStack;

	for (unsigned stack = 0; stack < numStack + 1; stack++)
	{
		float phi = -90 + stack * degreePerStack;
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			//v0
			float theta = slice * degreePerSlice;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.color = color;
			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			vertex_buffer_data.push_back(v);
		}
	}

	for (unsigned stack = 0; stack < numStack; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			index_buffer_data.push_back(stack * (numSlice + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlice + 1) + slice);
		}
	}


	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a part of a Sphere; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param radius - radius of a part of a Sphere
\param numSlice - Number of slices used to generate a part of a Sphere
\param numStack - Number of stacks used to generate a part of a Sphere
\param degreePerSlice - Degrees per slice
\param degreePerStack - Degrees per stack

\return Pointer to mesh storing VBO/IBO of a part of a Sphere
*/
/******************************************************************************/
Mesh* MeshBuilder::GeneratePartofSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius, float degreePerSlice, float degreePerStack)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	degreePerSlice = degreePerSlice / numSlice;
	degreePerStack = degreePerStack / numStack;

	for (unsigned stack = 0; stack < numStack + 1; stack++)
	{
		float phi = -90 + stack * degreePerStack;
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			//v0
			float theta = slice * degreePerSlice;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.color = color;
			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			vertex_buffer_data.push_back(v);
		}
	}

	for (unsigned stack = 0; stack < numStack; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			index_buffer_data.push_back(stack * (numSlice + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlice + 1) + slice);
		}
	}


	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

Mesh* MeshBuilder::GenerateCone(const std::string &meshName, Color color, unsigned numSlice, float radius, float height)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerSlice = 360.f / numSlice;

	for (unsigned slice = 0; slice < numSlice; slice++)
	{
		//v0
		float theta = slice * degreePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

		//v1
		v.pos.Set(0, height, 0);
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

		//v2
		float theta2 = (slice + 1) * degreePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta2)), 0, radius * sin(Math::DegreeToRadian(theta2)));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (unsigned slice = 0; slice < numSlice; slice++)
	{
		index_buffer_data.push_back(slice * 3 + 0);
		index_buffer_data.push_back(slice * 3 + 1);
		index_buffer_data.push_back(slice * 3 + 2);
	}


	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

float CylinderX(float R, float theta) {
	return R * cos(Math::DegreeToRadian(theta));
}

float CylinderZ(float R, float theta) {
	return R * sin(Math::DegreeToRadian(theta));
}
/******************************************************************************/
/*!
\brief
Generate the vertices of a cylinder; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param radius - radius of cylinder
\param height - height of the cylinder
\param numStack - Number of stacks used to generate the cylinder

\return Pointer to mesh storing VBO/IBO of the cylinder
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCylinder(const std::string &meshName, Color color, unsigned numStack, float radius, float height)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float stackheight = height / numStack;

	for (unsigned stack = 0; stack < numStack; ++stack)
	{
		for (float theta = 0; theta <= 360; theta += 10)
		{
			v.pos.Set(radius * CylinderX(radius, theta), -height / 2 + stack * stackheight, radius * CylinderZ(radius, theta));
			v.color = color;
			v.normal.Set(CylinderX(radius, theta), 0, CylinderZ(radius, theta));
			vertex_buffer_data.push_back(v);

			v.pos.Set(radius * CylinderX(radius, theta), -height / 2 + (stack + 1) * stackheight, radius * CylinderZ(radius, theta));
			v.color = color;
			v.normal.Set(CylinderX(radius, theta), 0, CylinderZ(radius, theta));
			vertex_buffer_data.push_back(v);
		}
	}

	for (float theta = 0; theta <= 360; theta += 10)
	{
		v.pos.Set(radius * CylinderX(radius, theta), height / 2, radius * CylinderZ(radius, theta));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, height / 2, 0);
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (float theta = 0; theta <= 360; theta += 10)
	{
		v.pos.Set(0, -height / 2, 0);
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(radius * CylinderX(radius, theta), -height / 2, radius * CylinderZ(radius, theta));
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (unsigned i = 0; i < vertex_buffer_data.size(); ++i)
	{
		index_buffer_data.push_back(i);
	}


	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}
/******************************************************************************/
/*!
\brief
Generate the vertices of a hollow cylinder; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param innerR - inner radius of hollow cylinder
\param outerR - outer radius of hollow cylinder
\param height - height of the hollow cylinder
\param numStack - Number of stacks used to generate the hollow cylinder

\return Pointer to mesh storing VBO/IBO of the hollow cylinder
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateHollowCylinder(const std::string &meshName, Color color, unsigned numStack, float innerR, float outerR, float height)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float stackheight = height / numStack;

	for (unsigned stack = 0; stack < numStack; ++stack)
	{
		for (float theta = 360; theta >= 0; theta -= 10)
		{
			v.pos.Set(innerR * CylinderX(innerR, theta), -height / 2 + stack * stackheight, innerR * CylinderZ(innerR, theta));
			v.color = color;
			v.normal.Set(-CylinderX(innerR, theta), 0, -CylinderZ(innerR, theta));
			vertex_buffer_data.push_back(v);

			v.pos.Set(innerR * CylinderX(innerR, theta), -height / 2 + (stack + 1) * stackheight, innerR * CylinderZ(innerR, theta));
			v.color = color;
			v.normal.Set(-CylinderX(innerR, theta), 0, -CylinderZ(innerR, theta));
			vertex_buffer_data.push_back(v);
		}
	}

	//OUTER

	for (unsigned stack = 0; stack < numStack; ++stack)
	{
		for (float theta = 0; theta <= 360; theta += 10)
		{
			v.pos.Set(outerR * CylinderX(outerR, theta), -height / 2 + stack * stackheight, outerR * CylinderZ(outerR, theta));
			v.color = color;
			v.normal.Set(CylinderX(outerR, theta), 0, CylinderZ(outerR, theta));
			vertex_buffer_data.push_back(v);

			v.pos.Set(outerR * CylinderX(outerR, theta), -height / 2 + (stack + 1) * stackheight, outerR * CylinderZ(outerR, theta));
			v.color = color;
			v.normal.Set(CylinderX(outerR, theta), 0, CylinderZ(outerR, theta));
			vertex_buffer_data.push_back(v);
		}
	}



	for (unsigned i = 0; i < vertex_buffer_data.size(); ++i)
	{
		index_buffer_data.push_back(i);
	}


	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const std::string &file_path) {
	//Read vertices, texcoords & normals from OBJ
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if (!success)
		return NULL;
	//Index the vertices, texcoords & normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);
	//Create the mesh and call glBindBuffer, glBufferData
	//Use triangle list and remember to set index size
	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a hollow cylinder; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param numRow - number of rows of text
\param numCol - number of collumns of text

\return Pointer to mesh storing VBO/IBO of the hollow cylinder
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float width = 1.f / numCol;
	float height = 1.f / numRow;
	int offset = 0;
	for (unsigned i = 0; i < numRow; ++i)
	{
		for (unsigned j = 0; j < numCol; ++j)
		{
			/*1/numCol;
			1/numRow;
			1/16;

			0 1				1/16 1

			0 1-1/16		1/16 1-1/16

			*/
			float U = j * width;
			float V = 1.f - height - i * height;





			v.pos.Set(-0.5, -0.5, 0.0f);//Bottom Left
			//v.color = color;
			//v.normal.Set(0, 0, 1);
			v.texCoord.Set(U, V);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5, -0.5, 0.0f);//Bottom right
			//v.color = color;
			//v.normal.Set(0, 0, 1);
			v.texCoord.Set(U + width, V);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5, 0.5, 0.0f);//Top right
			//v.color = color;
			//v.normal.Set(0, 0, 1);
			v.texCoord.Set(U + width, V + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5, 0.5, 0.0f);//Top left
			//v.color = color;
			//v.normal.Set(0, 0, 1);
			v.texCoord.Set(U, V + height);
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			offset += 4;
		}
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

// Skybox
Mesh* MeshBuilder::GenerateSkybox(const std::string &meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;

	v.pos.Set(-0.5f, -0.5f, 0.0f);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f, -0.5f, 0.0f);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(1, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f, 0.5f, 0.0f);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(1, 1);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f, 0.5f, 0.0f);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 1);
	vertex_buffer_data.push_back(v);


	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);


	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

Mesh* MeshBuilder::GenerateSkyPlane(const std::string &meshName, Color color, int slices, float PlanetRadius, float AtmosphereRadius, float hTile, float vTile)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float planeSize = 2.0f *(float)sqrtf((AtmosphereRadius*AtmosphereRadius) - (PlanetRadius*PlanetRadius));

	float delta = planeSize / (float)slices;
	float texDelta = 2.0f / (float)slices;

	for (int z = 0; z <= slices; ++z)
	{
		for (int x = 0; x <= slices; ++x)
		{
			float xDist = (-0.5f * planeSize) + ((float)x * delta);
			float zDist = (-0.5f * planeSize) + ((float)z * delta);

			float xHeight = (xDist * xDist) / AtmosphereRadius;
			float zHeight = (zDist * zDist) / AtmosphereRadius;
			float height = xHeight + zHeight;

			Vertex tv;

			tv.pos.x = xDist;
			tv.pos.y = 0.0f - height;
			tv.pos.z = zDist;

			tv.texCoord.u = hTile * ((float)x * texDelta * 0.5f);
			tv.texCoord.v = vTile * (1.0f - (float)z * texDelta * 0.5f);

			tv.color = color;

			vertex_buffer_data.push_back(tv);
		}
	}

	for (int i = 0; i < slices; ++i)
	{
		for (int j = 0; j < slices; ++j)
		{
			int startvert = (i * (slices + 1) + j);
			index_buffer_data.push_back(startvert);
			index_buffer_data.push_back(startvert + 1);
			index_buffer_data.push_back(startvert + slices + 1);

			index_buffer_data.push_back(startvert + 1);
			index_buffer_data.push_back(startvert + slices + 2);
			index_buffer_data.push_back(startvert + slices + 1);
		}
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

Mesh* MeshBuilder::GenerateTerrain(const std::string &meshName, const std::string &file_path, std::vector<unsigned char> &heightMap)
{
	if (!LoadHeightMap(file_path.c_str(), heightMap))
	{
		return NULL;
	}

	const float SCALE_FACTOR = 256.f;
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	unsigned terrainSize = (unsigned)sqrt((double)heightMap.size());

	for (unsigned z = 0; z < terrainSize; ++z)
	{
		for (unsigned x = 0; x < terrainSize; ++x)
		{
			float scaledHeight = (float)heightMap[z * terrainSize + x] / SCALE_FACTOR;
			v.pos.Set(static_cast<float>(x) / terrainSize - 0.5f, scaledHeight, static_cast<float>(z) / terrainSize - 0.5f);
			v.normal.Set(0, 1, 0);
			v.color.Set(scaledHeight, scaledHeight, scaledHeight);

			v.texCoord.Set((float)x / terrainSize * 8, 1.f - (float)z / terrainSize * 8);
			vertex_buffer_data.push_back(v);
		}
	}

	for (unsigned z = 0; z < terrainSize - 1; ++z)
	{
		for (unsigned x = 0; x < terrainSize - 1; ++x)
		{
			index_buffer_data.push_back(terrainSize * z + x);
			index_buffer_data.push_back(terrainSize * (z + 1) + x);
			index_buffer_data.push_back(terrainSize * z + x + 1);

			index_buffer_data.push_back(terrainSize * (z + 1) + x + 1);
			index_buffer_data.push_back(terrainSize * z + x + 1);
			index_buffer_data.push_back(terrainSize * (z + 1) + x);
		}
	}

	Vector3 firstPos, secondPos, thirdPos, normal;
	int firstPoint = 0, secondPoint = 0, thirdPoint = 0;
	for (unsigned z = 0; z < terrainSize - 1; ++z)
	{
		for (unsigned x = 1; x < terrainSize; ++x)
		{
			firstPoint = x + z * terrainSize;
			secondPoint = (x - 1) + (z * terrainSize);
			thirdPoint = x + (z + 1) * terrainSize;

			//First Point
			firstPos.x = vertex_buffer_data[firstPoint].pos.x;
			firstPos.y = vertex_buffer_data[firstPoint].pos.y;
			firstPos.z = vertex_buffer_data[firstPoint].pos.z;

			//Second Point
			secondPos.x = vertex_buffer_data[secondPoint].pos.x - firstPos.x;
			secondPos.y = vertex_buffer_data[secondPoint].pos.y - firstPos.y;
			secondPos.z = vertex_buffer_data[secondPoint].pos.z - firstPos.z;

			//Third Point
			thirdPos.x = vertex_buffer_data[thirdPoint].pos.x - firstPos.x;
			thirdPos.y = vertex_buffer_data[thirdPoint].pos.y - firstPos.y;
			thirdPos.z = vertex_buffer_data[thirdPoint].pos.z - firstPos.z;

			//Finding normal
			normal = secondPos.Cross(thirdPos);
			normal.Normalize();
			vertex_buffer_data[firstPoint].normal = normal;
		}
	}

	Vector3 aNormal; // Average Normal
	Vector3 firstNormal, secondNormal, thirdNormal;
	for (unsigned i = 0; i < index_buffer_data.size(); i += 3)
	{
		firstNormal = vertex_buffer_data[index_buffer_data[i]].normal;
		secondNormal = vertex_buffer_data[index_buffer_data[i + 1]].normal;
		thirdNormal = vertex_buffer_data[index_buffer_data[i + 2]].normal;

		//Getting all three normal together
		aNormal = firstNormal + secondNormal + thirdNormal;

		//Finding the average value
		aNormal.x /= 3;
		aNormal.y /= 3;
		aNormal.z /= 3;

		//Putting the values back
		vertex_buffer_data[index_buffer_data[i]].normal = aNormal;
		vertex_buffer_data[index_buffer_data[i + 1]].normal = aNormal;
		vertex_buffer_data[index_buffer_data[i + 2]].normal = aNormal;
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}

SpriteAnimation* MeshBuilder::GenerateSpriteAnimation(const std::string &meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float width = 1.f / numCol;
	float height = 1.f / numRow;
	int offset = 0;
	for (unsigned i = 0; i < numRow; ++i)
	{
		for (unsigned j = 0; j < numCol; ++j)
		{
			float U = j * width;
			float V = 1.f - height - i * height;


			v.pos.Set(-0.5, -0.5, 0.0f);//Bottom Left
			//v.color = color;
			//v.normal.Set(0, 0, 1);
			v.texCoord.Set(U, V);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5, -0.5, 0.0f);//Bottom right
			//v.color = color;
			//v.normal.Set(0, 0, 1);
			v.texCoord.Set(U + width, V);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5, 0.5, 0.0f);//Top right
			//v.color = color;
			//v.normal.Set(0, 0, 1);
			v.texCoord.Set(U + width, V + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5, 0.5, 0.0f);//Top left
			//v.color = color;
			//v.normal.Set(0, 0, 1);
			v.texCoord.Set(U, V + height);
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			offset += 4;
		}
	}

	SpriteAnimation *mesh = new SpriteAnimation(meshName, numRow, numCol);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	std::vector<Vertex>().swap(vertex_buffer_data);
	std::vector<GLuint>().swap(index_buffer_data);

	return mesh;
}