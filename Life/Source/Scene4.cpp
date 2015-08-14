#include "Scene4.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"


Scene4::Scene4()
{

}

Scene4::~Scene4()
{

}

Scene4 Sun, Earth, Moon, Sataon, Sataring, Satamoon, Orbitron, OM1, OM2, GEOJOE;

void Scene4::Init()
{
	// Init VBO here

	
	
	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.1f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	
	//Load vertex and fragment shaders
	m_programID = LoadShaders( "Shader//TransformVertexShader.vertexshader", "Shader//SimpleFragmentShader.fragmentshader" );
	// Use our shader
	glUseProgram(m_programID);
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	

	//variable to rotate geometry
	Sun.rotateA = 0;
	Earth.rotateA = 0;
	Earth.orbit = -40;
	Moon.rotateA = 0;
	Moon.orbit = 0;
	Sataon.rotateA = 90;
	Sataon.orbit = 0;
	Satamoon.orbit = 0;
	Satamoon.rotateA = 0;
	Orbitron.orbit = 180;
	Orbitron.rotateA = 0;
	OM1.orbit = 0;
	OM2.orbit = 180;
	OM1.rotateA = 0;
	OM2.rotateA = 0;
	GEOJOE.orbit = 100;
	GEOJOE.rotateA = 0;
	
	//Initialize camera settings
	camera.Init(Vector3(200, 100, 200), Vector3(0, 0, 0), Vector3(0, 1, 0));

	//Set projection matrix to perspective mode
	Mtx44 projection;
	projection.SetToPerspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f); //FOV, Aspect Ratio, Near plane, Far plane
	projectionStack.LoadMatrix(projection);

	//remove all glGenBuffers, glBindBuffer, glBufferData code
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	//meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 0, 0), 1, 1);
	//meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 0, 0), 1,1,1);
	//meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(1, 0, 0), 64, 1);
	//meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 0), 64, 1, 2);
	meshList[GEO_SUN] = MeshBuilder::GenerateSphere("SUNSABEACHES", Color(1, 1, 0), 18, 36, 1);
	meshList[GEO_EARTH] = MeshBuilder::GenerateSphere("EARTHIZLYFE", Color(0, 0.2, 1), 18, 36, 1);
	meshList[GEO_MOON] = MeshBuilder::GenerateSphere("GREYTHING", Color(0.5, 0.5, 0.5), 18, 36, 1);

	meshList[GEO_SATAON] = MeshBuilder::GenerateSphere("Theplanetwithrings", Color(0.83, 0.63, 0.21), 18, 36, 1);
	meshList[GEO_SATAONR] = MeshBuilder::GenerateRing("TheringsofSATAON", Color(0.83, 0.73, 0.31), 64, 1.5, 2.0);
	meshList[GEO_SATAMOON] = MeshBuilder::GenerateSphere("ThemoonofSATAON", Color(0.06, 0.81, 0.78), 18, 36, 1);

	meshList[GEO_ORBITRON] = MeshBuilder::GenerateSphere("Orbitron", Color(0.71, 0.43, 0.43), 18, 36, 1);
	meshList[GEO_ORBITRON_MOON1] = MeshBuilder::GenerateSphere("OrbitronM1", Color(2.1, 0.0, 0.0), 18, 36, 1);
	meshList[GEO_ORBITRON_MOON2] = MeshBuilder::GenerateSphere("OrbitronM2", Color(0.43, 0.47, 0.0), 18, 36, 1);

	meshList[GEO_MRJOE] = MeshBuilder::GenerateSphere("ITSMRJOE", Color(0.48, 0.38, 0.50), 18, 36, 1);
}

