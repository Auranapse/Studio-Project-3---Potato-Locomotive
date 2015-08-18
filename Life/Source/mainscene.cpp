/****************************************************************************/
/*!
\file mainscene.cpp
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Aperture Science Laboratories Underground
!*/
/****************************************************************************/
#include "mainscene.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"

#include <fstream>
#include <sstream>

#include <irrKlang.h>
#pragma comment (lib, "irrKlang.lib")

using namespace irrklang;

/******************************************************************************/
/*!
mainscene Constructor
*/
/******************************************************************************/
mainscene::mainscene() : TESTMODE(false), NUM_LIGHT_PARAM(11)
{

}

/******************************************************************************/
/*!
mainscene Destructor
*/
/******************************************************************************/
mainscene::~mainscene()
{

}

/******************************************************************************/
/*!
\brief
Loads save file from file
*/
/******************************************************************************/
void mainscene::assignSave(void)
{
	f_fov = static_cast<float>(std::stoi(*SF_1.Data[0]));
	f_mouseSensitivity = static_cast<float>(std::stoi(*SF_1.Data[14])) / 100.f;
	char CN = std::stoi(*SF_1.Data[1]);
	us_control[E_CTRL_MOVE_FRONT] = CN;
	CN = std::stoi(*SF_1.Data[2]);
	us_control[E_CTRL_MOVE_BACK] = CN;
	CN = std::stoi(*SF_1.Data[3]);
	us_control[E_CTRL_MOVE_LEFT] = CN;
	CN = std::stoi(*SF_1.Data[4]);
	us_control[E_CTRL_MOVE_RIGHT] = CN;
	CN = std::stoi(*SF_1.Data[5]);
	us_control[E_CTRL_MOVE_SPRINT] = CN;
	CN = std::stoi(*SF_1.Data[6]);
	us_control[E_CTRL_MOVE_WALK] = CN;
	CN = std::stoi(*SF_1.Data[7]);
	us_control[E_CTRL_MOVE_JUMP] = CN;
	CN = std::stoi(*SF_1.Data[8]);
	us_control[E_CTRL_INTERACT] = CN;
	CN = std::stoi(*SF_1.Data[9]);
	us_control[E_CTRL_KICK] = CN;
	CN = std::stoi(*SF_1.Data[10]);
	us_control[E_CTRL_ATTACK] = CN;
	CN = std::stoi(*SF_1.Data[11]);
	us_control[E_CTRL_AIM] = CN;
	CN = std::stoi(*SF_1.Data[12]);
	us_control[E_CTRL_NEXT_ITEM] = CN;
	CN = std::stoi(*SF_1.Data[13]);
	us_control[E_CTRL_RELOAD] = CN;
	SF_1.saveData();
}

