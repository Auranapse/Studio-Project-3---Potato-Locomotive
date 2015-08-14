#include "Scene3.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

Scene3::Scene3()
{
}

Scene3::~Scene3()
{
}

void Scene3::Init()
{
	// Init VBO here
	
	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

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
	rotateAngle = 0;

	//Initialize camera settings
	camera.Init(Vector3(40, 30, 30), Vector3(0, 0, 0), Vector3(0, 1, 0));

	//remove all glGenBuffers, glBindBuffer, glBufferData code
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	//meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 0, 0), 1, 1);
	//meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 0, 0), 1,1,1);
	//meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(1, 0, 0), 64, 1);
	//meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 0), 64, 1, 2);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1);
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

void Scene3::Update(double dt)
{
	if(Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	//rotateAngle += (float)(10 * dt);

	camera.Update(dt);
}

void Scene3::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Temp variables
	Mtx44 translate, rotate, scale;
	Mtx44 MVP;

	//These will be replaced by matrix stack soon
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;

	//Set all matrices to identity
	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();

	//Set view matrix using camera settings
	view.SetToLookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
					);

	//Set projection matrix to perspective mode
	projection.SetToPerspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f); //FOV, Aspect Ratio, Near plane, Far plane
	
	scale.SetToScale(1, 1, 1);
	rotate.SetToIdentity();
	translate.SetToIdentity();
	model = translate * rotate * scale; //scale, followed by rotate, then lastly translate
	MVP = projection * view * model; // Remember, matrix multiplication is the other way around
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP

	meshList[GEO_AXES]->Render();



	scale.SetToScale(10, 10, 10);
	rotate.SetToRotation(rotateAngle, 0, 0, 1);
	translate.SetToIdentity();
	model = translate * rotate * scale; //scale, followed by rotate, then lastly translate
	MVP = projection * view * model; // Remember, matrix multiplication is the other way around
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP

	//meshList[GEO_QUAD]->Render();
	//meshList[GEO_CUBE]->Render();
	//meshList[GEO_CIRCLE]->Render();
	//meshList[GEO_RING]->Render();
	meshList[GEO_SPHERE]->Render();

}

void Scene3::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
