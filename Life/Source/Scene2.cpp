#include "Scene2.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "MyMath.h"
#include "Application.h"
#include <math.h>


#define PI 3.14159265

int rotationspeedboost = 1;
int TURBBOOOOO = 7;

inline int RandIntMinMax (int min, int max)//I apologise for adding the two functions from MyMath.h here. I tried #include "MyMath.h" and still doesn't like me to use the function
{
	int num = rand() % (max - min + 1);
	return (num + min);
}//end of RandIntMinMax function

inline float RandFloat (void)
{
	return rand() / static_cast <float> (RAND_MAX);
}//end of RandFloat function

Scene2::Scene2()
{
}

Scene2::~Scene2()
{
}

Scene2 T1, T2, T3;

void Scene2::Init()
{
	T1.rotateAngle;
	// Init VBO here
	glClearColor(0.121f, 0.0f, 0.18f, 0.0f); //background color

	glEnable(GL_DEPTH_TEST);
	
	//load shaders
	m_programID = LoadShaders("Shader//TransformVertexShader.vertexshader", "Shader//SimpleFragmentShader.fragmentshader");
	glUseProgram(m_programID);

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//gen buffer
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);


	//================================================================================================================================================================

	static const GLfloat vertex_buffer_data1[] = { //points of Triangle 1
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data1), vertex_buffer_data1, GL_STATIC_DRAW);

	//array of 3 vectors, colors of the 3 vertices
	static const GLfloat color_buffer_data1[] = { //color
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	T1.rotateAngle = 0;
	T1.translateX = 60;
	T1.translateY = 60;
	T1.scaleAll = 1;

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data1), color_buffer_data1, GL_STATIC_DRAW);

	//================================================================================================================================================================

	static const GLfloat vertex_buffer_data2[] = { //points of Triangle 2
		0.0f, 1.0f, 0.0f,
		-0.8f, -1.0f, 0.0f,
		0.8f, -1.0f, 0.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data2), vertex_buffer_data2, GL_STATIC_DRAW);

	//array of 3 vectors, colors of the 3 vertices
	static const GLfloat color_buffer_data2[] = { //color
		1.0f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
	};

	T2.rotateAngle = 0;
	T2.translateX = 0;
	T2.translateY = 0;
	T2.scaleAll = 1.2;

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data2), color_buffer_data2, GL_STATIC_DRAW);

	//================================================================================================================================================================
	//================================================================================================================================================================

	static const GLfloat vertex_buffer_data3[] = { //points of Triangle 3
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data3), vertex_buffer_data3, GL_STATIC_DRAW);

	//array of 3 vectors, colors of the 3 vertices
	static const GLfloat color_buffer_data3[] = { //color
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	T3.rotateAngle = 0;
	T3.translateX = -70;
	T3.translateY = 50;
	T3.scaleAll = 1;

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data3), color_buffer_data3, GL_STATIC_DRAW);

	//================================================================================================================================================================
}

void Scene2::Update(double dt)
{
	if(Application::IsKeyPressed(VK_CONTROL))
	{
		rotationspeedboost = 2;
	}

	else
	{
		rotationspeedboost = 1;
	}

	if(Application::IsKeyPressed(VK_SHIFT))
	{
		TURBBOOOOO = 18;
	}

	else
	{
		TURBBOOOOO = 7;
	}

	if(Application::IsKeyPressed(VK_SPACE)) //TIME HAX BRO
	{
		dt *= 3;
		glClearColor(RandFloat(), RandFloat(), RandFloat(), 0.0f); //Enable if you want eye cancer
	}

	T3.translateY += (float)(-5 * dt);
	T3.rotateAngle += (float)(360 * dt);
	T1.translateY += (float)(-9 * dt);
	T1.rotateAngle += (float)(450 * dt);

	if(Application::IsKeyPressed(VK_UP))
	{
		T2.translateY += ( sin((T2.rotateAngle + 90) * PI/180) ) * dt * TURBBOOOOO;
		T2.translateX += ( cos((T2.rotateAngle + 90) * PI/180) ) * dt * TURBBOOOOO;
	}

	if(Application::IsKeyPressed(VK_DOWN))
	{
		T2.translateY -= ( sin((T2.rotateAngle + 90) * PI/180) ) * dt * TURBBOOOOO;
		T2.translateX -= ( cos((T2.rotateAngle + 90) * PI/180) ) * dt * TURBBOOOOO;
	}

	if(Application::IsKeyPressed(VK_LEFT) && (Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed(VK_DOWN)))
	{
		if(Application::IsKeyPressed(VK_UP))
		{
			T2.rotateAngle += (float)(100 * dt * rotationspeedboost);
		}

		else
		{
			T2.rotateAngle -= (float)(100 * dt * rotationspeedboost);
		}
	}

	if(Application::IsKeyPressed(VK_RIGHT) && (Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed(VK_DOWN)))
	{
		if(Application::IsKeyPressed(VK_UP))
		{
			T2.rotateAngle += (float)(-100 * dt * rotationspeedboost);
		}

		else
		{
			T2.rotateAngle -= (float)(-100 * dt * rotationspeedboost);
		}
	}

	//boundary protection==================================

	if(T3.translateY < -50) 
	{
		T3.translateY = 50;
		T3.translateX = RandIntMinMax(-75, 75);
	}

	if(T1.translateY < -50)
	{
		T1.translateY = 50;
		T1.translateX = RandIntMinMax(-75, 75);
	}

	if(T2.translateX > 85)
	{
		T2.translateX = -85;
	}

	if(T2.translateX < -85)
	{
		T2.translateX = 85;
	}

	if(T2.translateY > 50)
	{
		T2.translateY = -50;
	}

	if(T2.translateY < -50)
	{
		T2.translateY = 50;
	}
}

void Scene2::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;

	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();
	view.SetToIdentity();

	projection.SetToOrtho(-80, 80, -45, 45, -5, 5);


	//=======================================================================================================

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//=======================================================================================================
	//TRIANGLE1
	scale.SetToScale(T1.scaleAll, T1.scaleAll, 1);
	rotate.SetToRotation(T1.rotateAngle, 0, 0, 1);
	translate.SetToTranslation(T1.translateX, T1.translateY, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//=======================================================================================================
	//TRIANGLE2

	scale.SetToScale(T2.scaleAll, T2.scaleAll, 1);
	rotate.SetToRotation(T2.rotateAngle, 0, 0, 1);
	translate.SetToTranslation(T2.translateX, T2.translateY, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//=======================================================================================================
	//TRIANGLE3

	scale.SetToScale(T3.scaleAll, T3.scaleAll, 1);
	rotate.SetToRotation(T3.rotateAngle, 0, 0, 1);
	translate.SetToTranslation(T3.translateX, T3.translateY, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_3]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//=======================================================================================================

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);//
}

void Scene2::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