/******************************************************************************/
/*!
\brief
Initialize default variables, create meshes, lighting
*/
/******************************************************************************/
void mainscene::Init()
{
	engine = createIrrKlangDevice(ESOD_AUTO_DETECT, ESEO_MULTI_THREADED | ESEO_LOAD_PLUGINS | ESEO_USE_3D_BUFFERS);
	weaponsEnabled = true;
	//Control initialization--------------
	for (unsigned i = 0; i < E_CTRL_TOTAL; ++i)
	{
		us_control[i] = 0;
	}

	f_mouseSensitivity = 1;

	SF_1.init("GameData//playerdata.wtf");
	assignSave();

	m_renderPass = RENDER_PASS_PRE;

	// Init VBO here
	// Set background color to whatever
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Load vertex and fragment shaders
	m_gPassShaderID = LoadShaders("GameData//Shader//GPass.vertexshader", "GameData//Shader//GPass.fragmentshader");
	m_programID = LoadShaders("GameData//Shader//Shadow.vertexshader", "GameData//Shader//Shadow.fragmentshader");
	m_gBufferShaderID = LoadShaders("GameData//Shader//GBuffer.vertexshader", "GameData//Shader//GBuffer.fragmentshader");
	m_lightShaderID = LoadShaders("GameData//Shader//LightPass.vertexshader", "GameData//Shader//LightPass.fragmentshader");

	m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");
	m_parameters[U_MVP_GBUFFER] = glGetUniformLocation(m_gBufferShaderID, "MVP");
	m_parameters[U_MODELVIEW_GBUFFER] = glGetUniformLocation(m_gBufferShaderID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE_GBUFFER] = glGetUniformLocation(m_gBufferShaderID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT_GBUFFER] = glGetUniformLocation(m_gBufferShaderID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE_GBUFFER] = glGetUniformLocation(m_gBufferShaderID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR_GBUFFER] = glGetUniformLocation(m_gBufferShaderID, "material.kSpecular");
	m_parameters[U_MATERIAL_EMISSIVE_GBUFFER] = glGetUniformLocation(m_gBufferShaderID, "material.kEmissive");
	m_parameters[U_MATERIAL_SHININESS_GBUFFER] = glGetUniformLocation(m_gBufferShaderID, "material.kShininess");
	m_parameters[U_LIGHTENABLED_GBUFFER] = glGetUniformLocation(m_gBufferShaderID, "lightEnabled");
	m_parameters[U_COLOR_TEXTURE_GBUFFER] = glGetUniformLocation(m_gBufferShaderID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE1_GBUFFER] = glGetUniformLocation(m_gBufferShaderID, "colorTexture[1]");
	m_parameters[U_COLOR_TEXTURE_ENABLED_GBUFFER] = glGetUniformLocation(m_gBufferShaderID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED1_GBUFFER] = glGetUniformLocation(m_gBufferShaderID, "colorTextureEnabled[1]");
	m_parameters[U_LIGHT_DEPTH_MVP_GBUFFER] = glGetUniformLocation(m_gBufferShaderID, "lightDepthMVP");
	m_parameters[U_SHADOW_MAP_GBUFFER] = glGetUniformLocation(m_gBufferShaderID, "shadowMap");
	m_parameters[U_MVP_LIGHTPASS] = glGetUniformLocation(m_lightShaderID, "MVP");
	m_parameters[U_SCREEN_SIZE_LIGHTPASS] = glGetUniformLocation(m_lightShaderID, "screenSize");
	m_parameters[U_POSITION_MAP_LIGHTPASS] = glGetUniformLocation(m_lightShaderID, "positionMap");
	m_parameters[U_NORMAL_MAP_LIGHTPASS] = glGetUniformLocation(m_lightShaderID, "normalMap");
	m_parameters[U_AMBIENT_MAP_LIGHTPASS] = glGetUniformLocation(m_lightShaderID, "ambientMap");
	m_parameters[U_DIFFUSE_MAP_LIGHTPASS] = glGetUniformLocation(m_lightShaderID, "diffuseMap");
	m_parameters[U_SPECULAR_MAP_LIGHTPASS] = glGetUniformLocation(m_lightShaderID, "specularMap");
	m_parameters[U_EMISSIVE_MAP_LIGHTPASS] = glGetUniformLocation(m_lightShaderID, "emissiveMap");
	m_parameters[U_LIGHT_TYPE_LIGHTPASS] = glGetUniformLocation(m_lightShaderID, "light.type");
	m_parameters[U_LIGHT_POSITION_LIGHTPASS] = glGetUniformLocation(m_lightShaderID, "light.position_cameraspace");
	m_parameters[U_LIGHT_COLOR_LIGHTPASS] = glGetUniformLocation(m_lightShaderID, "light.color");
	m_parameters[U_LIGHT_POWER_LIGHTPASS] = glGetUniformLocation(m_lightShaderID, "light.power");
	m_parameters[U_LIGHT_RADIUS_LIGHTPASS] = glGetUniformLocation(m_lightShaderID, "light.radius");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");
	m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID, "shadowMap");
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");

	glUseProgram(m_programID);
	// Get a handle for our "MVP" uniform


	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
	m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
	m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
	m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
	m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
	m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
	m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
	m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[3].spotDirection");
	m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[3].cosCutoff");
	m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID, "lights[3].cosInner");
	m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID, "lights[3].exponent");

	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	m_gBuffer.Init(Application::GetWindowWidth(), Application::GetWindowHeight());
	m_lightDepthFBO.Init(4096, 4096);
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	//Main Lighting (VIewing room sign)
	/*lights[0].type = LIGHT_POINT;
	lights[0].position.Set(100.f, 200.f, -550.f);
	lights[0].color.Set(1.f, 1.f, 1.f);
	lights[0].power = 1.5f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.001f;
	lights[0].kQ = 0.000001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(50.f));
	lights[0].cosInner = cos(Math::DegreeToRadian(20.f));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);//*/

	lights[0].type = LIGHT_POINT;
	lights[0].position.Set(10.f, 100.f, 0.f);
	lights[0].color.Set(1.f, 1.f, 1.f);
	lights[0].kC = 1.f;
	lights[0].kL = 0.00001f;
	lights[0].kQ = 0.0000001f;
	lights[0].power = 1.5f;


	//Viewing room 2 light
	lights[1].type = LIGHT_POINT;
	lights[1].position.Set(-2450.f, 60.f, 0.f);
	lights[1].color.Set(1.f, 1.f, 1.f);
	lights[1].power = 1.5f;
	lights[1].kC = 1.f;
	lights[1].kL = 0.001f;
	lights[1].kQ = 0.000001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(50.f));
	lights[1].cosInner = cos(Math::DegreeToRadian(20.f));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);
	//Turret eyes light
	lights[2].type = LIGHT_POINT;
	lights[2].position.Set(100.f, 200.f, -550.f);
	lights[2].color.Set(1.f, 1.f, 1.f);
	lights[2].power = 1.5f;
	lights[2].kC = 1.f;
	lights[2].kL = 0.001f;
	lights[2].kQ = 0.000001f;
	lights[2].cosCutoff = cos(Math::DegreeToRadian(50.f));
	lights[2].cosInner = cos(Math::DegreeToRadian(20.f));
	lights[2].exponent = 3.f;
	lights[2].spotDirection.Set(0.f, 1.f, 0.f);
	//Flurenscent Light 1 modelStack.Translate(200, -480, 1680);
	lights[3].type = LIGHT_POINT;
	lights[3].position.Set(295.f, 1258.f, 1680.f);
	lights[3].color.Set(1.0f, 0.93f, 0.81f);
	lights[3].power = 1;
	lights[3].kC = 1.f;
	lights[3].kL = 0.001f;
	lights[3].kQ = 0.00001f;
	lights[3].cosCutoff = cos(Math::DegreeToRadian(0.3f));
	lights[3].cosInner = cos(Math::DegreeToRadian(0.2f));
	lights[3].exponent = 3.f;
	lights[3].spotDirection.Set(-1.f, 0.f, 0.f);
	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	glUniform1i(m_parameters[U_LIGHT2_TYPE], lights[2].type);
	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &lights[2].color.r);
	glUniform1f(m_parameters[U_LIGHT2_POWER], lights[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], lights[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], lights[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], lights[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], lights[2].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], lights[2].cosInner);
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], lights[2].exponent);

	glUniform1i(m_parameters[U_LIGHT3_TYPE], lights[3].type);
	glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &lights[3].color.r);
	glUniform1f(m_parameters[U_LIGHT3_POWER], lights[3].power);
	glUniform1f(m_parameters[U_LIGHT3_KC], lights[3].kC);
	glUniform1f(m_parameters[U_LIGHT3_KL], lights[3].kL);
	glUniform1f(m_parameters[U_LIGHT3_KQ], lights[3].kQ);
	glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], lights[3].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT3_COSINNER], lights[3].cosInner);
	glUniform1f(m_parameters[U_LIGHT3_EXPONENT], lights[3].exponent);

	m_parameters[U_TRANSPARENCY] = glGetUniformLocation(m_programID, "alpha");
	m_parameters[U_GLOW] = glGetUniformLocation(m_programID, "glow");
	m_parameters[U_GLOW_COLOR] = glGetUniformLocation(m_programID, "glowColor");
	
	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.start");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
	m_parameters[U_FOG_ENABLED] = glGetUniformLocation(m_programID, "fogParam.enabled");

	Color FogCol;
	FogCol.Set(0.5f, 0.5f, 0.5f);
	float f_start = 100;
	float f_end = 1000;
	float f_density = 0.005f;
	int f_type = 1;
	enableFOG = true;
	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &FogCol.r);
	glUniform1i(m_parameters[U_FOG_START], f_start);
	glUniform1i(m_parameters[U_FOG_END], f_end);
	glUniform1i(m_parameters[U_FOG_DENSITY], f_density);
	glUniform1i(m_parameters[U_FOG_TYPE], f_type);
	glUniform1i(m_parameters[U_FOG_ENABLED], enableFOG);

	//Initialize camera settings
	camera.Init(Vector3(-150.f, 150.f, 0.f), Vector3(-149.f, 150.f, 0.f), Vector3(0.f, 1.f, 0.f));

	//Set projection matrix to perspective mode
	editFOV(f_fov);
	f_currentfov = f_fov;

	//Generate meshes------------------------------------------------------------------------
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("AXES", 10000.f, 10000.f, 10000.f);
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("Crosshair", Color(0.f, 1.f, 1.f), 0.1f, 0.5f, 1.f);
	meshList[GEO_FLOOR_TILE] = MeshBuilder::GenerateQuad("Room floor", Color(1.f, 1.f, 1.f), 10.f, 10.f, 400.f);
	meshList[GEO_FLOOR_TILE]->textureID[0] = LoadTGA("GameData//Image//floortexture.tga", false);

	meshList[GEO_WORLD_CUBE] = MeshBuilder::GenerateCubeT2("World Cube", Color(0.7f, 0.7f, 0.7f), 1, 1, 1);
	meshList[GEO_WORLD_CUBE]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_WORLD_CUBE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WORLD_CUBE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WORLD_CUBE]->material.kShininess = 1.0f;

	meshList[GEO_LIGHT] = MeshBuilder::GenerateSphere("THELIGHT", Color(1.0, 1.0, 1.0), 9, 18, 1);

	//Load OBJ Models
	meshList[GEO_OBJCAKE] = MeshBuilder::GenerateOBJ("CAKE", "GameData//OBJ//CAKE.obj");
	meshList[GEO_OBJCAKE]->textureID[0] = LoadTGA("GameData//Image//OBJ//Cake_UV.tga", true);

	meshList[GEO_SECURITYCAMERA] = MeshBuilder::GenerateOBJ("CAKE", "GameData//OBJ//other//SecurityCamera.obj");
	meshList[GEO_SECURITYCAMERA]->textureID[0] = LoadTGA("GameData//Image//OBJ//SecurityCamera_UV.tga", true);

	//WEAPONS
	//if(weaponsEnabled)
	{
		meshList[GEO_BULLET] = MeshBuilder::GenerateSphere("Gun bullet", Color(1.f, 0.9f, 0.5f), 10, 10, 0.53f);

		meshList[GEO_M9] = MeshBuilder::GenerateOBJ("M9", "GameData//OBJ//weapons//M9.obj");
		meshList[GEO_M9]->textureID[0] = LoadTGA("GameData//Image//weapons//M9.tga", true);

		meshList[GEO_MP5K] = MeshBuilder::GenerateOBJ("MP5K", "GameData//OBJ//weapons//MP5K.obj");
		meshList[GEO_MP5K]->textureID[0] = LoadTGA("GameData//Image//weapons//MP5K.tga", true);
		
		meshList[GEO_SPAS12] = MeshBuilder::GenerateOBJ("SPAS-12", "GameData//OBJ//weapons//SPAS12.obj");
		meshList[GEO_SPAS12]->textureID[0] = LoadTGA("GameData//Image//weapons//SPAS12.tga", true);
	}

	//----------------------SKYBOX
	meshList[E_GEO_LEFT] = MeshBuilder::GenerateSkybox("left", Color(0.f, 0.f, 0.f), 1.f);
	meshList[E_GEO_LEFT]->textureID[0] = LoadTGA("GameData//Image//skybox//plain_sky_left.tga");
	meshList[E_GEO_RIGHT] = MeshBuilder::GenerateSkybox("right", Color(0.f, 0.f, 0.f), 1.f);
	meshList[E_GEO_RIGHT]->textureID[0] = LoadTGA("GameData//Image//skybox//plain_sky_right.tga");
	meshList[E_GEO_TOP] = MeshBuilder::GenerateSkybox("top", Color(0.f, 0.f, 0.f), 1.f);
	meshList[E_GEO_TOP]->textureID[0] = LoadTGA("GameData//Image//skybox//plain_sky_top.tga");
	meshList[E_GEO_FRONT] = MeshBuilder::GenerateSkybox("front", Color(0.f, 0.f, 0.f), 1.f);
	meshList[E_GEO_FRONT]->textureID[0] = LoadTGA("GameData//Image//skybox//plain_sky_front.tga");
	meshList[E_GEO_BACK] = MeshBuilder::GenerateSkybox("back", Color(0.f, 0.f, 0.f), 1.f);
	meshList[E_GEO_BACK]->textureID[0] = LoadTGA("GameData//Image//skybox//plain_sky_back.tga");

	meshList[E_GEO_SKYPLANE] = MeshBuilder::GenerateSkyPlane("SKYPLANE", Color(0.5, 0.5, 0.5), 128, 200.f, 2000.f, 1.f, 1.f);
	meshList[E_GEO_SKYPLANE]->textureID[0] = LoadTGA("GameData//Image//skybox//CLOUDS.tga");

	//---------------------------------------------------------------------------------------
	//Text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID[0] = LoadTGA("GameData//Image//font//inputm.tga", false);

	//--------------------------------------------------------------------------------------

	meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("LIGHT_DEPTH_TEXTURE", Color(1, 1, 1), 1.f, 1.f, 1.f);
	meshList[GEO_LIGHT_DEPTH_QUAD]->textureID[0] = m_lightDepthFBO.GetTexture();
	meshList[GEO_POSITION_QUAD] = MeshBuilder::GenerateQuad("position map", Color(1, 1, 1), 1.f, 1.f, 1.f);
	meshList[GEO_POSITION_QUAD]->textureID[0] = m_gBuffer.GetTexture(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	meshList[GEO_RENDERING_SPHERE] = MeshBuilder::GenerateSphere("rendering sphere", Color(1, 1, 1), 20, 20, 1); //radius must be 1
	meshList[GEO_RENDERING_QUAD] = MeshBuilder::GenerateQuad("rendering quad", Color(1, 1, 1), 1.f, 1.f, 1.f); //side must be length 2

	meshList[GEO_NORMAL_QUAD] = MeshBuilder::GenerateQuad("normal map", Color(1, 1, 1), 1.f, 1.f, 1.f);
	meshList[GEO_NORMAL_QUAD]->textureID[0] = m_gBuffer.GetTexture(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);

	meshList[GEO_AMBIENT_QUAD] = MeshBuilder::GenerateQuad("Ambient map", Color(1, 1, 1), 1.f, 1.f, 1.f);
	meshList[GEO_AMBIENT_QUAD]->textureID[0] = m_gBuffer.GetTexture(GBuffer::GBUFFER_TEXTURE_TYPE_AMBIENT);

	meshList[GEO_DIFFUSE_QUAD] = MeshBuilder::GenerateQuad("Diffuse map", Color(1, 1, 1), 1.f, 1.f, 1.f);
	meshList[GEO_DIFFUSE_QUAD]->textureID[0] = m_gBuffer.GetTexture(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);

	meshList[GEO_SPECULAR_QUAD] = MeshBuilder::GenerateQuad("Specular map", Color(1, 1, 1), 1.f, 1.f, 1.f);
	meshList[GEO_SPECULAR_QUAD]->textureID[0] = m_gBuffer.GetTexture(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR);

	meshList[GEO_EMISSIVE_QUAD] = MeshBuilder::GenerateQuad("Specular map", Color(1, 1, 1), 1.f, 1.f, 1.f);
	meshList[GEO_EMISSIVE_QUAD]->textureID[0] = m_gBuffer.GetTexture(GBuffer::GBUFFER_TEXTURE_TYPE_EMISSIVE);

	//Lighting-------------------------------------------------------------------------------

	meshList[GEO_FLOOR_TILE]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_FLOOR_TILE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FLOOR_TILE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_FLOOR_TILE]->material.kShininess = 1.0f;

	meshList[GEO_OBJCAKE]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_OBJCAKE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_OBJCAKE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_OBJCAKE]->material.kShininess = 1.0f;



	//if(weaponsEnabled)
	{
		meshList[GEO_M9]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_M9]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
		meshList[GEO_M9]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_M9]->material.kShininess = 10.0f;

		meshList[GEO_MP5K]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_MP5K]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
		meshList[GEO_MP5K]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_MP5K]->material.kShininess = 10.0f;
		
		meshList[GEO_SPAS12]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_SPAS12]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
		meshList[GEO_SPAS12]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_SPAS12]->material.kShininess = 12.0f;
	}


	//Starting position of translations and initialize physics


	renderAxis = false;

	mouseEnabled = true;
	if (mouseEnabled == true)
	{
		Application::SetMouseinput(Application::GetWindowWidth()*0.5, Application::GetWindowHeight()*0.5);
		Application::SetCursor(false);
	}
	FPScounter = 0.f;

	DisplayInfo = true;

	for (unsigned short i = 0; i < 100; ++i)
	{
		BulletInfo* BI = new BulletInfo();
		BI->setStatus(false);
		BIv_BulletList.push_back(BI);
	}

	

	P_Player.Init(Vector3(0, 100.f, 0), Vector3(0, 10, -1), "GameData//Image//player//PlayerSkin.tga");
	P_Player.Scale.Set(10, 10, 10);


	ai.Init(Vector3(0, 0, 0), Vector3(0, 0, -1), "GameData//Image//player//PlayerSkin.tga");
	ai.Scale.Set(10, 10, 10);

	sc.pos.Set(0, 30, 30);
	sc.scale.Set(5, 5, 5);

	f_step = 0.f;

	loadLevel(1);
	generateRoom1();
	FPC.Init(P_Player.getPosition() + P_Player.CamOffset, P_Player.getPosition() + P_Player.CamOffset + Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f), f_mouseSensitivity);

	gravity_force.Set(0.f, -9.82f * 20, 0.f);

	//if(weaponsEnabled)
	{
		initWeapons();
	}

	inputDelay = 0.f;
	timer = 0.f;

	ISoundSource* backgroundAmbience;
	backgroundAmbience = engine->addSoundSourceFromFile("GameData//sounds//ambience//background.wav");
	backgroundAmbience->setDefaultVolume(0.8f);
	engine->play2D(backgroundAmbience, true);

	soundList[ST_PANEL] = engine->addSoundSourceFromFile("GameData//sounds//other//panel.wav", ESM_AUTO_DETECT, true);
	soundList[ST_PANEL]->setDefaultVolume(4.5f);
	soundList[ST_PANEL]->setDefaultMinDistance(100.f);
	soundList[ST_STEP] = engine->addSoundSourceFromFile("GameData//sounds//other//step1.wav", ESM_AUTO_DETECT, true);
	soundList[ST_STEP_2] = engine->addSoundSourceFromFile("GameData//sounds//other//step2.wav", ESM_AUTO_DETECT, true);
	soundList[ST_KILL] = engine->addSoundSourceFromFile("GameData//sounds//other//kill.wav", ESM_AUTO_DETECT, true);
	soundList[ST_BUZZER] = engine->addSoundSourceFromFile("GameData//sounds//other//buzzer.wav", ESM_AUTO_DETECT, true);
	soundList[ST_ALERT] = engine->addSoundSourceFromFile("GameData//sounds//other//alert.wav", ESM_AUTO_DETECT, true);

	soundList[ST_WEAPON_M9_SHOOT] = engine->addSoundSourceFromFile("GameData//sounds//weapons//M9//FIRE.wav", ESM_AUTO_DETECT, true);
	soundList[ST_WEAPON_CLICK] = engine->addSoundSourceFromFile("GameData//sounds//weapons//empty.wav", ESM_AUTO_DETECT, true);

	soundList[ST_CAMERA_SPOTTED] = engine->addSoundSourceFromFile("GameData//sounds//other//EnemySpotted.mp3", ESM_AUTO_DETECT, true);
	soundList[ST_WEAPON_CLICK] = engine->addSoundSourceFromFile("GameData//sounds//other//Alarm.mp3", ESM_AUTO_DETECT, true);

}

