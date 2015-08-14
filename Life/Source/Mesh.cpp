/******************************************************************************/
/*!
\file	Mesh.cpp
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Mesh stuff
*/
/******************************************************************************/
#include "Mesh.h"
#include "GL\glew.h"
#include "Vertex.h"

/******************************************************************************/
/*!
\brief
Default constructor - generate VBO/IBO here

\param meshName - name of mesh
*/
/******************************************************************************/
Mesh::Mesh(const std::string &meshName)
	: name(meshName)
	, mode(DRAW_TRIANGLES)
{
	// Generate buffers
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);


	for(unsigned i = 0; i < NUM_TEXTURES; ++i)
	{
		textureID[i] = 0;
	}
}

/******************************************************************************/
/*!
\brief
Destructor - delete VBO/IBO here
*/
/******************************************************************************/
Mesh::~Mesh()
{
	// Cleanup VBO here
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	//if(textureID > 0)

	for(unsigned i = 0; i < NUM_TEXTURES; ++i)
	{
		glDeleteTextures(1, &textureID[i]);
	}
}

/******************************************************************************/
/*!
\brief
OpenGL render code
*/
/******************************************************************************/
void Mesh::Render()
{
	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	//Render call setup
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));

	//if(textureID > 0)

	
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));


	//Actual render call
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	if(mode == DRAW_TRIANGLES)
	{
		glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
	}
	else if(mode == DRAW_TRIANGLE_STRIP)
	{
		glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, 0);
	}
	else if(mode == DRAW_LINES)
	{
		glDrawElements(GL_LINES, indexSize, GL_UNSIGNED_INT, 0);
	}

	//if(textureID > 0)
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

/******************************************************************************/
/*!
\brief
OpenGL render code
*/
/******************************************************************************/
void Mesh::Render(unsigned offset, unsigned count)
{
	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	//Render call setup

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
	
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));


	//Actual render call
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	if(mode == DRAW_LINES)
		glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
	else if(mode == DRAW_TRIANGLE_STRIP)
		glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
	else
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));

	//if(textureID > 0)

	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