void Scene4::Update(double dt)
{
	if(Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	if(Application::IsKeyPressed(VK_SHIFT))
	{
		dt *= 5;
	}

	if(Application::IsKeyPressed(VK_CONTROL))
	{
		dt *= 5;
	}

	//rotateAngle += (float)(10 * dt);

	Sun.rotateA += (float) 10 * dt;
	Earth.rotateA += (float) 10 * dt;
	Earth.orbit += (float) 36.5 * dt;
	Moon.rotateA += (float) 360 * dt;
	Sataon.rotateA += (float) 6 * dt;
	Sataon.orbit += (float) 3 * dt;

	Satamoon.rotateA += (float) 360 * dt;
	Satamoon.orbit -= (float) 50 * dt;

	Orbitron.orbit += (float) 1.5 * dt;
	Orbitron.rotateA += (float) 40 * dt;
	OM1.orbit += (float) 180 * dt;
	OM1.rotateA += (float) 80 * dt;
	OM2.orbit += (float) 90 * dt;
	OM2.rotateA += (float) 90 * dt;

	GEOJOE.orbit += (float) 1 * dt;
	GEOJOE.rotateA += (float) 10 * dt;

	camera.Update(dt);
}

void Scene4::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Temp variables
	Mtx44 translate, rotate, scale;
	Mtx44 MVP;

	//These will be replaced by matrix stack soon
	Mtx44 model;
	Mtx44 view;

	//Set all matrices to identity
	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();

	viewStack.LoadIdentity();

	//Set view matrix using camera settings
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
					);

	modelStack.LoadIdentity();

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP

	meshList[GEO_AXES]->Render();



	//meshList[GEO_QUAD]->Render();
	//meshList[GEO_CUBE]->Render();
	//meshList[GEO_CIRCLE]->Render();
	//meshList[GEO_RING]->Render();
	
	modelStack.PushMatrix();//----------SUN
	modelStack.Rotate(Sun.rotateA, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP
	meshList[GEO_SUN]->Render();
	modelStack.PopMatrix();//-----------SUN


	modelStack.PushMatrix();//EARTH AND MOON

	modelStack.Scale(0.5, 0.5, 0.5);
	modelStack.Rotate(Earth.orbit, 0, 1, 0);
	modelStack.Translate(200, 0, 0);
	modelStack.Rotate(Earth.rotateA, 0, 1, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP
	meshList[GEO_EARTH]->Render();

	modelStack.Scale(0.5, 0.5, 0.5);
	modelStack.Rotate(Moon.rotateA, 0, 1, 0);
	modelStack.Translate(20, 0, 0);
	modelStack.Rotate(Moon.rotateA, 0, 1, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP
	meshList[GEO_MOON]->Render();

	modelStack.PopMatrix();//EARTH AND MOON


	modelStack.PushMatrix();//oooooooooooooooooSataon

	modelStack.Rotate(10, 1, 0, 0);
	//Planet
	modelStack.Scale(4.5, 4.5, 4.5);
	modelStack.Rotate(Sataon.orbit, 0, 1, 0);
	modelStack.Translate(40, 0, 0);
	modelStack.Rotate(Sataon.rotateA, 0, 1, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();//
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);//
	meshList[GEO_SATAON]->Render();
	modelStack.PushMatrix();
	//Rings
	//modelStack.Rotate(Sataon.rotateA, 0, 1, 0);
	modelStack.Rotate(20, 1, 0, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();//
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);//
	meshList[GEO_SATAONR]->Render();

	modelStack.Rotate(180, 1, 0, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();//
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);//
	meshList[GEO_SATAONR]->Render();

	modelStack.Scale(1.5, 1.5, 1.5);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();//
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);//
	meshList[GEO_SATAONR]->Render();

	modelStack.Rotate(180, 1, 0, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();//
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);//
	meshList[GEO_SATAONR]->Render();

	modelStack.PopMatrix();

	modelStack.Rotate(20, 1, 0, 0);
	modelStack.Scale(0.1, 0.1, 0.1);
	modelStack.Rotate(Satamoon.orbit, 0, 1, 0);
	modelStack.Translate(50, 0, 0);
	modelStack.Rotate(Satamoon.rotateA, 0, 1, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();//
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);//
	meshList[GEO_SATAMOON]->Render();

	modelStack.PopMatrix();//ooooooooooooooooooSataon


	modelStack.PushMatrix();//555555555555555555555555555555555555 Orbitron

	modelStack.Rotate(-7, 1, 0, 0);
	
	modelStack.Rotate(Orbitron.orbit, 0, 1, 0);
	modelStack.Translate(220, 0, 0);
	modelStack.Rotate(Orbitron.rotateA, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();//
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);//
	meshList[GEO_ORBITRON]->Render();

	modelStack.PushMatrix();
	//Moon1
	modelStack.Rotate(10, 1, 0, 0);
	modelStack.Rotate(OM1.orbit, 0, 1, 0);
	modelStack.Translate(5, 0, 0);
	modelStack.Rotate(OM1.rotateA, 0, 1, 0);
	modelStack.Scale(0.3, 0.3, 0.3);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();//
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);//
	meshList[GEO_ORBITRON_MOON1]->Render();

	modelStack.PopMatrix();

	modelStack.PushMatrix();

	modelStack.Rotate(-10, 1, 0, 0);
	modelStack.Rotate(OM2.orbit, 0, 1, 0);
	modelStack.Translate(9, 0, 0);
	modelStack.Rotate(OM2.rotateA, 0, 1, 0);
	modelStack.Scale(0.2, 0.2, 0.2);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();//
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);//
	meshList[GEO_ORBITRON_MOON1]->Render();

	modelStack.PopMatrix();

	modelStack.PopMatrix();//555555555555555555555555555555555555 Orbitron

	modelStack.PushMatrix();//GEO MR JOE

	modelStack.Rotate(-5, 1, 0, 0);
	modelStack.Rotate(GEOJOE.orbit, 0, 1, 0);
	modelStack.Translate(300, 0, 0);
	modelStack.Rotate(GEOJOE.rotateA, 0, 1, 0);
	modelStack.Scale(8, 8, 8);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();//
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);//
	meshList[GEO_MRJOE]->Render();

	modelStack.PopMatrix();//GEO MR JOE
}

void Scene4::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