void mainscene::InitShaders()
{
	Application::SetCursor(true);
	e_nextScene = Application::E_SCENE_MENU;
}

bool mainscene::loadLevel(int level)
{
	float worldsize = 40.f;
	std::cout << "\nLoading map...\n";
	std::string MAPLOC = "GameData//Maps//";
	MAPLOC += std::to_string(static_cast<unsigned long long>(level));
	MAPLOC += ".csv";
	if (!GAME_MAP.loadMap(MAPLOC))
	{
		std::cout << "!!!ERROR!!! Unable to load map\n";
		return false;
	}

	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		if (go != NULL)
		{
			delete go;
		}
		m_goList.pop_back();
	}
	P_Player.Velocity.SetZero();

	std::cout << "Map Size: ";
	std::cout << GAME_MAP.map_width << ", " << GAME_MAP.map_height << "\n";

	for (unsigned y = GAME_MAP.map_height - 1; y > 0; --y)
	{
		for (unsigned x = 0; x < GAME_MAP.map_width; ++x)
		{
			if (GAME_MAP.map_data[y][x] == "SPAWN")//Generate spawnpoint
			{
				P_Player.setPosition(Vector3(x*worldsize*2.f, worldsize, (GAME_MAP.map_height - y)*worldsize*2.f));
			}
			else if (GAME_MAP.map_data[y][x] != "-")//Generate the rest of the world
			{
				if (GAME_MAP.map_data[y][x] == "1")
				{
					WorldObject *WO;
					WO = new WorldObject();
					WO->active = true;
					WO->colEnable = true;
					WO->scale.Set(worldsize, worldsize, worldsize);
					WO->pos.Set(x*worldsize*2.f, worldsize, (GAME_MAP.map_height - y)*worldsize*2.f);
					WO->ColBox.Set(worldsize, worldsize, worldsize);
					WO->mesh = meshList[GEO_WORLD_CUBE];
					m_goList.push_back(WO);
				}
			}
		}
	}

	std::cout << "Map Successfully loaded\n";
	return true;
}

