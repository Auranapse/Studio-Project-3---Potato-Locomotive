#include "Scene1.h"
#include "GL\glew.h"

#include "shader.hpp"

Scene1::Scene1()
{
}

Scene1::~Scene1()
{
}

void Scene1::Init()
{
	// Init VBO here
	glClearColor(0.121f, 0.0f, 0.18f, 0.0f); //background color

	glEnable(GL_DEPTH_TEST);
	//load shaders
	m_programID = LoadShaders("Shader//SimpleVertexShader.vertexshader", "Shader//SimpleFragmentShader.fragmentshader");
	glUseProgram(m_programID);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//gen buffer
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	
	//================================================================================================================================================================

	static const GLfloat vertex_buffer_data[] = { //points of triangle
		-0.7f, 0.3f, 0.5f,
		-0.4f, 0.0f, 0.5f,
		-0.4f, 0.5f, 0.5f,

		-0.1f, 0.3f, 0.5f,
		-0.4f, 0.0f, 0.5f,
		0.0f, 0.0f, 0.5f,

		-0.1f, -0.3f, 0.5f,
		-0.4f, 0.0f, 0.5f,
		-0.4f, -0.5f, 0.5f,

		-0.7f, -0.3f, 0.5f,
		-0.4f, 0.0f, 0.5f,
		-0.8f, 0.0f, 0.5f,

		-0.7f, 0.3f, 0.5f,
		-0.4f, 0.0f, 0.5f,
		-0.4f, 0.5f, 0.5f,
	};

	

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

	//array of 3 vectors, colors of the 3 vertices
	static const GLfloat color_buffer_data[] = { //color of triangle
		1.0f, 0.23f, 0.98f,
		1.0f, 0.66f, 0.27f,
		1.0f, 0.41f, 0.9f,

		1.0f, 0.23f, 0.98f,
		1.0f, 0.66f, 0.27f,
		1.0f, 0.41f, 0.9f,

		1.0f, 0.23f, 0.98f,
		1.0f, 0.66f, 0.27f,
		1.0f, 0.41f, 0.9f,

		1.0f, 0.23f, 0.98f,
		1.0f, 0.66f, 0.27f,
		1.0f, 0.41f, 0.9f,

		1.0f, 0.23f, 0.98f,
		1.0f, 0.66f, 0.27f,
		1.0f, 0.41f, 0.9f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);

	//================================================================================================================================================================
	
	static const GLfloat vertex_buffer_data2[] = { //points of star MIDDLE
		0.3f, 0.2f, -0.7f,
		0.4f, 0.0f, -0.7f,
		0.5f, 0.2f, -0.7f,

		0.6f, 0.0f, -0.7f,
		0.4f, -0.2f, -0.7f,
		0.2f, 0.0f, -0.7f,

		0.3f, 0.2f, -0.7f,
		0.4f, 0.0f, -0.7f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data2), vertex_buffer_data2, GL_STATIC_DRAW);

	//array of 3 vectors, colors of the 3 vertices
	static const GLfloat color_buffer_data2[] = { //color of STAR MIDDLE
		1.0f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data2), color_buffer_data2, GL_STATIC_DRAW);

	//===========================================================================================================================================================

	static const GLfloat vertex_buffer_data3[] = { //points of star Edges
		0.3f, 0.2f, -0.7f,
		0.4f, 0.5f, -0.7f,
		0.5f, 0.2f, -0.7f,

		0.5f, 0.2f, -0.7f,
		0.8f, 0.2f, -0.7f,
		0.6f, 0.0f, -0.7f,

		0.6f, 0.0f, -0.7f,
		0.7f, -0.4f, -0.7f,
		0.4f, -0.2f, -0.7f,

		0.4f, -0.2f, -0.7f,
		0.1f, -0.4f, -0.7f,
		0.2f, 0.0f, -0.7f,

		0.2f, 0.0f, -0.7f,
		0.0f, 0.2f, -0.7f,
		0.3f, 0.2f, -0.7f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data3), vertex_buffer_data3, GL_STATIC_DRAW);

	//array of 3 vectors, colors of the 3 vertices
	static const GLfloat color_buffer_data3[] = { //color of STAR EDGES
		1.0f, 0.5f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.5f, 0.0f,

		1.0f, 0.5f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.5f, 0.0f,

		1.0f, 0.5f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.5f, 0.0f,

		1.0f, 0.5f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.5f, 0.0f,

		1.0f, 0.5f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.5f, 0.0f,

		1.0f, 0.5f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.5f, 0.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data3), color_buffer_data3, GL_STATIC_DRAW);
	

}

void Scene1::Update(double dt)
{

}

void Scene1::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glEnableVertexAttribArray(0);//OCTAGON
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 15);

	//=======================================================================================================



	//STAR MIDDLE
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);

	//=======================================================================================================


	//STAR EDGES
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_3]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 15);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);//
}

void Scene1::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