Particle* mainscene::FetchParticle(void)
{
	for (unsigned i = 0; i < m_ParList.size(); ++i)
	{
		if (!m_ParList[i]->active)
		{
			m_ParList[i]->active = true;
			m_ParList[i]->ParticleType = Particle::PAR_DEFAULT;
			return m_ParList[i];
			break;
		}
	}

	for (unsigned i = 0; i < 10; ++i)
	{
		Particle *Par;
		Par = new Particle;
		Par->active = false;
		m_ParList.push_back(Par);
	}
	return FetchParticle();
}

/******************************************************************************/
/*!
\brief
Intialize weapon stats, sounds, meshes
*/
/******************************************************************************/
void mainscene::initWeapons(void)
{
	firerate = 0.f;
	
	WeaponsObject *WPO;
	WPO = new WeaponsObject();
	WPO->active = true;
	WPO->mesh = meshList[GEO_M9];
	WPO->attackRate = 0.5f;
	WPO->scale.Set(0.03f, 0.03f, 0.03f);
	WPO->shootvelocity = 120.f;
	WPO->pos.Set(0, 10, 0);
	WPO->pos1.Set(-5, -4, 9);
	WPO->pos2.Set(0, -2.1f, 8);
	WPO->CurrentClip = 15;
	WPO->recoilEffect = 50.f;
	WPO->isGun = true;
	WPO->isWeapon = true;
	WPO->enablePhysics = true;
	WPO->colEnable = true;
	WPO->ColBox.Set(3, 3, 3);
	WPO->AttackSound = ST_WEAPON_M9_SHOOT;
	B = WPO;
	m_goList.push_back(WPO);

	f_curRecoil = 0.f;
}

/******************************************************************************/
/*!
\brief
Generate collision boxes and positions of objects in room 1
*/
/******************************************************************************/
void mainscene::generateRoom1(void)
{
	WorldObject *wo;

	wo = new WorldObject();
	wo->pos.Set(0, 0, 0);
	wo->rotation.x = -90;
	wo->scale.Set(400, 400, 400);
	wo->ColBox.Set(4000, 5, 4000);
	wo->active = true;
	wo->enablePhysics = false;
	wo->colEnable = true;
	wo->mesh = meshList[GEO_FLOOR_TILE];
	m_goList.push_back(wo);
}

/******************************************************************************/
/*!
\brief
Function to edit fov
*/
/******************************************************************************/
void mainscene::editFOV(float &newFOV)
{
	Mtx44 proj;
	proj.SetToPerspective(newFOV, static_cast<double>(Application::GetWindowWidth()) / static_cast<double>(Application::GetWindowHeight()), 0.1f, 10000.0f);
	projectionStack.LoadMatrix(proj);
}

/******************************************************************************/
/*!
\brief
Handles player physics and movement
*/
/******************************************************************************/
void mainscene::UpdatePlayer(double &dt)
{
	float walkSoundDelay = 0.7f;
	bool inAir = false;

	//Y axis collision handling
	if (!collide(Vector3(P_Player.getPosition())))
	{
		if (collide(Vector3(P_Player.getPosition() + P_Player.ModelPos + P_Player.HeadPos)))
		{
			if (P_Player.Velocity.y > 0)
				P_Player.Velocity.y = 0;
		}

		P_Player.Velocity += gravity_force * static_cast<float>(dt);
		inAir = true;
	}
	else
	{
		if (collide(Vector3(P_Player.getPosition() + Vector3(0.f, 4.f, 0.f))))//This is to prevent floor clipping, or rather, to make it bounce back up if it's clipping
		{
			P_Player.Velocity.y = 100;
		}

		else if (collide(Vector3(P_Player.getPosition() + Vector3(0.f, 2.f, 0.f))))
		{
			P_Player.Velocity.y = 50;
		}

		else if (collide(Vector3(P_Player.getPosition() + Vector3(0.f, 1.f, 0.f))))
		{
			P_Player.Velocity.y = 10;
		}

		else
		{
			if (P_Player.Velocity.y < -100)
			{
				engine->play2D(soundList[ST_STEP_2]);
			}
			P_Player.Velocity.y = 0;
		}
	}

	//PLAYER MOVEMENT
	Vector3 LookDir = FPC.target - FPC.position;
	LookDir.y = 0.f;
	Vector3 RightDir = LookDir.Cross(Vector3(0, 1, 0));
	LookDir.Normalize();
	RightDir.Normalize();

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_SPRINT]))
	{
		LookDir *= 25;
		RightDir *= 25;
		walkSoundDelay /= 2;
	}
	else if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_WALK]))
	{
		LookDir *= 4.5f;
		RightDir *= 4.5f;
		walkSoundDelay *= 2;
	}
	else
	{
		LookDir *= 15;
		RightDir *= 15;
	}

	//Player movement
	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_FRONT]) && !Application::IsKeyPressed(us_control[E_CTRL_MOVE_BACK]))
	{
		P_Player.Velocity.x += LookDir.x;
		P_Player.Velocity.z += LookDir.z;

		if (walkSoundDelay + f_step < timer && !inAir)
		{
			engine->play2D(soundList[ST_STEP]);
			f_step = timer;
		}
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_BACK]) && !Application::IsKeyPressed(us_control[E_CTRL_MOVE_FRONT]))
	{
		P_Player.Velocity.x -= LookDir.x;
		P_Player.Velocity.z -= LookDir.z;

		if (walkSoundDelay + f_step < timer && !inAir)
		{
			engine->play2D(soundList[ST_STEP]);
			f_step = timer;
		}
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_LEFT]) && !Application::IsKeyPressed(us_control[E_CTRL_MOVE_RIGHT]))
	{
		P_Player.Velocity -= RightDir;

		if (walkSoundDelay + f_step < timer && !inAir)
		{
			engine->play2D(soundList[ST_STEP]);
			f_step = timer;
		}
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_RIGHT]) && !Application::IsKeyPressed(us_control[E_CTRL_MOVE_LEFT]))
	{
		P_Player.Velocity += RightDir;

		if (walkSoundDelay + f_step < timer && !inAir)
		{
			engine->play2D(soundList[ST_STEP]);
			f_step = timer;
		}
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_JUMP]))
	{
		if (inAir == false)
		{
			P_Player.Velocity.y += 120;
			engine->play2D(soundList[ST_STEP]);
		}
	}

	//smooth slowing down
	if (P_Player.Velocity.x != 0)
	{
		float SForceX = 0 - P_Player.Velocity.x;
		P_Player.Velocity.x += SForceX * 0.1f;
	}

	if (P_Player.Velocity.z != 0)
	{
		float SForceZ = 0 - P_Player.Velocity.z;
		P_Player.Velocity.z += SForceZ * 0.1f;
	}


	//Collision handling
	if (collide(Vector3(P_Player.getPosition() + Vector3(10.f, 10.f, 0.f))) || collide(Vector3(P_Player.getPosition() + Vector3(10.f, 50.f, 0.f))))
	{
		if (P_Player.Velocity.x > 0)
		{
			P_Player.Velocity.x = 0;
		}
	}

	if (collide(Vector3(P_Player.getPosition() + Vector3(0.f, 10.f, 10.f))) || collide(Vector3(P_Player.getPosition() + Vector3(0.f, 50.f, 10.f))))
	{
		if (P_Player.Velocity.z > 0)
		{
			P_Player.Velocity.z = 0;
		}
	}

	if (collide(Vector3(P_Player.getPosition() + Vector3(-10.f, 10.f, 0.f))) || collide(Vector3(P_Player.getPosition() + Vector3(-10.f, 50.f, 0.f))))
	{
		if (P_Player.Velocity.x < 0)
		{
			P_Player.Velocity.x = 0;
		}
	}

	if (collide(Vector3(P_Player.getPosition() + Vector3(0.f, 10.f, -10.f))) || collide(Vector3(P_Player.getPosition() + Vector3(0.f, 50.f, -10.f))))
	{
		if (P_Player.Velocity.z < 0)
		{
			P_Player.Velocity.z = 0;
		}
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_INTERACT]))
	{
		if (P_Player.holding == NULL)
		{
			P_Player.holding = B;
			P_Player.holding->pos = Vector3(0, -10, 0);
			P_Player.holding->isHeld = true;
			P_Player.holding->colEnable = false;
		}
	}

	FPC = FPC + (P_Player.Velocity * static_cast<float>(dt));
	P_Player.Lookat = FPC.target;
	P_Player.Update(dt);
}

/******************************************************************************/
/*!
\brief
Handles game object physics
*/
/******************************************************************************/
void mainscene::UpdateGO(double &dt)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if (go->enablePhysics && !go->isHeld)
			{
				go->colEnable = false;
				if (collide(Vector3(go->pos.x, go->pos.y - go->ColBox.y, go->pos.z)))
				{
					if (go->vel.y != 0)
					{
						go->vel.y = 0.f;
					}

					if (go->vel.x != 0)
					{
						float Friction = 0 - go->vel.x;
						go->vel.x += Friction * 0.1f;
					}
					
					if (go->vel.z != 0)
					{
						float Friction = 0 - go->vel.z;
						go->vel.z += Friction * 0.1f;
					}
				}
				else
				{
					go->vel += gravity_force * static_cast<float>(dt);
				}

				if (collide(Vector3(go->pos.x + go->ColBox.x, go->pos.y, go->pos.z)))
				{
					if (go->vel.x > 0)
					{
						go->vel.x = 0;
					}
				}

				if (collide(Vector3(go->pos.x - go->ColBox.x, go->pos.y, go->pos.z)))
				{
					if (go->vel.x < 0)
					{
						go->vel.x = 0;
					}
				}

				if (collide(Vector3(go->pos.x, go->pos.y, go->pos.z + go->ColBox.z)))
				{
					if (go->vel.z > 0)
					{
						go->vel.z = 0;
					}
				}

				if (collide(Vector3(go->pos.x, go->pos.y, go->pos.z - go->ColBox.z)))
				{
					if (go->vel.z < 0)
					{
						go->vel.z = 0;
					}
				}

				go->colEnable = true;
				go->pos += go->vel * static_cast<float>(dt);
			}
			else
			{
				go->Update(dt);
			}
		}	
	}
}

/******************************************************************************/
/*!
\brief
Generates particles at position
*/
/******************************************************************************/
void mainscene::generateParticle(Vector3 &Pos, Vector3 &scale, Vector3 &Velocity, int type, float lifetime)
{
	Particle *Par = FetchParticle();
	Par->ParticleType = type;
	Par->Pos = Pos;
	Par->Scale = scale;
	Par->Vel = Velocity;
	Par->lifetime = lifetime;
	Par->active = true;
}

/******************************************************************************/
/*!
\brief
Handles particle physics
*/
/******************************************************************************/
void mainscene::UpdateParticles(double &dt)
{
	for (std::vector<Particle *>::iterator it = m_ParList.begin(); it != m_ParList.end(); ++it)
	{
		Particle *Par = (Particle *)*it;
		if (Par->active)
		{
			if (Par->lifetime < 0)
			{
				Par->Vel.SetZero();
				Par->active = false;
			}

			Par->lifetime -= static_cast<float>(dt);
			Par->Vel += gravity_force * static_cast<float>(dt);
			Par->Pos += Par->Vel * static_cast<float>(dt);
		}
	}
}

/******************************************************************************/
/*!
\brief
Updates bullet
*/
/******************************************************************************/
void mainscene::UpdateBullets(double &dt)
{
	for (std::vector<BulletInfo *>::iterator it = BIv_BulletList.begin(); it != BIv_BulletList.end(); ++it)
	{
		BulletInfo *BI = (BulletInfo *)*it;
		if (BI->getStatus())
		{
			if (collide(BI->getPosition(), true))
			{
				BI->verticalvelocity = 0.f;
				BI->setStatus(false);
				for (unsigned i = 0; i < 5; ++i)
				{
					generateParticle(BI->getPosition(), Vector3(0.5, 0.5, 0.5), Vector3(Math::RandFloatMinMax(-70, 70), Math::RandFloatMinMax(-5, 70), Math::RandFloatMinMax(-70, 70)) + BI->getDirection()*-20, Particle::PAR_DEFAULT, 1.0f);
				}
			}
			else
			{
				BI->Update(dt);
			}
		}
	}
}

/******************************************************************************/
/*!
\brief
Fires bullet
*/
/******************************************************************************/
void mainscene::Shoot(Vector3 Pos, Vector3 Dir, float Speed, float Longevity, float dmg)
{
	for (std::vector<BulletInfo *>::iterator it = BIv_BulletList.begin(); it != BIv_BulletList.end(); ++it)
	{
		BulletInfo *BI = (BulletInfo *)*it;
		if (!BI->getStatus())
		{
			BI->damage = dmg;
			BI->setLife(Longevity);
			BI->setSpeed(Speed);
			BI->setPosition(Pos);
			BI->setDirection(Dir);
			BI->setStatus(true);
			break;
		}
	}
}

/******************************************************************************/
/*!
\brief
Handles weapons firing, reloading, animations and so on
*/
/******************************************************************************/
void mainscene::weaponsUpdate(double &dt)
{
	if (P_Player.holding != NULL)
	{
		if (Application::IsKeyPressed(us_control[E_CTRL_AIM]) || !P_Player.holding->isWeapon && Application::IsKeyPressed(us_control[E_CTRL_ATTACK]))
		{
			Mtx44 tempR;
			tempR.SetToRotation(CalAnglefromPosition(P_Player.Lookat, P_Player.getPosition(), true), 0, 1, 0);
			P_Player.holding->pos = P_Player.getPosition() + P_Player.CamOffset + tempR*P_Player.holding->pos;
			P_Player.holding->rotation.y = CalAnglefromPosition(P_Player.Lookat, P_Player.getPosition(), true);
			P_Player.holding->colEnable = true;
			P_Player.holding->enablePhysics = true;
			P_Player.holding->isHeld = false;
			P_Player.holding->vel = P_Player.getDirection().Normalized() * 400.f;
			P_Player.holding = NULL;
		}

		else if (P_Player.holding->isWeapon)
		{
			static bool isAttackPressed = false;
			WeaponsObject *WO = dynamic_cast<WeaponsObject*>(P_Player.holding);
			if (Application::IsKeyPressed(us_control[E_CTRL_ATTACK]) && !isAttackPressed)
			{
				isAttackPressed = true;
				if (P_Player.holding->isGun)
				{
					if (WO->CurrentClip > 0 && WO->attackRate + firerate < timer)
					{
						firerate = timer;
						Vector3 ShootVector = FPC.target - FPC.position;
						FPC.rotateCamVertical(static_cast<float>(dt) * WO->recoilEffect);
						Shoot(FPC.position, ShootVector.Normalize(), WO->shootvelocity, 6);
						WO->rotation.x -= WO->recoilEffect *0.25f;
						WO->pos.z -= WO->recoilEffect*0.05f;
						engine->play2D(soundList[WO->AttackSound]);
						f_curRecoil += WO->recoilEffect * 0.25f;
						--WO->CurrentClip;
					}
					else if (WO->CurrentClip <= 0)
					{
						engine->play2D(soundList[ST_WEAPON_CLICK]);
					}
				}
				else
				{
					if (WO->isAnimationComplete() && firerate + WO->attackRate < timer && WO->animState)
					{
						firerate = timer;
						WO->toggleAnimation();
						engine->play2D(soundList[WO->AttackSound]);
					}
				}
			}
			else if (!Application::IsKeyPressed(us_control[E_CTRL_ATTACK]) && isAttackPressed)
			{
				isAttackPressed = false;
			}

			
			if (P_Player.holding->isGun)
			{
				if (Application::IsKeyPressed(VK_MBUTTON))
				{
					WO->toggleAnimation();
				}

				if (f_curRecoil > 0)
				{
					f_curRecoil -= f_curRecoil * 0.1f;
				}
			}
			else
			{
				if (WO->isAnimationComplete() && !WO->animState)
				{
					WO->toggleAnimation();
				}
			}
		}
	}
}

/******************************************************************************/
/*!
\brief
Check collision
*/
/******************************************************************************/
bool mainscene::collide(Vector3 &Position, bool bullet)
{
	//Game object collisions
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active && go->colEnable && go->pos != Position)
		{
			if (intersect(go->pos + go->ColBox, go->pos - go->ColBox, Position))
			{
				return true;
			}
		}
	}

	//collision with terrainmap
	/*if(monalisa.ColEnable)
	{
		Vector3 tempPos;
		tempPos = Position - monalisa.position;
		tempPos.x /= monalisa.scale.x;
		tempPos.z /= monalisa.scale.z;

		if(tempPos.x < 0.5 && tempPos.x > -0.5 && tempPos.z < 0.5 && tempPos.z > -0.5)
		{
			if(Position.y < (ReadHeightMap(monalisa.uc_heightmap, tempPos.x, tempPos.z)*monalisa.scale.y)+monalisa.position.y && Position.y > monalisa.position.y - 3)
			{
				return true;
			}
		}
	}*/

	return false;
}

bool mainscene::collideGO(GameObject *go)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go2 = (GameObject *)*it;
		if (go2->active && go2->colEnable && go2 != go)
		{
			if (intersect(go2->pos + go2->ColBox, go2->pos - go2->ColBox, go->pos + go->ColBox))
			{
				return true;
			}
			
			if (intersect(go2->pos + go2->ColBox, go2->pos - go2->ColBox, go->pos - go->ColBox))
			{
				return true;
			}
		}
	}
}

/******************************************************************************/
/*!
\brief
update player sound position
*/
/******************************************************************************/
void mainscene::UpdateSound(double &dt)
{
	engine->setListenerPosition(vec3df(FPC.position.x, FPC.position.y, FPC.position.z), vec3df(-(FPC.target.x - FPC.position.x), FPC.target.y - FPC.position.y, -(FPC.target.z - FPC.position.z)).normalize(), vec3df(0, 0, 0), vec3df(FPC.up.x, FPC.up.y, FPC.up.z));
}

/******************************************************************************/
/*!
\brief
Animations, controls
*/
/******************************************************************************/
void mainscene::Update(double dt)
{
	d_dt = dt;
	FPScounter = static_cast<float>(1 / dt);

	if (Application::IsKeyPressed('X'))
	{
		dt *= 0.05;

		if (P_Player.Velocity != 0)
		{
			dt *= P_Player.Velocity.LengthSquared() *0.005f;

			if (dt > d_dt)
			{
				dt = d_dt;
			}
		}
	}


	timer += static_cast<float>(dt);

	if (Application::IsKeyPressed('1'))
	{
		if (!Application::IsKeyPressed(VK_SHIFT))
		{
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glEnable(GL_CULL_FACE);
		}
	}

	if (Application::IsKeyPressed('2'))
	{
		if (!Application::IsKeyPressed(VK_SHIFT))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if (Application::IsKeyPressed('3'))
	{
		if (!Application::IsKeyPressed(VK_SHIFT))
		{
			renderAxis = true;
		}
		else
		{
			renderAxis = false;
		}
	}

	if (Application::IsKeyPressed('4'))
	{
		if (!Application::IsKeyPressed(VK_SHIFT))
		{
			DisplayInfo = false;
		}
		else
		{
			DisplayInfo = true;
		}
	}

	UpdatePlayer(dt);
	UpdateGO(dt);
	UpdateParticles(dt);
	FPC.Update(dt);

	ai.Update(dt, P_Player.getPosition());
	sc.update(dt, P_Player.getPosition());

	if (Application::IsKeyPressed(VK_F1))
	{
		if (!Application::IsKeyPressed(VK_SHIFT))
		{
			mouseEnabled = true;
			Application::SetCursor(false);
		}
		else
		{
			mouseEnabled = false;
			Application::SetCursor(true);
		}
	}

	if (weaponsEnabled)
	{
		UpdateBullets(dt);
		weaponsUpdate(dt);
	}

	UpdateSound(dt);

	CamRotationYaw = CalAnglefromPosition(FPC.target, FPC.position, true);
	CamRotationPitch = CalAnglefromPosition(FPC.target, FPC.position, false);
}

/******************************************************************************/
/*!
\brief
Rendering of game objects
*/
/******************************************************************************/
void mainscene::RenderGO(GameObject *go)
{
	if (!go->isHeld)
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos);
		modelStack.Rotate(go->rotation.x, 1, 0, 0);
		modelStack.Rotate(go->rotation.y, 0, 1, 0);
		modelStack.Rotate(go->rotation.z, 0, 0, 1);
		modelStack.Scale(go->scale);
		if (go->mesh)
		{
			RenderMesh(go->mesh, true, true);
		}
		modelStack.PopMatrix();
	}
}

/******************************************************************************/
/*!
\brief
Rendering of character objects
*/
/******************************************************************************/
void mainscene::RenderCharacter(CharacterObject *CO)
{
	float YRotation = CalAnglefromPosition(CO->Lookat, CO->getPosition(), true);
	float Pitch = -CalAnglefromPosition(CO->Lookat, CO->getPosition() + CO->CamOffset, false);

	if (CO->holding != NULL)
	{
		modelStack.PushMatrix();
		modelStack.Translate(CO->getPosition());
		modelStack.PushMatrix();
		modelStack.Translate(CO->CamOffset);
		modelStack.Rotate(YRotation, 0, 1, 0);
		modelStack.Rotate(Pitch, 1, 0, 0);
		modelStack.Translate(CO->holding->pos);
		modelStack.Rotate(CO->holding->rotation.x, 1, 0, 0);
		modelStack.Rotate(CO->holding->rotation.y, 0, 1, 0);
		modelStack.Rotate(CO->holding->rotation.z, 0, 0, 1);
		modelStack.Scale(CO->holding->scale);
		RenderMesh(CO->holding->mesh, true);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}

	if (CO == &P_Player)
	{
		return;
	}

	modelStack.PushMatrix();
	modelStack.Translate(CO->getPosition());
	modelStack.Translate(CO->ModelPos);
	modelStack.Rotate(YRotation, 0, 1, 0);

	modelStack.PushMatrix();
	modelStack.Scale(CO->Scale);
	RenderMesh(CO->Chest, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(CO->HeadPos);
	modelStack.Scale(CO->Scale);
	RenderMesh(CO->Head, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-CO->ArmPos.x, CO->ArmPos.y, CO->ArmPos.z);
	modelStack.Scale(CO->Scale);
	RenderMesh(CO->Arm_left, true, true, 10, 10);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(CO->ArmPos);
	modelStack.Scale(CO->Scale);
	RenderMesh(CO->Arm_right, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(CO->LegPos);
	modelStack.Rotate(CO->getAnimation().LEFT_LEG, 1, 0, 0);
	modelStack.Scale(CO->Scale);
	RenderMesh(CO->Leg_left, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(CO->LegPos);
	modelStack.Rotate(CO->getAnimation().RIGHT_LEG, 1, 0, 0);
	modelStack.Scale(CO->Scale);
	RenderMesh(CO->Leg_right, true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Rendering of particles
*/
/******************************************************************************/
void mainscene::RenderParticles(void)
{
	for (std::vector<Particle *>::iterator it = m_ParList.begin(); it != m_ParList.end(); ++it)
	{
		Particle *Par = (Particle *)*it;
		if (Par->active)
		{
			switch (Par->ParticleType)
			{
			case Particle::PAR_DEFAULT:
			{
				modelStack.PushMatrix();
				modelStack.Translate(Par->Pos);
				modelStack.Scale(Par->Scale);
				RenderMesh(meshList[GEO_BULLET], false);
				modelStack.PopMatrix();
				break;
			}
			default:
				break;
			}
		}
	}
}

/******************************************************************************/
/*!
\brief
Rendering of bullets
*/
/******************************************************************************/
void mainscene::RenderBullet(void)
{
	for (std::vector<BulletInfo *>::iterator it = BIv_BulletList.begin(); it != BIv_BulletList.end(); ++it)
	{
		BulletInfo *BI = (BulletInfo *)*it;
		if (BI->getStatus())
		{
			modelStack.PushMatrix();
			modelStack.Translate(BI->getPosition());
			RenderMesh(meshList[GEO_BULLET], false);
			modelStack.PopMatrix();
		}
	}
}

/******************************************************************************/
/*!
\brief
Renders mesh in 3D
*/
/******************************************************************************/
void mainscene::RenderMeshin2D(Mesh *mesh, bool enableLight, float size, float x, float y, float rotation)
{
	glUniform1i(m_parameters[U_GLOW], 0);
	glUniform1i(m_parameters[U_TRANSPARENCY], 10);
	glUniform1i(m_parameters[U_FOG_ENABLED], 0);
	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::GetWindowWidth()*0.1, 0, Application::GetWindowHeight()*0.1, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	//modelStack.LoadIdentity();

	modelStack.Translate(x, y, 0);
	modelStack.Rotate(rotation, 0, 0, 1);
	if (size != 0)
	{
		modelStack.Scale(size, size, size);
	}

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	for (unsigned i = 0; i < Mesh::NUM_TEXTURES; ++i)
	{
		if (mesh->textureID[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureID[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], 0);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}
	}
	mesh->Render();

	glBindTexture(GL_TEXTURE_2D, 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Renders mesh
*/
/******************************************************************************/
void mainscene::RenderMesh(Mesh *mesh, bool enableLight, bool enableFog, float visibility, float glow, Color glowColor, Material *material)
{
	glUniform1i(m_parameters[U_GLOW], glow);
	glUniform3fv(m_parameters[U_GLOW_COLOR], 1, &glowColor.r);
	glUniform1i(m_parameters[U_TRANSPARENCY], visibility);

	if (enableFOG && enableFog)
	{
		glUniform1i(m_parameters[U_FOG_ENABLED], 1);
	}
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	switch (m_renderPass)
	{
	case RENDER_PASS_PRE:
	{
		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GPASS], 1, GL_FALSE, &lightDepthMVP.a[0]);
		mesh->Render();
		break;
	}
	case RENDER_PASS_MAIN:
	{
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

		if (enableLight)
		{
			glUniform1i(m_parameters[U_LIGHTENABLED], 1);

			modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
			glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
			//
			Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1, GL_FALSE, &lightDepthMVP.a[0]);
			//
			//load material
			glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
			glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
			glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
			glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
		}
		else
		{
			glUniform1i(m_parameters[U_LIGHTENABLED], 0);
		}

		for (unsigned i = 0; i < Mesh::NUM_TEXTURES; ++i)
		{
			if (mesh->textureID[i] > 0)
			{
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			}
			else
			{
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
			}

			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureID[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		mesh->Render(); //this line should only be called once
		break;
	}
	case RENDER_PASS_LIGHT:
	{
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP_LIGHTPASS], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render();
		break;
	}
	case RENDER_PASS_GBUFFER:
	{
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP_GBUFFER], 1, GL_FALSE, &MVP.a[0]);

		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_GBUFFER], 1, GL_FALSE, &modelView.a[0]);

		if (enableLight)
		{
			glUniform1i(m_parameters[U_LIGHTENABLED_GBUFFER], 1);

			modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
			glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE_GBUFFER], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

			Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GBUFFER], 1, GL_FALSE, &lightDepthMVP.a[0]);

			//load material
			if (material == NULL)
			{
				glUniform3fv(m_parameters[U_MATERIAL_AMBIENT_GBUFFER], 1, &mesh->material.kAmbient.r);
				glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE_GBUFFER], 1, &mesh->material.kDiffuse.r);
				glUniform3fv(m_parameters[U_MATERIAL_SPECULAR_GBUFFER], 1, &mesh->material.kSpecular.r);
				glUniform3fv(m_parameters[U_MATERIAL_EMISSIVE_GBUFFER], 1, &mesh->material.kEmissive.r);
				glUniform1f(m_parameters[U_MATERIAL_SHININESS_GBUFFER], mesh->material.kShininess);
			}
			else
			{
				glUniform3fv(m_parameters[U_MATERIAL_AMBIENT_GBUFFER], 1, &material->kAmbient.r);
				glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE_GBUFFER], 1, &material->kDiffuse.r);
				glUniform3fv(m_parameters[U_MATERIAL_SPECULAR_GBUFFER], 1, &material->kSpecular.r);
				glUniform3fv(m_parameters[U_MATERIAL_EMISSIVE_GBUFFER], 1, &material->kEmissive.r);
				glUniform1f(m_parameters[U_MATERIAL_SHININESS_GBUFFER], material->kShininess);
			}
		}
		else
		{
			glUniform1i(m_parameters[U_LIGHTENABLED_GBUFFER], 0);
		}
		for (int i = 0; i < 1; ++i)
		{
			if (mesh->textureID[i] > 0)
			{
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED_GBUFFER + i], 1);

				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh->textureID[i]);
				glUniform1i(m_parameters[U_COLOR_TEXTURE_GBUFFER + i], i);
			}
			else
			{
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED_GBUFFER + i], 0);
			}
		}
		mesh->Render();

		break;
	}
	}
}

/******************************************************************************/
/*!
\brief
Renders text
*/
/******************************************************************************/
void mainscene::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID[0] <= 0) //Proper error check
		return;

	glUniform1i(m_parameters[U_GLOW], 0);
	glUniform1i(m_parameters[U_TRANSPARENCY], 10);

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

/******************************************************************************/
/*!
\brief
Renders text on screen
*/
/******************************************************************************/
void mainscene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID[0] <= 0) //Proper error check
		return;

	glUniform1i(m_parameters[U_GLOW], 0);
	glUniform1i(m_parameters[U_TRANSPARENCY], 10);

	glDisable(GL_DEPTH_TEST);

	glUniform1i(m_parameters[U_FOG_ENABLED], 0);
	//Add these code just after glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::GetWindowWidth()*0.1, 0, Application::GetWindowHeight()*0.1, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

/******************************************************************************/
/*!
\brief
Code to render skybox
*/
/******************************************************************************/
void mainscene::RenderSkybox(void)
{
	modelStack.PushMatrix();
	modelStack.Translate(FPC.position.x, FPC.position.y + 550, FPC.position.z + 5000);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	modelStack.Scale(10060.f, 10060.f, 0.f);
	RenderMesh(meshList[E_GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(FPC.position.x, FPC.position.y + 550, FPC.position.z - 5000);
	modelStack.Scale(10060.f, 10060.f, 0.f);
	RenderMesh(meshList[E_GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(FPC.position.x - 5000, FPC.position.y + 550, FPC.position.z);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	modelStack.Scale(10060.f, 10060.f, 0.f);
	RenderMesh(meshList[E_GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(FPC.position.x + 5000, FPC.position.y + 550, FPC.position.z);
	modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
	modelStack.Scale(10060.f, 10060.f, 0.f);
	RenderMesh(meshList[E_GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(FPC.position.x, FPC.position.y + 5550, FPC.position.z);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
	modelStack.Scale(10060.f, 10060.f, 0.f);
	RenderMesh(meshList[E_GEO_TOP], false);
	modelStack.PopMatrix();
	/*
	modelStack.PushMatrix();
	modelStack.Translate(0, FPC.position.y + 2400, 0);
	modelStack.Rotate(timer*0.8f, 0, 1, 0);
	modelStack.Scale(2.f, .5f, 2.f);
	RenderMesh(meshList[E_GEO_SKYPLANE], false);
	modelStack.PopMatrix();//*/
}

/******************************************************************************/
/*!
\brief
Renders the entire world with shadow
*/
/******************************************************************************/
void mainscene::RenderWorldShadow(void)
{
	//Render gameobjects
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if (!go->dynamicRendering)
			{
				RenderGO(go);
			}
			else//Dynamic rendering
			{
				RenderGO(go);
			}
		}
	}

	RenderParticles();

	RenderCharacter(&ai);
	RenderCharacter(&P_Player);

	modelStack.PushMatrix();
	modelStack.Translate(sc.pos.x, sc.pos.y, sc.pos.z);
	modelStack.Scale(sc.scale.x, sc.scale.y, sc.scale.z);
	RenderMesh(meshList[GEO_SECURITYCAMERA], false);
	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Renders the entire world without shadow
*/
/******************************************************************************/
void mainscene::RenderWorldNoShadow(void)
{
	if (renderAxis == true)
	{
		modelStack.PushMatrix();
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_AXES], false);
		modelStack.PopMatrix();
	}

	RenderSkybox();

	if (weaponsEnabled)
	{
		RenderBullet();
	}

	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHT], false);
	modelStack.PopMatrix();

	//RenderObjectsAlpha();//MUST RENDER IN ENVIRONMENT LAST OR WE'LL ALL DIE
}

/******************************************************************************/
/*!
\brief
Renders the ingameUI
*/
/******************************************************************************/
void mainscene::RenderUI(void)
{
	if (weaponsEnabled)
	{
		RenderMeshin2D(meshList[GEO_CROSSHAIR], false, 1, Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f - 1 - f_curRecoil * 0.5f);
		RenderMeshin2D(meshList[GEO_CROSSHAIR], false, 1, Application::GetWindowWidth()*0.05f + 1 + f_curRecoil * 0.5f, Application::GetWindowHeight()*0.05f, 90);
		RenderMeshin2D(meshList[GEO_CROSSHAIR], false, 1, Application::GetWindowWidth()*0.05f - 1 - f_curRecoil * 0.5f, Application::GetWindowHeight()*0.05f, 90);
		RenderMeshin2D(meshList[GEO_CROSSHAIR], false, 1, Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f + 1 + f_curRecoil * 0.5f);
	}

	if (DisplayInfo == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(static_cast<long double>(FPScounter)), Color(0, 1, 1), 2, 1, 2);
	}
}

/******************************************************************************/
/*!
\brief
Main renderpass
*/
/******************************************************************************/
void mainscene::RenderPassMain(void)
{
	m_renderPass = RENDER_PASS_MAIN;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetWindowWidth(), Application::GetWindowHeight());

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);


	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

	RenderWorldShadow();
	RenderWorldNoShadow();
	/*
	RenderMeshin2D(meshList[GEO_POSITION_QUAD], false, 5, Application::GetWindowWidth()*0.095f, Application::GetWindowHeight()*0.09f);
	RenderMeshin2D(meshList[GEO_NORMAL_QUAD], false, 5, Application::GetWindowWidth()*0.095f, Application::GetWindowHeight()*0.075f);
	RenderMeshin2D(meshList[GEO_AMBIENT_QUAD], false, 5, Application::GetWindowWidth()*0.095f, Application::GetWindowHeight()*0.06f);
	RenderMeshin2D(meshList[GEO_DIFFUSE_QUAD], false, 5, Application::GetWindowWidth()*0.095f, Application::GetWindowHeight()*0.045f);
	RenderMeshin2D(meshList[GEO_SPECULAR_QUAD], false, 5, Application::GetWindowWidth()*0.095f, Application::GetWindowHeight()*0.03f);
	RenderMeshin2D(meshList[GEO_EMISSIVE_QUAD], false, 5, Application::GetWindowWidth()*0.095f, Application::GetWindowHeight()*0.015f);//*/
	RenderMeshin2D(meshList[GEO_LIGHT_DEPTH_QUAD], false, 5, Application::GetWindowWidth()*0.08f, Application::GetWindowHeight()*0.09f);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	RenderUI();

	glDisable(GL_BLEND);
}

/******************************************************************************/
/*!
\brief
GPass render for shadows
*/
/******************************************************************************/
void mainscene::RenderPassGPass(void)
{
	m_renderPass = RENDER_PASS_PRE;
	m_lightDepthFBO.BindForWriting();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_gPassShaderID);

	//if (lights[0].type == LIGHT_DIRECTIONAL)
	{
		//m_lightDepthProj.SetToOrtho(-100 - FPC.position.z*0.1, 100 - FPC.position.z*0.1, -100 - FPC.position.x*0.1, 100 - FPC.position.x*0.1, -200 + FPC.position.y*0.1, 400 + FPC.position.y*0.1);
		m_lightDepthProj.SetToOrtho(-200, 200, -200, 200, -200, 400);
	}
	//else
	{
		//m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);
	}

	lights[0].position.x = P_Player.getPosition().x;
	lights[0].position.z = P_Player.getPosition().z;
	//m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y + FPC.position.y*0.1, lights[0].position.z, 0, 0, 0, 0, 1, 0);
	m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y, lights[0].position.z, lights[0].position.x + 1, lights[0].position.y -10, lights[0].position.z + 1, 0, 1, 0);

	RenderWorldShadow();
}

/******************************************************************************/
/*!
\brief
Render light
*/
/******************************************************************************/
void mainscene::RenderPassLight(void)
{
	m_renderPass = RENDER_PASS_LIGHT;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetWindowWidth(), Application::GetWindowHeight());

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glUseProgram(m_lightShaderID);

	for (unsigned i = 0; i < GBuffer::GBUFFER_NUM_TEXTURES; ++i)
	{
		m_gBuffer.BindForReading(GL_TEXTURE0 + i, i);
		glUniform1i(m_parameters[U_POSITION_MAP_LIGHTPASS + i], 0 + i);
	}

	float screenSize[2] = { (float)Application::GetWindowWidth(), (float)Application::GetWindowHeight() };
	glUniform2fv(m_parameters[U_SCREEN_SIZE_LIGHTPASS], 1, &screenSize[0]);

	//Point light - local light without shadow
	/*for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = static_cast<GameObject *>(*it);
		Position pos(go->pos.x, go->pos.y, go->pos.z);
		Position lightPosition_cameraspace = viewStack.Top() * pos;
		glUniform1i(m_parameters[U_LIGHT_TYPE_LIGHTPASS], LIGHT_POINT);
		glUniform3fv(m_parameters[U_LIGHT_POSITION_LIGHTPASS], 1, &lightPosition_cameraspace.x);
		glUniform3fv(m_parameters[U_LIGHT_COLOR_LIGHTPASS], 1, &go->material.kDiffuse.r);
		glUniform1f(m_parameters[U_LIGHT_POWER_LIGHTPASS], go->lightPower);
		glUniform1f(m_parameters[U_LIGHT_RADIUS_LIGHTPASS], go->lightRadius);
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->lightRadius, go->lightRadius, go->lightRadius);
		RenderMesh(meshList[GEO_RENDERING_SPHERE], false);
		modelStack.PopMatrix();
	}*/

	glEnable(GL_CULL_FACE);

	//Directional light - the global light with shadow
	Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
	glUniform1i(m_parameters[U_LIGHT_TYPE_LIGHTPASS], LIGHT_DIRECTIONAL);
	glUniform3fv(m_parameters[U_LIGHT_POSITION_LIGHTPASS], 1, &lightDirection_cameraspace.x);
	glUniform3fv(m_parameters[U_LIGHT_COLOR_LIGHTPASS], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT_POWER_LIGHTPASS], lights[0].power);
	projectionStack.PushMatrix();
	viewStack.PushMatrix();
	modelStack.PushMatrix();
	projectionStack.LoadIdentity();
	viewStack.LoadIdentity();
	modelStack.LoadIdentity();

	RenderMesh(meshList[GEO_RENDERING_QUAD], false);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

/******************************************************************************/
/*!
\brief
Render pass GBuffer
*/
/******************************************************************************/
void mainscene::RenderPassGBuffer(void)
{
	m_renderPass = RENDER_PASS_GBUFFER;

	m_gBuffer.BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_gBufferShaderID);

	//pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP_GBUFFER], 8);


	//RenderWorldShadow();
	//RenderWorldNoShadow();
}

/******************************************************************************/
/*!
\brief
Renders the scene
*/
/******************************************************************************/
void mainscene::Render(void)
{
	for (unsigned i = 0; i < 1; ++i)
	{
		if (lights[i].type == LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(lights[i].position.x, lights[i].position.y, lights[i].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + (i * NUM_LIGHT_PARAM)], 1, &lightDirection_cameraspace.x);
		}
		else if (lights[i].type == LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[i].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + (i * NUM_LIGHT_PARAM)], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * lights[i].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION + (i * NUM_LIGHT_PARAM)], 1, &spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[i].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + (i * NUM_LIGHT_PARAM)], 1, &lightPosition_cameraspace.x);
		}
	}

	viewStack.LoadIdentity();

	viewStack.LookAt(
		FPC.position.x, FPC.position.y, FPC.position.z,
		FPC.target.x, FPC.target.y, FPC.target.z,
		FPC.up.x, FPC.up.y, FPC.up.z
		);


	modelStack.LoadIdentity();

	RenderPassGPass();
	RenderPassGBuffer();
	RenderPassLight();
	RenderPassMain();
}

/******************************************************************************/
/*!
\brief
Clears memory upon exit
*/
/******************************************************************************/
void mainscene::Exit(void)
{
	if (engine != NULL)
	{
		engine->drop();
	}

	while (BIv_BulletList.size() > 0)
	{
		BulletInfo *BI = BIv_BulletList.back();
		if (BI != NULL)
		{
			delete BI;
			BI = NULL;
		}
		
		BIv_BulletList.pop_back();
	}

	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		if (go != NULL)
		{
			delete go;
			go = NULL;
		}
		m_goList.pop_back();
	}

	while (m_ParList.size() > 0)
	{
		Particle *Par = m_ParList.back();
		if (Par != NULL)
		{
			delete Par;
			Par = NULL;
		}
		
		m_ParList.pop_back();
	}

	glDeleteProgram(m_gPassShaderID);
	glDeleteProgram(m_gBufferShaderID);
	glDeleteProgram(m_lightShaderID);

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
	glDeleteProgram(m_gPassShaderID);
}