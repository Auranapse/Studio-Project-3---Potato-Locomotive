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
	f_mouseSensitivity = static_cast<float>(std::stoi(*SF_1.Data[14]))/100.f;
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
	weaponsEnabled = false;
	//Control initialization--------------
	for(unsigned i = 0; i < E_CTRL_TOTAL; ++i)
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
	m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");
	m_programID = LoadShaders("GameData//Shader//Shadow.vertexshader", "GameData//Shader//Shadow.fragmentshader" );
	m_gBufferShaderID = LoadShaders("GameData//Shader//GBuffer.vertexshader", "GameData//Shader//GBuffer.fragmentshader" );
	m_lightShaderID = LoadShaders("GameData//Shader//LightPass.vertexshader", "GameData//Shader//LightPass.fragmentshader" );
	
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
	
	lights[0].type = LIGHT_DIRECTIONAL;
	lights[0].position.Set(10.f, 100.f, 0.f);
	lights[0].color.Set(1.f, 1.f, 1.f);
	lights[0].power = 1.0f;
	

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
	meshList[GEO_SNIPER_CROSSHAIR_1] = MeshBuilder::GenerateQuad("Crosshair Sniper Part 1", Color(0.f, 0.f, 0.f), 1.0f, 1.0f, 1.f);
	meshList[GEO_SNIPER_CROSSHAIR_1]->textureID[0] = LoadTGA("GameData//Image//weapons//SNIPER_SCOPE.tga", true);
	meshList[GEO_SNIPER_CROSSHAIR_2] = MeshBuilder::GenerateQuad("Crosshair Sniper Part 2", Color(0.f, 0.f, 0.f), .5f, 1.0f, 1.f);
	meshList[GEO_FLOOR_TILE] = MeshBuilder::GenerateQuad("Room floor", Color(1.f, 1.f, 1.f), 1.f, 1.f, 40.f);
	meshList[GEO_FLOOR_TILE_SQUARE] = MeshBuilder::GenerateQuad("Room floor square to scale", Color(1.f, 1.f, 1.f), 1.f, 1.f, 4.f);
	meshList[GEO_FLOOR_CONCRETE] = MeshBuilder::GenerateQuad("Office floor square", Color(1.f, 1.f, 1.f), 1.f, 1.f, 20.f);//ViewRoom
	meshList[GEO_CHAMBER] = MeshBuilder::GenerateQuad("Chamber7", Color(1.f, 1.f, 1.f), 3.f, 1.f, 1.f);//ViewRoom
	meshList[GEO_GLASSR1] = MeshBuilder::GenerateQuad("Room glass rectangle", Color(1.f, 1.f, 1.f), 2.f, 0.6f, 1.f);
	meshList[GEO_WALL_PANEL] = MeshBuilder::GenerateQuad("Portal panel wall 1", Color(1.f, 1.f, 1.f), 2.f, 0.6f, 3.5f);
	meshList[GEO_WALL_PANEL2] = MeshBuilder::GenerateQuad("Portal panel wall 2", Color(1.f, 1.f, 1.f), 0.6f, 0.6f, 3.5f);
	meshList[GEO_WALL_PANEL_SQUARE] = MeshBuilder::GenerateQuad("Portal panel wall square to scale", Color(1.f, 1.f, 1.f), 1.f, 1.f, 1.f);
	meshList[GEO_WALL_WORN1] = MeshBuilder::GenerateQuad("Worn Wall", Color(1.f, 1.f, 1.f), 0.6f, 0.6f, 3.5f);
	meshList[GEO_WALL_WORN_SQUARE] = MeshBuilder::GenerateQuad("Worn Wall square to scale", Color(1.f, 1.f, 1.f), 1.f, 1.f, 1.f);
	meshList[GEO_WALL2] = MeshBuilder::GenerateQuad("Room wall2 rectagle black pattern", Color(1.f, 1.f, 1.f), 2.f, 0.6f, 6.f);
	meshList[GEO_WALL3] = MeshBuilder::GenerateQuad("Office Wall texture", Color(1.f, 1.f, 1.f), 1.f, 1.f, 10.f);//View room
	meshList[GEO_WALLRUSTED1] = MeshBuilder::GenerateQuad("Metal rusted wall", Color(1.f, 1.f, 1.f), 2.f, 0.6f, 3.5f);//Ruins area wall

	meshList[GEO_WSC_SPHERE_COMPANION] = MeshBuilder::GenerateSphere("Companion Cube round middle", Color(1.f, 0.7f, 0.96f), 18, 36, 0.53f);
	meshList[GEO_WSC_SPHERE] = MeshBuilder::GenerateSphere("Weighted Storage Cube round middle", Color(1.f, 1.f, 1.f), 18, 36, 0.53f);
	meshList[GEO_WSC] = MeshBuilder::GenerateCubeT2("Weighted Storage Cube", Color(0.4f, 0.4f, 0.4f), 0.5f, 0.5f, 0.5f, 1.f);
	meshList[GEO_WSC_MID_EDGE] = MeshBuilder::GenerateCubeT2("Cube middle edge", Color(1.0f, 1.0f, 1.0f), 0.5f, 0.5f, 0.5f, 1.f);
	meshList[GEO_WSC_SIDE_EDGE] = MeshBuilder::GenerateCubeT2("Cube side edge", Color(1.0f, 1.0f, 1.0f), 0.5f, 0.5f, 0.5f, 1.f);
	meshList[GEO_WSC_LINES_COMPANION] = MeshBuilder::GenerateCubeT2("Connecting lines Pink", Color(1.f, 0.56f, 0.77f), 0.5f, 0.5f, 0.5f, 1.f);
	meshList[GEO_WSC_LINES] = MeshBuilder::GenerateCubeT2("Connecting lines Blue", Color(0.7f, 0.7f, 1.f), 0.5f, 0.5f, 0.5f, 1.f);

	meshList[GEO_OLDCUBE1] = MeshBuilder::GenerateCube("Old Aperture cube", Color(1.f, 1.f, 1.f), 1.f, 1.f, 1.f);

	meshList[GEO_TURRETCYLINDER] = MeshBuilder::GenerateCylinder("TURRETMID", Color(0.3f, 0.3f, 0.3f), 1, 5.f, 5.f);
	meshList[GEO_TURRETBODY] = MeshBuilder::GeneratePartofSphere("SIDES OF TURRET", Color(1.0f, 1.0f, 1.0f), 18, 30, 0.53f, 360.f, 40.f);
	meshList[GEO_TURRETBODYMAIN] = MeshBuilder::GeneratePartofSphere("TURRET MAIN BODY", Color(1.0f, 1.0f, 1.0f), 18, 16, 0.53f, 180.f, 180.f);
	meshList[GEO_TURRETGUNMOUNT1] = MeshBuilder::GenerateCubeT2("Turret Gun holder Part 1", Color(0.3f, 0.3f, 0.3f), 0.5f, 0.5f, 0.5f, 1.f);
	meshList[GEO_TURRETGUNMOUNT2] = MeshBuilder::GenerateCubeT2("Turret Gun holder Part 2", Color(0.1f, 0.1f, 0.1f), 0.5f, 0.5f, 0.5f, 1.f);
	meshList[GEO_TURRETGUNMOUNT3] = MeshBuilder::GenerateCubeT2("Turret Gun holder Part 3", Color(0.6f, 0.6f, 0.6f), 0.5f, 0.5f, 0.5f, 1.f);
	meshList[GEO_TURRETLEGSP1] = MeshBuilder::GeneratePartofSphere("LEGS OF TURRET 1", Color(0.3f, 0.3f, 0.3f), 18, 16, 0.53f, 360.f, 40.f);
	meshList[GEO_TURRETLEGSP2] = MeshBuilder::GeneratePartofSphere("LEGS OF TURRET 2", Color(1.0f, 1.0f, 1.0f), 18, 16, 0.53f, 360.f, 40.f);
	meshList[GEO_TURRETEYES1] = MeshBuilder::GenerateSphere("Turret Eyes 1", Color(1.f, 0.f, 0.f), 18, 18, 1.f);
	meshList[GEO_TURRETANTENNA] = MeshBuilder::GenerateCylinder("TURRET's Antenna", Color (0.3f, 0.3f, 0.3f), 1, 0.7f, 6.f);
	meshList[GEO_GUNBARREL1] = MeshBuilder::GenerateHollowCylinder("Gun Barrel P1", Color(0.6f, 0.6f, 0.6f), 1, 0.7f, 1.f, 6.f);
	meshList[GEO_GUNBARREL2] = MeshBuilder::GenerateRing("Gun Barrel P2", Color(0.6f, 0.6f, 0.6f), 36, 1.f, 2.f);

	meshList[GEO_BULLETP1] = MeshBuilder::GenerateCylinder("Gun bullet P1", Color(1.0f, 0.77f, 0.01f), 1, 0.7f, 6.f);
	meshList[GEO_BULLETP2] = MeshBuilder::GeneratePartofSphere("Gun bullet P2", Color(0.7f, 0.54f, 0.0f), 18, 36, 0.53f, 360.f, 90.f);

	meshList[GEO_FLight] = MeshBuilder::GenerateCylinder("The faulty light that flickers", Color(1.0f, 1.0f, 1.0f), 1, 0.7f, 6.f);

	meshList[GEO_LIGHT] = MeshBuilder::GenerateSphere("THELIGHT", Color(1.0, 1.0, 1.0), 9, 18, 1);

	meshList[GEO_MOSS1] = MeshBuilder::GenerateQuad("Moss1", Color(1, 1, 1), 1, 1, 1);
	meshList[GEO_MOSS2] = MeshBuilder::GenerateQuad("Moss2", Color(1, 1, 1), 1, 1, 1);
	meshList[GEO_MOSS3] = MeshBuilder::GenerateQuad("Moss3", Color(1, 1, 1), 1, 1, 1);

	meshList[GEO_APERTUREOLDLOGO] = MeshBuilder::GenerateQuad("Aperture Logo", Color(1, 1, 1), 2, 1, 1);
	meshList[GEO_WALL_WORN_RUINS1] = MeshBuilder::GenerateQuad("Ruin walls", Color(1, 1, 1), 1, 1, 20);

	//Load OBJ Models
	meshList[GEO_OBJCAKE] = MeshBuilder::GenerateOBJ("CAKE", "GameData//OBJ//CAKE.obj");
	meshList[GEO_OBJTABLE] = MeshBuilder::GenerateOBJ("Aperture Table", "GameData//OBJ//Aperture_Table.obj");
	meshList[GEO_PANELP1] = MeshBuilder::GenerateOBJ("Panel P1", "GameData//OBJ//PanelP1.obj");
	meshList[GEO_PANELP2] = MeshBuilder::GenerateOBJ("Panel P2", "GameData//OBJ//PanelP2.obj");
	meshList[GEO_PANELP3] = MeshBuilder::GenerateOBJ("Panel P3", "GameData//OBJ//PanelP3.obj");
	meshList[GEO_OBJ_TV_A] = MeshBuilder::GenerateOBJ("Orange juice can", "GameData//OBJ//TV_A.obj");
	meshList[GEO_ELSHAFT] = MeshBuilder::GenerateOBJ("Elevator shaft", "GameData//OBJ//SupportE.obj");
	meshList[GEO_ELBOTTOM] = MeshBuilder::GenerateOBJ("Elevator stopping thing", "GameData//OBJ//BottomE.obj");
	meshList[GEO_DOORA] = MeshBuilder::GenerateOBJ("Door VA", "GameData//OBJ//door_a.obj");
	meshList[GEO_ELEVATOR] = MeshBuilder::GenerateOBJ("Magical Elevator", "GameData//OBJ//Elevator.obj");
	meshList[GEO_RAILING] = MeshBuilder::GenerateOBJ("Railing", "GameData//OBJ//Railing.obj");

	meshList[GEO_LOWERH] = MeshBuilder::GenerateOBJ("Hydraulics1", "GameData//OBJ//LowerHydraulic.obj");
	meshList[GEO_UPPERH] = MeshBuilder::GenerateOBJ("Hydraulics2", "GameData//OBJ//UpperHydraulic.obj");

	meshList[GEO_BUTTON_STAND] = MeshBuilder::GenerateOBJ("Button Stand", "GameData//OBJ//Button_Stand.obj");
	meshList[GEO_BUTTON] = MeshBuilder::GenerateOBJ("Button", "GameData//OBJ//Button.obj");
	meshList[GEO_DISPENSER] = MeshBuilder::GenerateOBJ("Cube Dispenser", "GameData//OBJ//Dispenser.obj");
	
	meshList[GEO_GEL] = MeshBuilder::GenerateSphere("Gel", Color(0.f, 0.5f, 1.0f), 5, 5, 1.f);

	//WEAPONS
	//if(weaponsEnabled)
	{
		meshList[GEO_BULLET] = MeshBuilder::GenerateSphere("Gun bullet", Color(1.f, 0.9f, 0.5f), 10, 10, 0.53f);

		meshList[GEO_M9] = MeshBuilder::GenerateOBJ("M9", "GameData//OBJ//weapons//M9.obj");
		meshList[GEO_M9]->textureID[0] = LoadTGA("GameData//Image//weapons//M9.tga", true);

		meshList[GEO_MP5K] = MeshBuilder::GenerateOBJ("MP5K", "GameData//OBJ//weapons//MP5K.obj");
		meshList[GEO_MP5K]->textureID[0] = LoadTGA("GameData//Image//weapons//MP5K.tga", true);

		meshList[GEO_M4A1] = MeshBuilder::GenerateOBJ("M4A1", "GameData//OBJ//weapons//M4A1.obj");
		meshList[GEO_M4A1]->textureID[0] = LoadTGA("GameData//Image//weapons//M4A1.tga", true);

		meshList[GEO_L11A3_1] = MeshBuilder::GenerateOBJ("L11A3", "GameData//OBJ//weapons//L11A3BODY.obj");
		meshList[GEO_L11A3_1]->textureID[0] = LoadTGA("GameData//Image//weapons//L11A3BODY.tga", true);
		meshList[GEO_L11A3_2] = MeshBuilder::GenerateOBJ("L11A3 Scope", "GameData//OBJ//weapons//L11A3SCOPE.obj");
		meshList[GEO_L11A3_2]->textureID[0] = LoadTGA("GameData//Image//weapons//L11A3SCOPE.tga", true);
		meshList[GEO_SPAS12] = MeshBuilder::GenerateOBJ("SPAS-12", "GameData//OBJ//weapons//SPAS12.obj");
		meshList[GEO_SPAS12]->textureID[0] = LoadTGA("GameData//Image//weapons//SPAS12.tga", true);
	}

	meshList[GEO_TARGET] = MeshBuilder::GenerateOBJ("M4A1", "GameData//OBJ//other//TARGET.obj");
	meshList[GEO_TARGET]->textureID[0] = LoadTGA("GameData//Image//other//TARGET.tga", true);
	meshList[GEO_TARGETBASE] = MeshBuilder::GenerateOBJ("M4A1", "GameData//OBJ//other//TARGETBASE.obj");
	meshList[GEO_TARGETBASE]->textureID[0] = LoadTGA("GameData//Image//other//TARGET.tga", true);

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
	//---------------------TERRAIN
	meshList[GEO_WATER_SPRITE] = MeshBuilder::GenerateSpriteAnimation("Water", 32, 1);
	meshList[GEO_WATER_SPRITE]->textureID[0] = LoadTGA("GameData//Image//terrain//water.tga");
	SA_waterSprite = dynamic_cast<SpriteAnimation*>(meshList[GEO_WATER_SPRITE]);
	SA_waterSprite->m_anim = new Animation();
	SA_waterSprite->m_anim->Set(0, 32, 2, true);

	meshList[GEO_TERRAIN] = MeshBuilder::GenerateTerrain("Terrain", "GameData//Image//terrain//heightmap.raw", monalisa.uc_heightmap);
	meshList[GEO_TERRAIN]->textureID[0] = LoadTGA("GameData//Image//terrain//moss1.tga");
	monalisa.mesh = meshList[GEO_TERRAIN];
	monalisa.position.Set(1200, 1003, 1680);
	//monalisa.position.Set(0, 0, 0);
	monalisa.scale.Set(1600, 120, 1600);
	monalisa.ColEnable = true;

	meshList[GEO_TREE_1] = MeshBuilder::GenerateQuad("Tree 1", Color(1, 1, 1), 1, 1, 1);
	meshList[GEO_TREE_1]->textureID[0] = LoadTGA("GameData//Image//environment//tree1.tga", false);

	//---------------------------------------------------------------------------------------
	//Text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID[0] = LoadTGA("GameData//Image//font//inputm.tga", false);

	//UI
	meshList[GEO_UI_AMMO] = MeshBuilder::GenerateQuad("Ammo", Color(1.f, 1.f, 1.f), 1.28f, 0.72f, 1.f);
	meshList[GEO_UI_AMMO]->textureID[0] = LoadTGA("GameData//Image//UI//ingame//Ammo.tga", false);
	//Minimap
	meshList[GEO_MINIMAP_BASE] = MeshBuilder::GenerateQuad("Minimap", Color(1.f, 1.f, 1.f), 1.f, 1.f, 1.f);
	meshList[GEO_MINIMAP_BASE]->textureID[0] = LoadTGA("GameData//Image//UI//ingame//minimap.tga", false);
	meshList[GEO_MINIMAP_PLAYER] = MeshBuilder::GenerateQuad("MinimapPlayer", Color(1.f, 1.f, 1.f), 1.f, 1.f, 1.f);
	meshList[GEO_MINIMAP_PLAYER]->textureID[0] = LoadTGA("GameData//Image//UI//ingame//prograde.tga", false);
	//Generate Textures---------------------------------------------------------------------
	meshList[GEO_FLOOR_TILE]->textureID[0] = LoadTGA("GameData//Image//floortexture.tga", false);
	meshList[GEO_FLOOR_TILE_SQUARE]->textureID[0] = LoadTGA("GameData//Image//floortexture.tga", false);
	meshList[GEO_FLOOR_CONCRETE]->textureID[0] = LoadTGA("GameData//Image//concrete_floor.tga", false);
	meshList[GEO_WALL_PANEL]->textureID[0] = LoadTGA("GameData//Image//walltexture.tga", false);
	meshList[GEO_WALL_PANEL2]->textureID[0] = LoadTGA("GameData//Image//walltexture.tga", false);
	meshList[GEO_WALL_PANEL_SQUARE]->textureID[0] = LoadTGA("GameData//Image//walltexture.tga", true);
	meshList[GEO_WALL_WORN1]->textureID[0] = LoadTGA("GameData//Image//concrete_wall.tga", false);
	meshList[GEO_WALL_WORN_SQUARE]->textureID[0] = LoadTGA("GameData//Image//concrete_wall.tga", false);
	meshList[GEO_WALL2]->textureID[0] = LoadTGA("GameData//Image//walltexture2.tga", false);
	meshList[GEO_WALL3]->textureID[0] = LoadTGA("GameData//Image//walltexture3.tga", false);
	meshList[GEO_GLASSR1]->textureID[0] = LoadTGA("GameData//Image//Glass.tga", false);
	meshList[GEO_WALLRUSTED1]->textureID[0] = LoadTGA("GameData//Image//wall_rusted1.tga", false);

	meshList[GEO_APERTUREOLDLOGO]->textureID[0] = LoadTGA("GameData//Image//other//Aperture_innovators.tga", false);
	meshList[GEO_WALL_WORN_RUINS1]->textureID[0] = LoadTGA("GameData//Image//wall_ruin.tga", false);

	meshList[GEO_OLDCUBE1]->textureID[0] = LoadTGA("GameData//Image//other//CubeUV.tga", false);
	meshList[GEO_CHAMBER]->textureID[0] = LoadTGA("GameData//Image//other//chamber17.tga", false);
	meshList[GEO_MOSS1]->textureID[0] = LoadTGA("GameData//Image//other//moss1.tga", false);
	meshList[GEO_MOSS2]->textureID[0] = LoadTGA("GameData//Image//other//moss2.tga", false);
	meshList[GEO_MOSS3]->textureID[0] = LoadTGA("GameData//Image//other//moss3.tga", false);

	meshList[GEO_OBJCAKE]->textureID[0] = LoadTGA("GameData//Image//OBJ//Cake_UV.tga", true);
	meshList[GEO_OBJTABLE]->textureID[0] = LoadTGA("GameData//Image//OBJ//Aperture_Table_UV.tga", true);
	meshList[GEO_PANELP1]->textureID[0] = LoadTGA("GameData//Image//OBJ//PanelP1.tga", true);
	meshList[GEO_PANELP2]->textureID[0] = LoadTGA("GameData//Image//OBJ//PanelP2.tga", true);
	meshList[GEO_PANELP3]->textureID[0] = LoadTGA("GameData//Image//OBJ//PanelP2.tga", true);
	meshList[GEO_OBJ_TV_A]->textureID[0] = LoadTGA("GameData//Image//OBJ//TV_A_UV.tga", true);
	meshList[GEO_ELSHAFT]->textureID[0] = LoadTGA("GameData//Image//OBJ//shaft_UV.tga", false);
	meshList[GEO_ELBOTTOM]->textureID[0] = LoadTGA("GameData//Image//OBJ//shaft_UV.tga", false);
	meshList[GEO_DOORA]->textureID[0] = LoadTGA("GameData//Image//OBJ//Door_A_UV.tga", false);
	meshList[GEO_ELEVATOR]->textureID[0] = LoadTGA("GameData//Image//OBJ//Elevator_UV.tga", false);
	meshList[GEO_RAILING]->textureID[0] = LoadTGA("GameData//Image//wall_rusted1.tga", true);

	meshList[GEO_LOWERH]->textureID[0] = LoadTGA("GameData//Image//OBJ//LowerHydraulic_UV.tga", true);
	meshList[GEO_UPPERH]->textureID[0] = LoadTGA("GameData//Image//OBJ//UpperHydraulic_UV.tga", true);

	meshList[GEO_BUTTON_STAND]->textureID[0] = LoadTGA("GameData//Image//OBJ//Button_Stand_UV.tga", true);
	meshList[GEO_BUTTON]->textureID[0] = LoadTGA("GameData//Image//OBJ//Button_UV.tga", true);
	meshList[GEO_DISPENSER]->textureID[0] = LoadTGA("GameData//Image//OBJ//Dispenser_UV.tga", true);

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
	/*
	RenderMeshin2D(meshList[GEO_LIGHT_DEPTH_QUAD], false, 20, 50, 50);
	//*/
	//Lighting-------------------------------------------------------------------------------
	meshList[GEO_DOORA]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_DOORA]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_DOORA]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_DOORA]->material.kShininess = 1.0f;

	meshList[GEO_ELEVATOR]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_ELEVATOR]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_ELEVATOR]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_ELEVATOR]->material.kShininess = 1.0f;

	meshList[GEO_FLOOR_TILE]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_FLOOR_TILE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FLOOR_TILE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_FLOOR_TILE]->material.kShininess = 1.0f;

	meshList[GEO_FLOOR_TILE_SQUARE]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_FLOOR_TILE_SQUARE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_FLOOR_TILE_SQUARE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_FLOOR_TILE_SQUARE]->material.kShininess = 1.0f;

	meshList[GEO_FLOOR_CONCRETE]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_FLOOR_CONCRETE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_FLOOR_CONCRETE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_FLOOR_CONCRETE]->material.kShininess = 1.0f;

	meshList[GEO_DISPENSER]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_DISPENSER]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_DISPENSER]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_DISPENSER]->material.kShininess = 5.0f;

	meshList[GEO_PANELP1]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_PANELP1]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_PANELP1]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_PANELP1]->material.kShininess = 1.0f;

	meshList[GEO_PANELP2]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_PANELP2]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_PANELP2]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_PANELP2]->material.kShininess = 1.0f;

	meshList[GEO_PANELP3]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_PANELP3]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_PANELP3]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_PANELP3]->material.kShininess = 1.0f;

	meshList[GEO_LOWERH]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_LOWERH]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_LOWERH]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_LOWERH]->material.kShininess = 1.0f;

	meshList[GEO_UPPERH]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_UPPERH]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_UPPERH]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_UPPERH]->material.kShininess = 1.0f;

	meshList[GEO_WALL_PANEL]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_WALL_PANEL]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WALL_PANEL]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WALL_PANEL]->material.kShininess = 2.0f;

	meshList[GEO_WALL_PANEL2]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_WALL_PANEL2]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WALL_PANEL2]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WALL_PANEL2]->material.kShininess = 2.0f;

	meshList[GEO_WALL_PANEL_SQUARE]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_WALL_PANEL_SQUARE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WALL_PANEL_SQUARE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WALL_PANEL_SQUARE]->material.kShininess = 2.0f;

	meshList[GEO_WALL2]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_WALL2]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WALL2]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WALL2]->material.kShininess = 7.0f;

	meshList[GEO_WALL3]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_WALL3]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WALL3]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WALL3]->material.kShininess = 1.0f;

	meshList[GEO_CHAMBER]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_CHAMBER]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_CHAMBER]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_CHAMBER]->material.kShininess = 8.0f;

	meshList[GEO_WALL_WORN1]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_WALL_WORN1]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WALL_WORN1]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WALL_WORN1]->material.kShininess = 1.0f;

	meshList[GEO_WALL_WORN_SQUARE]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_WALL_WORN_SQUARE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WALL_WORN_SQUARE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WALL_WORN_SQUARE]->material.kShininess = 1.0f;

	meshList[GEO_WALL_WORN_RUINS1]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_WALL_WORN_RUINS1]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WALL_WORN_RUINS1]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WALL_WORN_RUINS1]->material.kShininess = 1.0f;

	meshList[GEO_WALLRUSTED1]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_WALLRUSTED1]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WALLRUSTED1]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WALLRUSTED1]->material.kShininess = 1.0f;

	meshList[GEO_RAILING]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_RAILING]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_RAILING]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_RAILING]->material.kShininess = 1.0f;

	meshList[GEO_WSC_SPHERE]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_WSC_SPHERE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WSC_SPHERE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WSC_SPHERE]->material.kShininess = 1.0f;

	meshList[GEO_WSC_SPHERE_COMPANION]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_WSC_SPHERE_COMPANION]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WSC_SPHERE_COMPANION]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WSC_SPHERE_COMPANION]->material.kShininess = 1.0f;

	meshList[GEO_WSC]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_WSC]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_WSC]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_WSC]->material.kShininess = 1.0f;

	meshList[GEO_WSC_MID_EDGE]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_WSC_MID_EDGE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WSC_MID_EDGE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WSC_MID_EDGE]->material.kShininess = 1.2f;

	meshList[GEO_WSC_SIDE_EDGE]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_WSC_SIDE_EDGE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WSC_SIDE_EDGE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WSC_SIDE_EDGE]->material.kShininess = 1.2f;

	meshList[GEO_WSC_LINES_COMPANION]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_WSC_LINES_COMPANION]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WSC_LINES_COMPANION]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WSC_LINES_COMPANION]->material.kShininess = 1.0f;

	meshList[GEO_WSC_LINES]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_WSC_LINES]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_WSC_LINES]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WSC_LINES]->material.kShininess = 1.0f;

	meshList[GEO_BUTTON_STAND]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_BUTTON_STAND]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BUTTON_STAND]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_BUTTON_STAND]->material.kShininess = 0.1f;

	meshList[GEO_BUTTON]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_BUTTON]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BUTTON]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_BUTTON]->material.kShininess = 0.1f;

	meshList[GEO_TURRETBODY]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_TURRETBODY]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TURRETBODY]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_TURRETBODY]->material.kShininess = 20.0f;

	meshList[GEO_TURRETBODYMAIN]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_TURRETBODYMAIN]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TURRETBODYMAIN]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_TURRETBODYMAIN]->material.kShininess = 20.0f;

	meshList[GEO_TURRETCYLINDER]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_TURRETCYLINDER]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TURRETCYLINDER]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_TURRETCYLINDER]->material.kShininess = 1.0f;

	meshList[GEO_TURRETGUNMOUNT1]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_TURRETGUNMOUNT1]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TURRETGUNMOUNT1]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_TURRETGUNMOUNT1]->material.kShininess = 1.0f;

	meshList[GEO_TURRETGUNMOUNT2]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_TURRETGUNMOUNT2]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TURRETGUNMOUNT2]->material.kSpecular.Set(0.12f, 0.12f, 0.12f);
	meshList[GEO_TURRETGUNMOUNT2]->material.kShininess = 1.0f;

	meshList[GEO_TURRETGUNMOUNT3]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_TURRETGUNMOUNT3]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_TURRETGUNMOUNT3]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_TURRETGUNMOUNT3]->material.kShininess = 3.0f;

	meshList[GEO_GUNBARREL1]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_GUNBARREL1]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_GUNBARREL1]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_GUNBARREL1]->material.kShininess = 30.0f;

	meshList[GEO_GUNBARREL2]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_GUNBARREL2]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_GUNBARREL2]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_GUNBARREL2]->material.kShininess = 30.0f;

	meshList[GEO_TURRETLEGSP1]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_TURRETLEGSP1]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TURRETLEGSP1]->material.kSpecular.Set(0.12f, 0.12f, 0.12f);
	meshList[GEO_TURRETLEGSP1]->material.kShininess = 1.0f;

	meshList[GEO_TURRETLEGSP2]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_TURRETLEGSP2]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TURRETLEGSP2]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_TURRETLEGSP2]->material.kShininess = 20.0f;

	meshList[GEO_TURRETANTENNA]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_TURRETANTENNA]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TURRETANTENNA]->material.kSpecular.Set(0.12f, 0.12f, 0.12f);
	meshList[GEO_TURRETANTENNA]->material.kShininess = 1.0f;

	meshList[GEO_TURRETEYES1]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_TURRETEYES1]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TURRETEYES1]->material.kSpecular.Set(0.12f, 0.12f, 0.12f);
	meshList[GEO_TURRETEYES1]->material.kShininess = 1.0f;

	meshList[GEO_BULLETP1]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_BULLETP1]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_BULLETP1]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BULLETP1]->material.kShininess = 30.0f;

	meshList[GEO_BULLETP2]->material.kAmbient.Set(0.35f, 0.35f, 0.35f);
	meshList[GEO_BULLETP2]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BULLETP2]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_BULLETP2]->material.kShininess = 1.0f;

	meshList[GEO_GLASSR1]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_GLASSR1]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_GLASSR1]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_GLASSR1]->material.kShininess = 25.0f;

	meshList[GEO_FLight]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_FLight]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FLight]->material.kSpecular.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_FLight]->material.kShininess = 25.0f;

	meshList[GEO_OBJCAKE]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_OBJCAKE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_OBJCAKE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_OBJCAKE]->material.kShininess = 1.0f;

	meshList[GEO_OBJTABLE]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_OBJTABLE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_OBJTABLE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_OBJTABLE]->material.kShininess = 1.0f;

	meshList[GEO_OLDCUBE1]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_OLDCUBE1]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_OLDCUBE1]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_OLDCUBE1]->material.kShininess = 1.0f;

	meshList[GEO_OBJ_TV_A]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_OBJ_TV_A]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_OBJ_TV_A]->material.kSpecular.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_OBJ_TV_A]->material.kShininess = 12.0f;

	meshList[GEO_ELSHAFT]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_ELSHAFT]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_ELSHAFT]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_ELSHAFT]->material.kShininess = 1.0f;

	meshList[GEO_ELBOTTOM]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_ELBOTTOM]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_ELBOTTOM]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_ELBOTTOM]->material.kShininess = 1.0f;

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

		meshList[GEO_M4A1]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_M4A1]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
		meshList[GEO_M4A1]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_M4A1]->material.kShininess = 10.0f;

		meshList[GEO_L11A3_1]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_L11A3_1]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
		meshList[GEO_L11A3_1]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_L11A3_1]->material.kShininess = 10.0f;

		meshList[GEO_L11A3_2]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_L11A3_2]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
		meshList[GEO_L11A3_2]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_L11A3_2]->material.kShininess = 12.0f;

		meshList[GEO_SPAS12]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_SPAS12]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
		meshList[GEO_SPAS12]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_SPAS12]->material.kShininess = 12.0f;
	}
	meshList[GEO_TARGETBASE]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_TARGETBASE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TARGETBASE]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TARGETBASE]->material.kShininess = 10.0f;

	meshList[GEO_TARGET]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_TARGET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TARGET]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TARGET]->material.kShininess = 10.0f;


	//Starting position of translations and initialize physics

	//Turret
	TURRET.rotateA = 0.f;
	TURRET.translateX = 0.f;
	TURRET.translateY = 25.f;
	TURRET.translateZ = 0.f;

	S1.translateX = 0.f;
	S1.translateY = 0.f;
	S1.translateZ = 0.f;
	S2.translateX = 0.f;
	S2.translateY = 0.f;
	S2.translateZ = 0.f;


	TLEG1.rotateA = 0.f;
	TLEG2.rotateA = -10.f;
	TLEG3.rotateA = -98.f;
	BULLETR.translateY = 10.f;
	BULLETL.translateY = 10.f;

	LIFT.translateX = 200.f;
	LIFT.translateY = 0.f;
	LIFT.translateZ = 1680.f;

	LIFT2.translateX = 0.f;
	LIFT2.translateY = 0.f;
	LIFT2.translateZ = 980.f;
	LIFT2.rotateA = 90.f;

	SWITCH.translateX = 70.f;
	SWITCH.translateY = 0.f;
	SWITCH.translateZ = 700.f;
	SWITCH.rotateA = 20.f;

	SWITCH2.translateX = -500.f;
	SWITCH2.translateY = 0.f;
	SWITCH2.translateZ = -350.f;

	SWITCH3.translateX = 350.f;
	SWITCH3.translateY = 0.f;
	SWITCH3.translateZ = 200.f;

	ElevatorDown = true;
	ElevatorDown2 = false;

	Attackmode = false;
	moving = false;
	limitmove1 = false; 
	limitmove2 = false; 
	limitmove3 = false; 
	forward = true;
	turnleft = false; 
	turnright = false;
	renderAxis = false;
	firing = false;
	TURRET.Camera = false;
	mouseEnabled = true;
	if(mouseEnabled == true)
	{
		Application::SetMouseinput(Application::GetWindowWidth()*0.5, Application::GetWindowHeight()*0.5);
		Application::SetCursor(false);
	}
	LightSwitch = true;
	delayf = 0.f;
	FPScounter = 0.f;

	DisplayInfo = true;
	PanelOpen = false;
	PanelOpen2 = false;


	PanelP1.translateX = -120.f;
	PanelP1.translateY = 0.f;
	PanelP1.translateZ = 400.f;
	PanelP1.rotateA = -90.f;

	PanelP2.translateX = -400.f;
	PanelP2.translateY = 0.f;
	PanelP2.translateZ = -120.f;
	PanelP2.rotateA = -90.f;
	CubeDispense = false;

	for(unsigned short i = 0; i < 100; ++i)
	{
		BulletInfo* BI = new BulletInfo();
		BI->setStatus(false);
		BIv_BulletList.push_back(BI);
	}

	targetBoard *TB;
	for(unsigned i = 0; i <= 9; ++i)
	{
		TB = new targetBoard();
		TB->active = false;
		TB->rotation = 80.f;
		TB->health = 100.f;
		TB->life = 5.f;
		TB->collisionBox.Set(10, 38, 9);
		switch(i)
		{
		case 0:
			{
				TB->position.Set(-1000, 0, 80);
				break;
			}
		case 1:
			{
				TB->position.Set(-1200, 0, -220);
				break;
			}
		case 2:
			{
				TB->position.Set(-1800, 0, -10);
				break;
			}
		case 3:
			{
				TB->position.Set(-1500, 0, -120);
				break;
			}
		case 4:
			{
				TB->position.Set(-1400, 0, 200);
				break;
			}
		case 5:
			{
				TB->position.Set(-1100, 0, 300);
				break;
			}
		case 6:
			{
				TB->position.Set(-800, 0, -110);
				break;
			}
		case 7:
			{
				TB->position.Set(-1300, 0, 120);
				break;
			}
		case 8:
			{
				TB->position.Set(-1800, 53, 300);
				break;
			}
		case 9:
			{
				TB->position.Set(-1700, 0, -300);
				break;
			}
		}
		targetBList.push_back(TB);
	}

	ShootingRangeMode = false;
	resetShootingRange();
	targetsHit = 0;

	generateRoom1();
	generateRoom2();
	generateRuins();
	generateexterior();

	GOp_Player = new GameObject(GameObject::GO_PLAYER_CHAR);
	GOp_Player->active = true;
	GOp_Player->mass = 50;
	GOp_Player->scale.Set(10.f, 10.f, 10.f);
	GOp_Player->pos.Set(0.f, 100.f, 0.f);
	f_step = 0.f;

	ShakeState = false;
	V3_CamShaker.Set(0.f, 0.f, 0.f);
	FPC.Init(GOp_Player->pos, GOp_Player->pos + Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f), f_mouseSensitivity);

	gravity_force.Set(0.f, -9.82f * 20, 0.f);

	//if(weaponsEnabled)
	{
		initWeapons();
	}

	inputDelay = 0.f;
	timer = 0.f;
	liftDelay = 0.f;

	ISoundSource* backgroundAmbience;
	backgroundAmbience = engine->addSoundSourceFromFile("GameData//sounds//ambience//background.ogg");
	backgroundAmbience->setDefaultVolume(0.8f);
	engine->play2D(backgroundAmbience, true);

	soundList[ST_PANEL] = engine->addSoundSourceFromFile("GameData//sounds//other//panel.wav");
	soundList[ST_PANEL]->setDefaultVolume(4.5f);
	soundList[ST_PANEL]->setDefaultMinDistance(100.f);
	soundList[ST_STEP] = engine->addSoundSourceFromFile("GameData//sounds//other//step1.wav");
	soundList[ST_STEP_2] = engine->addSoundSourceFromFile("GameData//sounds//other//step2.wav");
	soundList[ST_KILL] = engine->addSoundSourceFromFile("GameData//sounds//other//kill.wav");
	soundList[ST_BUZZER] = engine->addSoundSourceFromFile("GameData//sounds//other//buzzer.wav");
	soundList[ST_ALERT] = engine->addSoundSourceFromFile("GameData//sounds//other//alert.wav");
}

void mainscene::InitShaders()
{
	e_nextScene = Application::E_SCENE_MENU;
}

Particle* mainscene::FetchParticle(void)
{
	for(unsigned i = 0; i < m_ParList.size(); ++i)
	{
		if(!m_ParList[i]->active)
		{
			m_ParList[i]->active = true;
			m_ParList[i]->ParticleType = Particle::PAR_DEFAULT;
			return m_ParList[i];
			break;
		}
	}

	for(unsigned i = 0; i < 10; ++i)
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
	fireRate = 0.f;
	reloadTimer = 0.f;

	for(unsigned i = 0; i < WT_TOTAL; ++i)
	{
		weaponList[i].mesh = NULL;
		weaponList[i].damage = 10;
		weaponList[i].fireRate = 1000;
		weaponList[i].bulletvelocity = 200;
		weaponList[i].name = "Gun";
		weaponList[i].recoilEffect = 2;
		weaponList[i].reloadTime = 1.2f;
		weaponList[i].ClipSize = 30;
		weaponList[i].CurrentClip = weaponList[i].ClipSize;
		weaponList[i].heldpos.Set(-4, -3, 12);
		weaponList[i].adspos.Set(0, -4, 12);
		weaponList[i].scale.Set(1, 1, 1);
		weaponList[i].currentpos = weaponList[i].heldpos;
		weaponList[i].adsZoom = 0.f;
		weaponList[i].bulletSpread = 0.f;
		weaponList[i].numBullet = 1;
		weaponList[i].reloadSound = NULL;
		weaponList[i].shootSound = NULL;
	}

	weaponList[WT_M9].mesh = meshList[GEO_M9];
	weaponList[WT_M9].damage = 40;
	weaponList[WT_M9].fireRate = 100;
	weaponList[WT_M9].bulletvelocity = 300.f;
	weaponList[WT_M9].name = "M9 Beretta";
	weaponList[WT_M9].recoilEffect = 50;
	weaponList[WT_M9].reloadTime = 1.2f;
	weaponList[WT_M9].ClipSize = 15;
	weaponList[WT_M9].CurrentClip = weaponList[WT_M9].ClipSize;
	weaponList[WT_M9].heldpos.Set(-5, -4, 9);
	weaponList[WT_M9].adspos.Set(0, -2.1f, 8);
	weaponList[WT_M9].scale.Set(0.03f, 0.03f, 0.03f);
	weaponList[WT_M9].currentpos = weaponList[WT_M9].heldpos;
	weaponList[WT_M9].adsZoom = 1.4f;
	weaponList[WT_M9].bulletSpread = 10.f;
	weaponList[WT_M9].reloadSound = engine->addSoundSourceFromFile("GameData//sounds//weapons//M9//RELOAD.wav");
	weaponList[WT_M9].shootSound = engine->addSoundSourceFromFile("GameData//sounds//weapons//M9//FIRE.wav");

	weaponList[WT_SPAS12].mesh = meshList[GEO_SPAS12];
	weaponList[WT_SPAS12].damage = 10;
	weaponList[WT_SPAS12].fireRate = 120;
	weaponList[WT_SPAS12].bulletvelocity = 305.f;
	weaponList[WT_SPAS12].name = "SPAS-12";
	weaponList[WT_SPAS12].recoilEffect = 300;
	weaponList[WT_SPAS12].reloadTime = 3.2f;
	weaponList[WT_SPAS12].ClipSize = 6;
	weaponList[WT_SPAS12].CurrentClip = weaponList[WT_SPAS12].ClipSize;
	weaponList[WT_SPAS12].heldpos.Set(-4.5f, -3.5f, 9);
	weaponList[WT_SPAS12].adspos.Set(0, -2.f, 8);
	weaponList[WT_SPAS12].scale.Set(0.2f, 0.2f, 0.2f);
	weaponList[WT_SPAS12].currentpos = weaponList[WT_SPAS12].heldpos;
	weaponList[WT_SPAS12].adsZoom = 1.4f;
	weaponList[WT_SPAS12].bulletSpread = 10.f;
	weaponList[WT_SPAS12].numBullet = 12;
	weaponList[WT_SPAS12].reloadSound = engine->addSoundSourceFromFile("GameData//sounds//weapons//SPAS12//RELOAD.wav");
	weaponList[WT_SPAS12].shootSound = engine->addSoundSourceFromFile("GameData//sounds//weapons//SPAS12//FIRE.wav");

	weaponList[WT_MP5K].mesh = meshList[GEO_MP5K];
	weaponList[WT_MP5K].damage = 40;
	weaponList[WT_MP5K].fireRate = 950;
	weaponList[WT_MP5K].bulletvelocity = 305.f;
	weaponList[WT_MP5K].name = "MP5K";
	weaponList[WT_MP5K].recoilEffect = 50;
	weaponList[WT_MP5K].reloadTime = 1.2f;
	weaponList[WT_MP5K].ClipSize = 30;
	weaponList[WT_MP5K].CurrentClip = weaponList[WT_MP5K].ClipSize;
	weaponList[WT_MP5K].heldpos.Set(-4.5f, -3.5f, 9);
	weaponList[WT_MP5K].adspos.Set(0, -2.95f, 8);
	weaponList[WT_MP5K].scale.Set(0.2f, 0.2f, 0.2f);
	weaponList[WT_MP5K].currentpos = weaponList[WT_MP5K].heldpos;
	weaponList[WT_MP5K].adsZoom = 1.4f;
	weaponList[WT_MP5K].bulletSpread = 125.f;
	weaponList[WT_MP5K].reloadSound = engine->addSoundSourceFromFile("GameData//sounds//weapons//MP5K//RELOAD.wav");
	weaponList[WT_MP5K].shootSound = engine->addSoundSourceFromFile("GameData//sounds//weapons//MP5K//FIRE.wav");

	weaponList[WT_M4A1].mesh = meshList[GEO_M4A1];
	weaponList[WT_M4A1].damage = 55;
	weaponList[WT_M4A1].fireRate = 700;
	weaponList[WT_M4A1].bulletvelocity = 320.f;
	weaponList[WT_M4A1].name = "M4A1 Carbine";
	weaponList[WT_M4A1].recoilEffect = 90;
	weaponList[WT_M4A1].reloadTime = 2.8f;
	weaponList[WT_M4A1].ClipSize = 30;
	weaponList[WT_M4A1].CurrentClip = weaponList[WT_M4A1].ClipSize;
	weaponList[WT_M4A1].heldpos.Set(-4.2f, -3.2f, 10);
	weaponList[WT_M4A1].adspos.Set(0, -2.98f, 10);
	weaponList[WT_M4A1].scale.Set(0.8f, 0.8f, 0.8f);
	weaponList[WT_M4A1].currentpos = weaponList[WT_M4A1].heldpos;
	weaponList[WT_M4A1].adsZoom = 1.4f;
	weaponList[WT_M4A1].bulletSpread = 70.f;
	weaponList[WT_M4A1].reloadSound = engine->addSoundSourceFromFile("GameData//sounds//weapons//M4A1//RELOAD.wav");
	weaponList[WT_M4A1].shootSound = engine->addSoundSourceFromFile("GameData//sounds//weapons//M4A1//FIRE.wav");

	weaponList[WT_L11A3].mesh = meshList[GEO_L11A3_1];
	weaponList[WT_L11A3].damage = 150;
	weaponList[WT_L11A3].fireRate = 50;
	weaponList[WT_L11A3].bulletvelocity = 320.f;
	weaponList[WT_L11A3].name = "L11A3";
	weaponList[WT_L11A3].recoilEffect = 300;
	weaponList[WT_L11A3].reloadTime = 2.2f;
	weaponList[WT_L11A3].ClipSize = 6;
	weaponList[WT_L11A3].CurrentClip = weaponList[WT_L11A3].ClipSize;
	weaponList[WT_L11A3].heldpos.Set(-4, -3, 10);
	weaponList[WT_L11A3].adspos.Set(0, -1.8f, 8);
	weaponList[WT_L11A3].scale.Set(2.5f, 2.5f, 2.5f);
	weaponList[WT_L11A3].currentpos = weaponList[WT_L11A3].heldpos;
	weaponList[WT_L11A3].adsZoom = 8.f;
	weaponList[WT_L11A3].bulletSpread = 1.f;
	weaponList[WT_L11A3].reloadSound = engine->addSoundSourceFromFile("GameData//sounds//weapons//L11A3//RELOAD.wav");
	weaponList[WT_L11A3].shootSound = engine->addSoundSourceFromFile("GameData//sounds//weapons//L11A3//FIRE.wav");
	renderScope = true;
	ScopeAnim = 15.f;
	scopeExeTime = 0.f;

	currentWeapon = WT_M9;
	fireRateMax = 60.f/static_cast<float>(weaponList[currentWeapon].fireRate);
	AimDownSight = false;
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
	GameObject *go;

	go = new GameObject(GameObject::GO_COMPANION_CUBE);
	go->pos.Set(130, 13.5, 120);
	go->rotationY = 10;
	go->scale.Set(1, 1, 1);
	go->ColBox.Set(13.5, 13.5, 13.5);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->dynamicRendering = true;
	m_goList.push_back(go);

	go = new GameObject(GameObject::GO_STORAGE_CUBE);
	go->pos.Set(-130, 13.5, -180);
	go->rotationY = 10;
	go->scale.Set(1, 1, 1);
	go->ColBox.Set(13.5, 13.5, 13.5);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->dynamicRendering = true;
	m_goList.push_back(go);

	go = new GameObject(GameObject::GO_STORAGE_CUBE);
	go->pos.Set(-130, 40.5, -178);
	go->rotationY = 30;
	go->scale.Set(1, 1, 1);
	go->ColBox.Set(13.5, 13.5, 13.5);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->dynamicRendering = true;
	m_goList.push_back(go);

	go = new GameObject(GameObject::GO_STORAGE_CUBE);
	go->pos.Set(-95, 13.5, -190);
	go->rotationY = -5;
	go->scale.Set(1, 1, 1);
	go->ColBox.Set(13.5, 13.5, 13.5);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->dynamicRendering = true;
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_WALL_PANEL);
	go->pos.Set(400, 120, 0);
	go->rotationY = -90;
	go->scale.Set(200, 200, 200);
	go->ColBox.Set(2, 120, 400);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_WALL_PANEL];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_FLOOR_TILE);
	go->pos.Set(0, 240, 0);
	go->rotationX = 90;
	go->scale.Set(400, 400, 400);
	go->ColBox.Set(400, 2, 400);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = false;
	go->obMesh = meshList[GEO_FLOOR_TILE];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_FLOOR_TILE);
	go->pos.Set(0, 0, 0);
	go->rotationX = -90;
	go->scale.Set(400, 400, 400);
	go->ColBox.Set(400, 2, 400);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_FLOOR_TILE];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_WALL_PANEL2);
	go->pos.Set(-400, 120, 280);
	go->rotationY = 90;
	go->scale.Set(200, 200, 200);
	go->ColBox.Set(2, 120, 120);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_WALL_PANEL2];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_WALL_PANEL2);
	go->pos.Set(-400, 120, 40);
	go->rotationY = 90;
	go->scale.Set(200, 200, 200);
	go->ColBox.Set(2, 120, 120);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_WALL_PANEL2];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_WALL_PANEL2);
	go->pos.Set(-400, 120, -280);
	go->rotationY = 90;
	go->scale.Set(200, 200, 200);
	go->ColBox.Set(2, 120, 120);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_WALL_PANEL2];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_WALL_PANEL2);
	go->pos.Set(280, 120, 400);
	go->rotationY = 180;
	go->scale.Set(200, 200, 200);
	go->ColBox.Set(120, 120, 2);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_WALL_PANEL2];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_WALL_PANEL2);
	go->pos.Set(-280, 120, 400);
	go->rotationY = 180;
	go->scale.Set(200, 200, 200);
	go->ColBox.Set(120, 120, 2);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_WALL_PANEL2];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_WALL_PANEL2);
	go->pos.Set(40, 120, 400);
	go->rotationY = 180;
	go->scale.Set(200, 200, 200);
	go->ColBox.Set(120, 120, 2);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_WALL_PANEL2];
	m_goList.push_back(go);
}

/******************************************************************************/
/*!
\brief
Generate collision boxes and positions of objects in room 2
*/
/******************************************************************************/
void mainscene::generateRoom2(void)
{
	GameObject *go;

	go = new GameObject(GameObject::GO_STORAGE_CUBE_OLD);
	go->pos.Set(0, 13.5f, 800);
	go->rotationY = 0;
	go->scale.Set(26.5f, 26.5f, 26.5f);
	go->ColBox.Set(13.5f, 13.5f, 13.5f);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->dynamicRendering = true;
	go->obMesh = meshList[GEO_OLDCUBE1];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GO_STORAGE_CUBE_OLD);
	go->pos.Set(15, 40.5f, 801);
	go->rotationY = 0;
	go->scale.Set(26.5f, 26.5f, 26.5f);
	go->ColBox.Set(13.5f, 13.5f, 13.5f);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->dynamicRendering = true;
	go->obMesh = meshList[GEO_OLDCUBE1];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GO_STORAGE_CUBE_OLD);
	go->pos.Set(28, 13.5f, 802);
	go->rotationY = 0;
	go->scale.Set(26.5f, 26.5f, 26.5f);
	go->ColBox.Set(13.5f, 13.5f, 13.5f);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_OLDCUBE1];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GO_STORAGE_CUBE_OLD);
	go->pos.Set(-500, 13.5, 190);
	go->rotationY = -5;
	go->scale.Set(26.5, 26.5, 26.5);
	go->ColBox.Set(13.5, 13.5, 13.5);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->dynamicRendering = true;
	go->obMesh = meshList[GEO_OLDCUBE1];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GO_STORAGE_CUBE_OLD);
	go->pos.Set(-1100, 13.5, -100);
	go->rotationY = 10;
	go->scale.Set(26.5, 26.5, 26.5);
	go->ColBox.Set(13.5, 13.5, 13.5);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->dynamicRendering = true;
	go->obMesh = meshList[GEO_OLDCUBE1];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GO_STORAGE_CUBE_OLD);
	go->pos.Set(-1100, 40, -101);
	go->rotationY = -1;
	go->scale.Set(26.5, 26.5, 26.5);
	go->ColBox.Set(13.5, 13.5, 13.5);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->dynamicRendering = true;
	go->obMesh = meshList[GEO_OLDCUBE1];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GO_STORAGE_CUBE_OLD);
	go->pos.Set(-1800, 13.5, 300);
	go->rotationY = -10;
	go->scale.Set(26.5, 26.5, 26.5);
	go->ColBox.Set(13.5, 13.5, 13.5);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->dynamicRendering = true;
	go->obMesh = meshList[GEO_OLDCUBE1];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GO_STORAGE_CUBE_OLD);
	go->pos.Set(-1800, 40, 300);
	go->rotationY = 10;
	go->scale.Set(26.5, 26.5, 26.5);
	go->ColBox.Set(13.5, 13.5, 13.5);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->dynamicRendering = true;
	go->obMesh = meshList[GEO_OLDCUBE1];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GO_STORAGE_CUBE_OLD);
	go->pos.Set(-1500, 13.5, 10);
	go->rotationY = -20;
	go->scale.Set(26.5, 26.5, 26.5);
	go->ColBox.Set(13.5, 13.5, 13.5);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->dynamicRendering = true;
	go->obMesh = meshList[GEO_OLDCUBE1];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GO_STORAGE_CUBE_OLD);
	go->pos.Set(-1600, 13.5, -300);
	go->rotationY = 90;
	go->scale.Set(26.5, 26.5, 26.5);
	go->ColBox.Set(13.5, 13.5, 13.5);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->dynamicRendering = true;
	go->obMesh = meshList[GEO_OLDCUBE1];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GO_STORAGE_CUBE_OLD);
	go->pos.Set(-900, 13.5, 150);
	go->rotationY = 95;
	go->scale.Set(26.5, 26.5, 26.5);
	go->ColBox.Set(13.5, 13.5, 13.5);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->dynamicRendering = true;
	go->obMesh = meshList[GEO_OLDCUBE1];
	m_goList.push_back(go);


	for(unsigned i = 0; i < 4; ++i)
	{
		go = new GameObject(GameObject::GOW_WALL_PANEL);
		if(i == 0)
		{
			go->pos.Set(-880, 120, 400);
			go->rotationY = 180;
		}

		else if(i == 1)
		{
			go->pos.Set(-1680, 120, 400);
			go->rotationY = 180;
		}

		else if(i == 2)
		{
			go->pos.Set(-880, 120, -400);
		}

		else if(i == 3)
		{
			go->pos.Set(-1680, 120, -400);
		}

		go->scale.Set(200, 200, 200);
		go->ColBox.Set(400, 120, 2);
		go->active = true;
		go->enablePhysics = false;
		go->colEnable = true;
		go->obMesh = meshList[GEO_WALL_PANEL];
		m_goList.push_back(go);
	}

	go = new GameObject(GameObject::GOW_FLOOR_TILE);
	go->pos.Set(-880, 240, 0);
	go->rotationX = 90;
	go->scale.Set(400, 400, 400);
	go->ColBox.Set(400, 2, 400);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_FLOOR_TILE];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_FLOOR_TILE);
	go->pos.Set(-1680, 240, 0);
	go->rotationX = 90;
	go->scale.Set(400, 400, 400);
	go->ColBox.Set(400, 2, 400);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_FLOOR_TILE];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_FLOOR_TILE);
	go->pos.Set(-880, 0, 0);
	go->rotationX = -90;
	go->scale.Set(400, 400, 400);
	go->ColBox.Set(400, 2, 400);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_FLOOR_TILE];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_FLOOR_TILE);
	go->pos.Set(-1680, 0, 0);
	go->rotationX = -90;
	go->scale.Set(400, 400, 400);
	go->ColBox.Set(400, 2, 400);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_FLOOR_TILE];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_WALL_PANEL2);
	go->pos.Set(-480, 120, 280);
	go->rotationY = -90;
	go->scale.Set(200, 200, 200);
	go->ColBox.Set(2, 120, 120);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_WALL_PANEL2];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_WALL_PANEL2);
	go->pos.Set(-480, 120, 40);
	go->rotationY = -90;
	go->scale.Set(200, 200, 200);
	go->ColBox.Set(2, 120, 120);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_WALL_PANEL2];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_WALL_PANEL2);
	go->pos.Set(-480, 120, -280);
	go->rotationY = -90;
	go->scale.Set(200, 200, 200);
	go->ColBox.Set(2, 120, 120);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_WALL_PANEL2];
	m_goList.push_back(go);
}

/******************************************************************************/
/*!
\brief
Generate collision boxes and positions of objects in the ruins area
*/
/******************************************************************************/
void mainscene::generateRuins(void)
{
	GameObject *go;
	for(unsigned i = 0; i < 10; ++i)
	{
		go = new GameObject(GameObject::GOW_WALL_RUSTED1);

		go->scale.Set(200, 200, 200);
		go->active = true;
		go->enablePhysics = false;
		go->colEnable = true;

		switch(i)
		{
		case 0:
			{
				go->pos.Set(0, 1080, 1440);
				go->ColBox.Set(400, 120, 2);
				break;
			}
		case 1:
			{
				go->pos.Set(0, 1080, 1920);
				go->rotationY = 180;
				go->ColBox.Set(400, 120, 2);
				break;
			}
		case 2:
			{
				go->pos.Set(0, 1320, 1440);
				go->colEnable = false;
				break;
			}
		case 3:
			{
				go->pos.Set(0, 1320, 1920);
				go->rotationY = 180;
				go->colEnable = false;
				break;
			}
		case 4:
			{
				go->pos.Set(400, 1360, 1800);
				go->rotationY = -90;
				go->colEnable = false;
				break;
			}
		case 5:
			{
				go->pos.Set(400, 1000, 1800);
				go->rotationY = -90;
				go->ColBox.Set(2, 120, 400);
				break;
			}
		case 6:
			{
				go->pos.Set(-400, 1320, 1800);
				go->rotationY = 90;
				go->colEnable = false;
				break;
			}
		case 7:
			{
				go->pos.Set(-400, 1080, 1800);
				go->rotationY = 90;
				go->ColBox.Set(2, 120, 400);
				break;
			}
		case 8:
			{
				go->pos.Set(0, 1440, 1800);
				go->rotationX = 90;
				go->colEnable = false;
				break;
			}
		case 9:
			{
				go->pos.Set(0, 1440, 1560);
				go->rotationX = 90;
				go->colEnable = false;
				break;
			}
		}

		go->obMesh = meshList[GEO_WALLRUSTED1];
		m_goList.push_back(go);
	}
}

/******************************************************************************/
/*!
\brief
Generate collision boxes and positions of objects in the exterior
*/
/******************************************************************************/
void mainscene::generateexterior(void)
{
	GameObject *go;

	go = new GameObject(GameObject::GOW_WALL_PANEL);
	go->pos.Set(400, 1360, 1800);
	go->rotationY = 90;
	go->scale.Set(200, 200, 200);
	go->ColBox.Set(2, 120, 400);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_WALL_PANEL];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_WALL_PANEL);
	go->pos.Set(400, 1360, 2320);
	go->rotationY = 90;
	go->scale.Set(200, 200, 200);
	go->ColBox.Set(2, 120, 400);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_WALL_PANEL];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_WALL_PANEL);
	go->pos.Set(400, 1360, 1040);
	go->rotationY = 90;
	go->scale.Set(200, 200, 200);
	go->ColBox.Set(2, 120, 400);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_WALL_PANEL];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_WALL_PANEL);
	go->pos.Set(400, 1120, 2320);
	go->rotationY = 90;
	go->scale.Set(200, 200, 200);
	go->ColBox.Set(2, 120, 400);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_WALL_PANEL];
	m_goList.push_back(go);

	go = new GameObject(GameObject::GOW_WALL_PANEL);
	go->pos.Set(400, 1120, 1040);
	go->rotationY = 90;
	go->scale.Set(200, 200, 200);
	go->ColBox.Set(2, 120, 400);
	go->active = true;
	go->enablePhysics = false;
	go->colEnable = true;
	go->obMesh = meshList[GEO_WALL_PANEL];
	m_goList.push_back(go);
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
	proj.SetToPerspective(newFOV, static_cast<double>(Application::GetWindowWidth())/static_cast<double>(Application::GetWindowHeight()), 0.1f, 10000.0f);
	projectionStack.LoadMatrix(proj);
}

/******************************************************************************/
/*!
\brief
Turret Animations
*/
/******************************************************************************/
void mainscene::UpdateTurret(double dt)
{
	float walkspeedanimation = 117.0f;
	float turnspeedanimation = 60.0f;
	float stepreturnspeedanimation = 200.0f;
	float walkspeed = 45.0f;
	float turnspeed = 60.0f;

	if(firing == true)
	{
		walkspeedanimation /= 2;
		walkspeed /= 2;
	}

	if(TURRET.rotateA > 180)
	{
		TURRET.rotateA -= 360;
	}

	if(TURRET.rotateA < -180)
	{
		TURRET.rotateA += 360;
	}


	if(moving == true || turnleft == true || turnright == true)//WALKING ANIMATION FOR TURRET
	{
		if(limitmove1 == false)
		{
			if(TLEG1.rotateA < 40)
			{
				if(forward == true && turnleft == false && turnright == false)
				{
					TLEG1.rotateA += static_cast<float>(walkspeedanimation * dt);
				}
				else if(forward == false && turnleft == false && turnright == false)
				{
					TLEG1.rotateA += static_cast<float>(stepreturnspeedanimation * dt);
				}
				else if(turnleft == true)
				{
					TLEG1.rotateA += static_cast<float>(stepreturnspeedanimation * dt);
				}
				else if(turnright == true)
				{
					TLEG1.rotateA += static_cast<float>(turnspeedanimation * dt);
				}
			}

			if(TLEG1.rotateA >= 40)
			{
				limitmove1 = true;
			}
		}
		if(limitmove1 == true)
		{
			if(TLEG1.rotateA > 0)
			{
				if(forward == true && turnleft == false && turnright == false)
				{
					TLEG1.rotateA -= static_cast<float>(stepreturnspeedanimation * dt);
				}
				else if (forward == false && turnleft == false && turnright == false)
				{
					TLEG1.rotateA -= static_cast<float>(walkspeedanimation * dt);
				}
				else if(turnleft == true)
				{
					TLEG1.rotateA -= static_cast<float>(turnspeedanimation * dt);
				}
				else if(turnright == true)
				{
					TLEG1.rotateA -= static_cast<float>(stepreturnspeedanimation * dt);
				}
			}
			if(TLEG1.rotateA <= 0)
			{
				limitmove1 = false;
			}
		}

		if(limitmove2 == false)
		{
			if(TLEG2.rotateA > -40)
			{
				if(forward == true && turnleft == false && turnright == false)
				{
					TLEG2.rotateA -= static_cast<float>(walkspeedanimation * dt);
				}
				else if(forward == false && turnleft == false && turnright == false)
				{
					TLEG2.rotateA -= static_cast<float>(stepreturnspeedanimation * dt);
				}
				else if(turnleft == true)
				{
					TLEG2.rotateA -= static_cast<float>(turnspeedanimation * dt);
				}
				else if(turnright == true)
				{
					TLEG2.rotateA -= static_cast<float>(stepreturnspeedanimation * dt);
				}

			}
			if(TLEG2.rotateA <= -40)
			{
				limitmove2 = true;
			}
		}
		if(limitmove2 == true)
		{
			if(TLEG2.rotateA < 0)
			{
				if(forward == true && turnleft == false && turnright == false)
				{
					TLEG2.rotateA += static_cast<float>(stepreturnspeedanimation * dt);
				}
				else if(forward == false && turnleft == false && turnright == false)
				{
					TLEG2.rotateA += static_cast<float>(walkspeedanimation * dt);
				}
				else if(turnleft == true)
				{
					TLEG2.rotateA += static_cast<float>(stepreturnspeedanimation * dt);
				}
				else if(turnright == true)
				{
					TLEG2.rotateA += static_cast<float>(turnspeedanimation * dt);
				}
			}
			if(TLEG2.rotateA >= 0)
			{
				limitmove2 = false;
			}
		}
		if(limitmove3 == false)
		{
			if(TLEG3.rotateA > -100)
			{
				TLEG3.rotateA -= static_cast<float>(20 * dt);
			}

			if(TLEG3.rotateA <= -100)
			{
				limitmove3 = true;
			}
		}
		if(limitmove3 == true)
		{
			if(TLEG3.rotateA < -98)
			{
				TLEG3.rotateA += static_cast<float>(20 * dt);
			}

			if(TLEG3.rotateA >= -98)
			{
				limitmove3 = false;
			}
		}
	}


	if(Application::IsKeyPressed('E'))//ATTACK MODE TOGGLE
	{
		Attackmode = true;
	}

	if(Application::IsKeyPressed('Q'))//ATTACK MODE TOGGLE
	{
		Attackmode = false;
	}

	if(Attackmode == true)//ANIMATION / TRANSITION TO ATTACK MODE
	{
		if(S1.translateZ > -10)
		{
			S1.translateZ -= static_cast<float>(20 * dt);
		}
		if(S2.translateZ < 10)
		{
			S2.translateZ += static_cast<float>(20 * dt);
		}
	}

	if(Attackmode == false)//ANIMATION / TRANSITION TO NORMAL MODE
	{
		if(S1.translateZ < 0)
		{
			S1.translateZ += static_cast<float>(20 * dt);
		}
		if(S2.translateZ > 0)
		{
			S2.translateZ -= static_cast<float>(20 * dt);
		}
	}

	if(Application::IsKeyPressed(VK_SPACE))
	{
		if(Attackmode == true)
		{
			firing = true;
		}
	}

	if(firing == true)
	{
		BULLETR.translateY -= static_cast<float>(2000 * dt);
		if(BULLETR.translateY < -2000.f)
		{
			BULLETR.translateY = 10.f;
			BULLETL.translateY = 10.f;
			firing = false;
		}

		if(BULLETR.translateY < -100.f)
		{
			BULLETL.translateY -= static_cast<float>(2000 * dt);
		}
	}
}

/******************************************************************************/
/*!
\brief
Animations for the panels and handles when to open/close
*/
/******************************************************************************/
void mainscene::UpdatePanel(double dt, Vector3 Pos, bool &panel, float &doorangle)
{
	if(panel == true)
	{
		if(doorangle < 0)
		{
			doorangle += static_cast<float>(40 * dt);

			if(!engine->isCurrentlyPlaying(soundList[ST_PANEL]))
			{
				engine->play3D(soundList[ST_PANEL], vec3df(Pos.x, Pos.y, Pos.z), false, false, true);
			}
		}
		if(doorangle > 0)
		{
			doorangle = 0;
		}
	}
	if(panel == false)
	{
		if(doorangle > -90)
		{
			doorangle -= static_cast<float>(40 * dt);

			if(!engine->isCurrentlyPlaying(soundList[ST_PANEL]))
			{
				engine->play3D(soundList[ST_PANEL], vec3df(Pos.x, Pos.y, Pos.z), false, false, true);
			}
		}
		if(doorangle < -90)
		{
			doorangle = -90.f;
		}
	}

	if(intersect(Pos + Vector3(240, 120, 240), Pos - Vector3(240, 120, 240), GOp_Player->pos))
	{
		panel = true;
	}

	else
	{
		panel = false;
	}
}

/******************************************************************************/
/*!
\brief
Code for elevator up and down
*/
/******************************************************************************/
void mainscene::UpdateElevator(double dt)
{
	if(ElevatorDown == true)
	{
		if(LIFT.translateY > -480)
		{
			LIFT.translateY -= static_cast<float>(60 * dt);
		}

		if(LIFT.translateY < -480)
		{
			LIFT.translateY = -480;
		}

		if(SWITCH.translateY < 0)
		{
			SWITCH.translateY += static_cast<float>(1 * dt);
		}
		if(SWITCH.translateY > 0)
		{
			SWITCH.translateY = 0;
		}
	}

	if(ElevatorDown == false)
	{
		if(LIFT.translateY < 1120)
		{
			LIFT.translateY += static_cast<float>(60 * dt);
		}

		if(LIFT.translateY > 1120)
		{
			LIFT.translateY = 1120;
		}

		if(SWITCH.translateY > -1)
		{
			SWITCH.translateY -= static_cast<float>(1 * dt);
		}

		if(SWITCH.translateY < -1)
		{
			SWITCH.translateY = -1;
		}
	}

	if(ElevatorDown2 == true)
	{
		if(LIFT2.translateY > -480)
		{
			LIFT2.translateY -= static_cast<float>(60 * dt);
		}

		if(LIFT2.translateY < -480)
		{
			LIFT2.translateY = -480;
		}
	}

	if(ElevatorDown2 == false)
	{
		if(LIFT2.translateY < 0)
		{
			LIFT2.translateY += static_cast<float>(60 * dt);
		}

		if(LIFT2.translateY > 0)
		{
			LIFT2.translateY = 0;
		}
	}
}

/******************************************************************************/
/*!
\brief
Handles player physics and movement
*/
/******************************************************************************/
void mainscene::UpdatePlayer(double dt)
{
	float walkSoundDelay = 0.7f;
	float ShakeMultiplier = 0.f;
	bool inAir = false;
	if(GOp_Player->active)
	{
		//Y axis collision handling
		if(!collide(Vector3(GOp_Player->pos - Vector3(0.f, 42.f, 0.f))))
		{
			if(collide(Vector3(GOp_Player->pos + Vector3(0.f, 8.f, 0.f))))
			{
				GOp_Player->vel.y = -10;
			}
			else if(collide(Vector3(GOp_Player->pos + Vector3(0.f, 5.f, 0.f))))
			{
				GOp_Player->vel.y = 0;
			}

			GOp_Player->vel += gravity_force * static_cast<float>(dt);
			inAir = true;
		}
		else
		{
			if(collide(Vector3(GOp_Player->pos - Vector3(0.f, 36.f, 0.f))))//This is to prevent floor clipping, or rather, to make it bounce back up if it's clipping
			{
				GOp_Player->vel.y = 100;
			}

			else if(collide(Vector3(GOp_Player->pos - Vector3(0.f, 38.f, 0.f))))
			{
				GOp_Player->vel.y = 50;
			}

			else if(collide(Vector3(GOp_Player->pos - Vector3(0.f, 40.f, 0.f))))
			{
				GOp_Player->vel.y = 10;
			}

			else
			{
				if(GOp_Player->vel.y < -100)
				{
					engine->play2D(soundList[ST_STEP_2]);
				}
				GOp_Player->vel.y = 0;
			}
		}

		//PLAYER MOVEMENT
		Vector3 LookDir = FPC.target - FPC.position;
		LookDir.y = 0.f;
		Vector3 RightDir = LookDir.Cross(Vector3(0, 1, 0));
		LookDir.Normalize();
		RightDir.Normalize();

		if(Application::IsKeyPressed(us_control[E_CTRL_MOVE_SPRINT]))
		{
			LookDir *= 18;
			RightDir *= 12;
			ShakeMultiplier = 1.6f;
			walkSoundDelay /= 2;
		}
		else if(Application::IsKeyPressed(us_control[E_CTRL_MOVE_WALK]))
		{
			LookDir *= 4.5f;
			RightDir *= 4.5f;
			walkSoundDelay *= 2;
		}
		else
		{
			LookDir *= 9;
			RightDir *= 9;
			ShakeMultiplier = 0.4f;
		}

		//Player movement
		if(Application::IsKeyPressed(us_control[E_CTRL_MOVE_FRONT]) && !Application::IsKeyPressed(us_control[E_CTRL_MOVE_BACK]))
		{
			GOp_Player->vel.x += LookDir.x;
			GOp_Player->vel.z += LookDir.z;

			if(walkSoundDelay + f_step < timer && !inAir)
			{
				engine->play2D(soundList[ST_STEP]);
				f_step = timer;
			}
		}

		if(Application::IsKeyPressed(us_control[E_CTRL_MOVE_BACK]) && !Application::IsKeyPressed(us_control[E_CTRL_MOVE_FRONT]))
		{
			GOp_Player->vel.x -= LookDir.x;
			GOp_Player->vel.z -= LookDir.z;

			if(walkSoundDelay + f_step < timer && !inAir)
			{
				engine->play2D(soundList[ST_STEP]);
				f_step = timer;
			}
		}

		if(Application::IsKeyPressed(us_control[E_CTRL_MOVE_LEFT]) && !Application::IsKeyPressed(us_control[E_CTRL_MOVE_RIGHT]))
		{
			GOp_Player->vel -= RightDir;

			if(walkSoundDelay + f_step < timer && !inAir)
			{
				engine->play2D(soundList[ST_STEP]);
				f_step = timer;
			}
		}

		if(Application::IsKeyPressed(us_control[E_CTRL_MOVE_RIGHT]) && !Application::IsKeyPressed(us_control[E_CTRL_MOVE_LEFT]))
		{
			GOp_Player->vel += RightDir;

			if(walkSoundDelay + f_step < timer && !inAir)
			{
				engine->play2D(soundList[ST_STEP]);
				f_step = timer;
			}
		}

		if(Application::IsKeyPressed(us_control[E_CTRL_MOVE_JUMP]))
		{
			if(inAir == false)
			{
				GOp_Player->vel.y += 120;
				engine->play2D(soundList[ST_STEP]);
			}
		}

		//smooth slowing down
		if(GOp_Player->vel.x != 0)
		{
			float SForceX = 0 - GOp_Player->vel.x;
			GOp_Player->vel.x += SForceX * 0.1f;
		}

		if(GOp_Player->vel.z != 0)
		{
			float SForceZ = 0 - GOp_Player->vel.z;
			GOp_Player->vel.z += SForceZ * 0.1f;
		}

		if(Application::IsKeyPressed(us_control[E_CTRL_MOVE_LEFT]) || Application::IsKeyPressed(us_control[E_CTRL_MOVE_RIGHT]) || Application::IsKeyPressed(us_control[E_CTRL_MOVE_FRONT]) || Application::IsKeyPressed(us_control[E_CTRL_MOVE_BACK]))
		{
			if(ShakeState)
			{
				V3_CamShaker.y += 0.03f * static_cast<float>(dt) * ShakeMultiplier;
				V3_CamShaker.x += 0.03f * static_cast<float>(dt) * ShakeMultiplier;
				V3_CamShaker.z += 0.03f * static_cast<float>(dt) * ShakeMultiplier;

				if(V3_CamShaker.y >= (0.006f))
				{
					ShakeState = false;
				}
			}
			else
			{
				V3_CamShaker.y -= 0.03f * static_cast<float>(dt) * ShakeMultiplier;
				V3_CamShaker.x -= 0.03f * static_cast<float>(dt) * ShakeMultiplier;
				V3_CamShaker.z -= 0.03f * static_cast<float>(dt) * ShakeMultiplier;

				if(V3_CamShaker.y <= (-0.006f))
				{
					ShakeState = true;
				}
			}
		}
		else
		{
			if(V3_CamShaker.x != 0)
			{
				float SForceX = 0 - V3_CamShaker.x;
				V3_CamShaker.x += SForceX * 0.1f;
			}

			if(V3_CamShaker.y != 0)
			{
				float SForceY = 0 - V3_CamShaker.y;
				V3_CamShaker.y += SForceY * 0.1f;
			}

			if(V3_CamShaker.z != 0)
			{
				float SForceZ = 0 - V3_CamShaker.z;
				V3_CamShaker.z += SForceZ * 0.1f;
			}
		}

		//Collision handling
		if(collide(Vector3(GOp_Player->pos + Vector3(10.f, -30.f, 0.f))) || collide(Vector3(GOp_Player->pos + Vector3(10.f, 5.f, 0.f))))
		{
			if(GOp_Player->vel.x > 0)
			{
				GOp_Player->vel.x = 0;
			}
		}

		if(collide(Vector3(GOp_Player->pos + Vector3(0.f, -30.f, 10.f))) || collide(Vector3(GOp_Player->pos + Vector3(0.f, 5.f, 10.f))))
		{
			if(GOp_Player->vel.z > 0)
			{
				GOp_Player->vel.z = 0;
			}
		}

		if(collide(Vector3(GOp_Player->pos + Vector3(-10.f, -30.f, 0.f))) || collide(Vector3(GOp_Player->pos + Vector3(-10.f, 5.f, 0.f))))
		{
			if(GOp_Player->vel.x < 0)
			{
				GOp_Player->vel.x = 0;
			}
		}

		if(collide(Vector3(GOp_Player->pos + Vector3(0.f, -30.f, -10.f))) || collide(Vector3(GOp_Player->pos + Vector3(0.f, 5.f, -10.f))))
		{
			if(GOp_Player->vel.z < 0)
			{
				GOp_Player->vel.z = 0;
			}
		}

		GOp_Player->pos += GOp_Player->vel * static_cast<float>(dt);
		FPC = FPC + (GOp_Player->vel * static_cast<float>(dt));
	}
}

/******************************************************************************/
/*!
\brief
Handles game object physics
*/
/******************************************************************************/
void mainscene::UpdateGO(double dt)
{
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active && go->enablePhysics)
		{
			{
				go->vel += gravity_force * static_cast<float>(dt);
			}
			go->pos += go->vel * static_cast<float>(dt);
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
void mainscene::UpdateParticles(double dt)
{
	for(std::vector<Particle *>::iterator it = m_ParList.begin(); it != m_ParList.end(); ++it)
	{
		Particle *Par = (Particle *)*it;
		if(Par->active)
		{
			if(Par->lifetime < 0)
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
void mainscene::UpdateBullets(double dt)
{
	for(std::vector<BulletInfo *>::iterator it = BIv_BulletList.begin(); it != BIv_BulletList.end(); ++it)
	{
		BulletInfo *BI = (BulletInfo *)*it;
		if(BI->getStatus())
		{
			if(collide(BI->getPosition(), true))
			{
				BI->verticalvelocity = 0.f;
				BI->setStatus(false);
				for(unsigned i = 0; i < 5; ++i)
				{
					generateParticle(BI->getPosition(), Vector3(0.5, 0.5, 0.5), Vector3(Math::RandFloatMinMax(-70, 70), Math::RandFloatMinMax(-5, 70), Math::RandFloatMinMax(-70, 70))+BI->getDirection()*-20, Particle::PAR_DEFAULT, 1.0f);
				}
			}
			else
			{
				BI->Update(dt * 0.8);
			}
		}
	}
}

/******************************************************************************/
/*!
\brief
Updates the target boards
*/
/******************************************************************************/
void mainscene::UpdateTargetBoard(double dt)
{
	for(std::vector<targetBoard *>::iterator it = targetBList.begin(); it != targetBList.end(); ++it)
	{
		targetBoard *TB = (targetBoard *)*it;
		if(TB->active)
		{
			TB->life -= static_cast<float>(dt);

			if(TB->rotation > 0)
			{
				TB->rotation -= 90.f * static_cast<float>(dt);
			}

			if(TB->health < 0 || TB->life < 0)
			{
				--targetsActive;

				if(TB->health < 0)
				{
					engine->play2D(soundList[ST_KILL]);
					++targetsHit;
				}

				TB->active = false;
			}
		}
		else
		{
			if(TB->rotation < 80)
			{
				TB->rotation += 90.f * static_cast<float>(dt);
			}
		}
	}
}

/******************************************************************************/
/*!
\brief
Activates a random target board
*/
/******************************************************************************/
void mainscene::ActivateTargetBoard(void)
{
	int targetNum = 0;
	int TargetNum = Math::RandIntMinMax(0, targetBList.size());
	for(std::vector<targetBoard *>::iterator it = targetBList.begin(); it != targetBList.end(); ++it)
	{
		targetBoard *TB = (targetBoard *)*it;
		if(!TB->active)
		{
			if(targetNum == TargetNum)
			{
				TB->health = 100;
				TB->life = 5.f;
				TB->active = true;
				++targetsActive;
				break;
			}
			++targetNum;
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
	for(std::vector<BulletInfo *>::iterator it = BIv_BulletList.begin(); it != BIv_BulletList.end(); ++it)
	{
		BulletInfo *BI = (BulletInfo *)*it;
		if(!BI->getStatus())
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
void mainscene::weaponsUpdate(double dt)
{
	if(f_curRecoil > 0)
	{
		if(AimDownSight)
		{
			f_curRecoil -= static_cast<float>(dt) * f_curRecoil * 4.f;
		}
		else
		{
			f_curRecoil -= static_cast<float>(dt) * f_curRecoil * 2.f;
		}
	}
	//SHOOTING
	if(Application::IsKeyPressed(us_control[E_CTRL_ATTACK]))
	{
		if(timer - fireRate > fireRateMax && weaponList[currentWeapon].CurrentClip > 0)
		{
			if(weaponList[currentWeapon].numBullet == 1)
			{
				Vector3 ShootVector = Vector3(Math::RandFloatMinMax(-f_curRecoil*0.01f, f_curRecoil*0.01f), Math::RandFloatMinMax(-f_curRecoil*0.01f, f_curRecoil*0.01f), Math::RandFloatMinMax(-f_curRecoil*0.01f, f_curRecoil*0.01f)) + FPC.target - FPC.position;
				FPC.rotateCamVertical(static_cast<float>(dt) * weaponList[currentWeapon].recoilEffect);
				Shoot(GOp_Player->pos, ShootVector.Normalize(), weaponList[currentWeapon].bulletvelocity * 5.f, 6, weaponList[currentWeapon].damage);
			}
			else
			{
				for(int i = 0; i < weaponList[currentWeapon].numBullet; ++i)
				{
					f_curRecoil = weaponList[currentWeapon].bulletSpread * 0.5f;
					Vector3 ShootVector = Vector3(Math::RandFloatMinMax(-f_curRecoil*0.01f, f_curRecoil*0.01f), Math::RandFloatMinMax(-f_curRecoil*0.01f, f_curRecoil*0.01f), Math::RandFloatMinMax(-f_curRecoil*0.01f, f_curRecoil*0.01f)) + FPC.target - FPC.position;
					Shoot(GOp_Player->pos, ShootVector.Normalize(), weaponList[currentWeapon].bulletvelocity * 5.f, 6, weaponList[currentWeapon].damage);
				}
				FPC.rotateCamVertical(static_cast<float>(dt) * weaponList[currentWeapon].recoilEffect);
			}
			if(!AimDownSight)
			{
				f_curRecoil += static_cast<float>(dt) * weaponList[currentWeapon].bulletSpread;//Cursor recoil effect and bullet spread effect
			}
			else
			{
				f_curRecoil += static_cast<float>(dt) * weaponList[currentWeapon].bulletSpread * 0.5f;//Aiming down sights reduce spread
			}
			engine->play2D(weaponList[currentWeapon].shootSound);

			weaponList[currentWeapon].currentpos.z -= static_cast<float>(dt) * weaponList[currentWeapon].recoilEffect;
			fireRate = timer;
			--weaponList[currentWeapon].CurrentClip;
		}
	}
	//RELOADING
	if(Application::IsKeyPressed(us_control[E_CTRL_RELOAD]))
	{
		weaponList[currentWeapon].CurrentClip = 0;
	}
	if(weaponList[currentWeapon].CurrentClip <= 0)
	{
		if(reloadTimer <= 0)
		{
			engine->play2D(weaponList[currentWeapon].reloadSound);
		}

		reloadTimer += static_cast<float>(dt);

		if(AimDownSight)
		{
			AimDownSight = false;
		}

		if(weaponList[currentWeapon].currentpos.y > weaponList[currentWeapon].heldpos.y - 8.f)
		{
			float f_temp1 = (weaponList[currentWeapon].heldpos.y - 8.f) - weaponList[currentWeapon].currentpos.y;
			weaponList[currentWeapon].currentpos.y += f_temp1 * static_cast<float>(dt) * 10.f;
		}

		if(reloadTimer > weaponList[currentWeapon].reloadTime)
		{
			reloadTimer = 0.f;
			weaponList[currentWeapon].CurrentClip = weaponList[currentWeapon].ClipSize;
		}
	}

	//AIM DOWN SIGHT
	if(Application::IsKeyPressed(us_control[E_CTRL_AIM]))
	{
		if(timer - inputDelay > 0.25)
		{
			if(!AimDownSight)
			{
				AimDownSight = true;
			}
			else
			{
				AimDownSight = false;
			}
			inputDelay = timer;
		}
	}

	//SWITCH WEAPONS
	if(Application::IsKeyPressed(us_control[E_CTRL_NEXT_ITEM]))
	{
		if(timer - inputDelay > 0.25)
		{
			if(currentWeapon < WT_TOTAL - 1)
			{
				currentWeapon += 1;
			}
			else
			{
				currentWeapon = 0;
			}

			weaponList[currentWeapon].currentpos = weaponList[currentWeapon].heldpos - Vector3(0, 5, 0);
			AimDownSight = false;
			fireRateMax = 60.f/static_cast<float>(weaponList[currentWeapon].fireRate);
			inputDelay = timer;
		}
	}

	if(AimDownSight)//Handle aim down sights
	{
		if(f_fov/weaponList[currentWeapon].adsZoom < f_currentfov)
		{
			f_currentfov -= static_cast<float>(dt) * (f_currentfov - f_fov/weaponList[currentWeapon].adsZoom) * 10;
			editFOV(f_currentfov);
		}
		if(weaponList[currentWeapon].currentpos != weaponList[currentWeapon].adspos)
		{
			Vector3 v3_temp1 = weaponList[currentWeapon].adspos - weaponList[currentWeapon].currentpos;
			weaponList[currentWeapon].currentpos += v3_temp1 * static_cast<float>(dt) * 12.f;
		}
		if(currentWeapon == WT_L11A3)
		{
			scopeExeTime += static_cast<float>(dt);
			if(scopeExeTime > 0.25)
			{
				if(renderScope)
				{
					renderScope = false;
				}
				if(ScopeAnim > 0)
				{
					ScopeAnim -= static_cast<float>(dt) * 10.f * ScopeAnim;
				}
				else if(ScopeAnim < 0)
				{
					ScopeAnim = 0.f;
				}
			}
		}
		if(FPC.mouseSensitivity == f_mouseSensitivity)
		{
			FPC.mouseSensitivity = f_mouseSensitivity/weaponList[currentWeapon].adsZoom; 
		}
	}
	else
	{
		if(!renderScope)
		{
			scopeExeTime = 0.f;
			ScopeAnim = 15.f;
			renderScope = true;
		}

		if(f_fov > f_currentfov)
		{
			f_currentfov += static_cast<float>(dt) * (f_fov - f_currentfov) * 10;
			editFOV(f_currentfov);
		}
		if(weaponList[currentWeapon].currentpos != weaponList[currentWeapon].heldpos)
		{
			Vector3 v3_temp1 = weaponList[currentWeapon].heldpos - weaponList[currentWeapon].currentpos;
			weaponList[currentWeapon].currentpos += v3_temp1 * static_cast<float>(dt) * 10.f;
		}
		if(FPC.mouseSensitivity != f_mouseSensitivity)
		{
			FPC.mouseSensitivity = f_mouseSensitivity; 
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
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active && go->colEnable && go->pos != Position)
		{
			if(intersect(go->pos + go->ColBox, go->pos - go->ColBox, Position))
			{
				return true;
			}
		}
	}

	//collision with terrainmap
	if(monalisa.ColEnable)
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
	}

	//Targetboard collision
	for(std::vector<targetBoard *>::iterator it = targetBList.begin(); it != targetBList.end(); ++it)
	{
		targetBoard *TB = (targetBoard *)*it;
		if(TB->active)
		{
			if(intersect(TB->position + TB->collisionBox, TB->position - TB->collisionBox, Position))
			{
				if(bullet)
				{
					TB->health -= weaponList[currentWeapon].damage;
				}
				return true;
			}
		}
	}

	if(intersect(Vector3(402, 1125, 3000), Vector3(398, -2000, -600), Position))//Huge wall left
	{
		return true;
	}

	else if(intersect(Vector3(-398, 1120, 3000), Vector3(-502, -2000, 400), Position))//Huge wall right
	{
		return true;
	}

	else if(intersect(Vector3(-155, 1000, 485), Vector3(-400, -500, 395), Position))//Wall on right of door
	{
		return true;
	}

	else if(intersect(Vector3(400, 1000, 485), Vector3(-85, -500, 395), Position))//Wall on left of door
	{
		return true;
	}

	else if(intersect(Vector3(-80, 1000, 485), Vector3(-160, 75, 395), Position))//Wall on top of door
	{
		return true;
	}

	else if(intersect(Vector3(-80, 0, 485), Vector3(-160, -500, 395), Position))//Wall below of door
	{
		return true;
	}

	else if(intersect(Vector3(-395, 800, 405), Vector3(-485, 75, -405), Position))//wall on top of door 2
	{
		return true;
	}

	else if(intersect(Vector3(-395, 400, 0), Vector3(-485, 0, -80), Position))//wall on right of door 2
	{
		return true;
	}

	else if(intersect(Vector3(-395, 400, -160), Vector3(-485, 0, -405), Position))//wall on left of door 2
	{
		return true;
	}

	else if(intersect(Vector3(-395, 2, 405), Vector3(-500, -2, -405), Position))//wall below of door 2
	{
		return true;
	}

	else if(intersect(Vector3(400, 240, -395), Vector3(-400, -5, -410), Position))//Observation room
	{
		return true;
	}

	else if(intersect(Vector3(105, 5, 885), Vector3(-305, -405, 400), Position))//ruins area platform
	{
		return true;
	}

	else if(intersect(Vector3(400, 960, 2890), Vector3(-400, -480, 2875), Position))//ruins area Aperture wall
	{
		return true;
	}

	else if(intersect(Vector3(400, -475, 2890), Vector3(-400, -490, 400), Position))//ruins area floor
	{
		return true;
	}

	else if(intersect(Vector3(295, 1440, 1620), Vector3(100, -480, 1580), Position))//Elevator shaft
	{
		return true;
	}

	else if(intersect(Vector3(295, 1440, 1780), Vector3(100, -480, 1740), Position))//Elevator shaft
	{
		return true;
	}

	else if(intersect(Vector3(140, 1440, 1780), Vector3(100, -480, 1580), Position))//Elevator shaft
	{
		return true;
	}

	else if(intersect(Vector3(300, 1125, 1780), Vector3(255, -400, 1580), Position))//Elevator shaft
	{
		return true;
	}

	//ELEVATOR2
	else if(intersect(Vector3(95, 0, 920), Vector3(-100, -400, 880), Position))//Elevator shaft
	{
		return true;
	}

	else if(intersect(Vector3(95, 1440, 1080), Vector3(-100, -480, 1040), Position))//Elevator shaft
	{
		return true;
	}

	else if(intersect(Vector3(-40, 1440, 1080), Vector3(-100, -480, 880), Position))//Elevator shaft
	{
		return true;
	}

	else if(intersect(Vector3(100, 1125, 1080), Vector3(55, -480, 880), Position))//Elevator shaft
	{
		return true;
	}
	//-----------------------------------------------------------------------------------------
	else if(intersect(Vector3(LIFT.translateX + 60, LIFT.translateY, LIFT.translateZ + 60), Vector3(LIFT.translateX - 60, LIFT.translateY - 40, LIFT.translateZ - 60), Position))
	{
		return true;
	}

	else if(intersect(Vector3(LIFT2.translateX + 60, LIFT2.translateY, LIFT2.translateZ + 60), Vector3(LIFT2.translateX - 60, LIFT2.translateY - 40, LIFT2.translateZ - 60), Position))
	{
		return true;
	}

	else if(intersect(Vector3(102, 50, 885), Vector3(90, 0, 480), Position))//Railings
	{
		return true;
	}

	else if(intersect(Vector3(-285, 50, 885), Vector3(-300, 0, 480), Position))//Railings
	{
		return true;
	}

	else if(intersect(Vector3(400, 1125, 1725), Vector3(315, 1035, 1630), Position))//lift top platform
	{
		return true;
	}

	else if(intersect(Vector3(SWITCH.translateX + 7, 38, SWITCH.translateZ + 7), Vector3(SWITCH.translateX - 7, 0, SWITCH.translateZ - 7), Position))//Button
	{
		return true;
	}

	else if(intersect(Vector3(SWITCH2.translateX + 7, 38, SWITCH2.translateZ + 7), Vector3(SWITCH2.translateX - 7, 0, SWITCH2.translateZ - 7), Position))//Button
	{
		return true;
	}

	else if(intersect(Vector3(SWITCH3.translateX + 7, 38, SWITCH3.translateZ + 7), Vector3(SWITCH3.translateX - 7, 0, SWITCH3.translateZ - 7), Position))//Button
	{
		return true;
	}

	else if(intersect(Vector3(-2076, 240, 410), Vector3(-2090, -10, -410), Position))//Room2 viewroom collision
	{
		return true;
	}
	//Shooting range barrier
	else if(intersect(Vector3(-648, 20, 400), Vector3(-654, 0, -400), Position))
	{
		return true;
	}
	//Panelcollisions
	else if(intersect(Vector3(PanelP1.translateX + 20, -PanelP1.rotateA, PanelP1.translateZ + 20), Vector3(PanelP1.translateX - 20, 0, PanelP1.translateZ - 20), Position))
	{
		return true;
	}

	else if(intersect(Vector3(PanelP2.translateX + 20, -PanelP2.rotateA, PanelP2.translateZ + 20), Vector3(PanelP2.translateX - 20, 0, PanelP2.translateZ - 20), Position))
	{
		return true;
	}

	return false;
}

/******************************************************************************/
/*!
\brief
Reset the shooting range
*/
/******************************************************************************/
void mainscene::resetShootingRange(void)
{
	targetSpawnTimer = 0.f;
	targetSpawnrate = 1.f;
	targetsActive = 0;
	targetLimit = 3;
	shootingRangeDelay = 5.f;
	ShootingRangeTimer = 120.f;
	showScoreTimer = 0.f;

	for(std::vector<targetBoard *>::iterator it = targetBList.begin(); it != targetBList.end(); ++it)
	{
		targetBoard *TB = (targetBoard *)*it;
		if(TB->active)
		{
			TB->active = false;
		}
	}
}

/******************************************************************************/
/*!
\brief
update player sound position
*/
/******************************************************************************/
void mainscene::UpdateSound(double dt)
{
	engine->setListenerPosition(vec3df(GOp_Player->pos.x, GOp_Player->pos.y, GOp_Player->pos.z), vec3df(-(FPC.target.x - FPC.position.x), FPC.target.y - FPC.position.y, -(FPC.target.z - FPC.position.z)).normalize(), vec3df(0,0,0), vec3df(FPC.up.x, FPC.up.y, FPC.up.z));
}

/******************************************************************************/
/*!
\brief
Animations, controls
*/
/******************************************************************************/
void mainscene::Update(double dt)
{
	timer += static_cast<float>(dt);
	float LSPEED2 = 100.0f;
	
	FPScounter = static_cast<float>(1/dt);

	if(Application::IsKeyPressed('1'))
	{
		if(!Application::IsKeyPressed(VK_SHIFT))
		{
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glEnable(GL_CULL_FACE);
		}
	}

	if(Application::IsKeyPressed('2'))
	{
		if(!Application::IsKeyPressed(VK_SHIFT))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if(Application::IsKeyPressed('3'))
	{
		if(!Application::IsKeyPressed(VK_SHIFT))
		{
			renderAxis = true;
		}
		else
		{
			renderAxis = false; 
		}
	}

	if(Application::IsKeyPressed('4'))
	{
		if(!Application::IsKeyPressed(VK_SHIFT))
		{
			DisplayInfo = false;
		}
		else
		{
			DisplayInfo = true;
		}
	}

	if(LightSwitch == false)
	{
		delayf += static_cast<float>(Math::RandFloatMinMax(-4, 6) * dt);
		if(delayf > 1)
		{
			lights[3].power = 1.f;
			glUniform1f(m_parameters[U_LIGHT3_POWER], lights[3].power);
			LightSwitch = true;
		}
	}

	if(LightSwitch == true)
	{
		delayf -= static_cast<float>(Math::RandFloatMinMax(-3, 8) * dt);
		if(delayf < 0)
		{
			lights[3].power = 0.f;
			glUniform1f(m_parameters[U_LIGHT3_POWER], lights[3].power);
			LightSwitch = false;
		}
	}

	UpdatePlayer(dt);
	UpdateGO(dt);
	UpdateParticles(dt);
	FPC.Update(dt);

	if(Application::IsKeyPressed(us_control[E_CTRL_INTERACT]))
	{
		if(intersect(Vector3(LIFT.translateX + 100, LIFT.translateY + 100, LIFT.translateZ + 100), Vector3(LIFT.translateX - 100, LIFT.translateY - 100, LIFT.translateZ - 100), GOp_Player->pos))
		{
			if(0.5 + liftDelay < timer)
			{
				if(!ElevatorDown)
				{
					ElevatorDown = true;
					liftDelay = timer;
				}
				else
				{
					ElevatorDown = false; 
					liftDelay = timer;
				}
			}
		}

		if(intersect(Vector3(LIFT2.translateX + 100, LIFT2.translateY + 100, LIFT2.translateZ + 100), Vector3(LIFT2.translateX - 100, LIFT2.translateY - 100, LIFT2.translateZ - 100), GOp_Player->pos))
		{
			if(0.5 + liftDelay < timer)
			{
				if(!ElevatorDown2)
				{
					ElevatorDown2 = true;
					liftDelay = timer;
				}
				else
				{
					ElevatorDown2 = false; 
					liftDelay = timer;
				}
			}
		}

		if(intersect(Vector3(SWITCH.translateX + 50, 50, SWITCH.translateZ + 50), Vector3(SWITCH.translateX - 50, 0, SWITCH.translateZ - 50), GOp_Player->pos))
		{
			if(SWITCH.translateY == -1)
			{
				ElevatorDown = true;
			}

			else if(SWITCH.translateY == 0)
			{
				ElevatorDown = false; 
			}
		}

		if(intersect(Vector3(SWITCH3.translateX + 50, 50, SWITCH3.translateZ + 50), Vector3(SWITCH3.translateX - 50, 0, SWITCH3.translateZ - 50), GOp_Player->pos))
		{
			if(SWITCH3.translateY == 0)
			{
				CubeDispense = true; 
			}
			else if(SWITCH3.translateY == -1)
			{
				CubeDispense = false; 
			}
		}

		if(intersect(Vector3(SWITCH2.translateX + 50, 50, SWITCH2.translateZ + 50), Vector3(SWITCH2.translateX - 50, 0, SWITCH2.translateZ - 50), GOp_Player->pos))
		{
			if(!ShootingRangeMode)
			{
				targetsHit = 0;
				resetShootingRange();
				engine->play2D(soundList[ST_ALERT]);
				weaponsEnabled = true;
				ShootingRangeMode = true;
			}
		}
	}

	if(Application::IsKeyPressed(VK_F1))
	{
		if(!Application::IsKeyPressed(VK_SHIFT))
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

	if(CubeDispense == true)
	{
		if(SWITCH3.translateY > -1)
		{
			SWITCH3.translateY -= static_cast<float>(1 * dt);
		}

		else if(SWITCH3.translateY < -1)
		{
			SWITCH3.translateY = -1;
		}

		generateParticle(Vector3(350, 400, 350), Vector3(3.f, 3.f, 3.f), Vector3(Math::RandFloatMinMax(-15, 15), Math::RandFloatMinMax(-15, 15), Math::RandFloatMinMax(-15, 15)), Particle::PAR_GEL, 2);
	}
	else
	{
		if(SWITCH3.translateY < 0)
		{
			SWITCH3.translateY += static_cast<float>(1 * dt);
		}

		else if(SWITCH3.translateY > 0)
		{
			SWITCH3.translateY = 0;
		}
	}

	UpdatePanel(dt, Vector3(PanelP1.translateX, PanelP1.translateY, PanelP1.translateZ), PanelOpen, PanelP1.rotateA);
	UpdatePanel(dt, Vector3(PanelP2.translateX, PanelP2.translateY, PanelP2.translateZ), PanelOpen2, PanelP2.rotateA);
	UpdateElevator(dt);

	if(weaponsEnabled)
	{
		UpdateBullets(dt);
		weaponsUpdate(dt);
	}

	UpdateTargetBoard(dt);
	if(ShootingRangeMode)
	{
		if(shootingRangeDelay <= 0)
		{
			ShootingRangeTimer -= static_cast<float>(dt);
			if(targetSpawnrate+targetSpawnTimer < timer)
			{
				if(targetsActive < targetLimit)
				{
					ActivateTargetBoard();
					targetSpawnTimer = timer;
				}
			}

			if(SWITCH2.translateY > -1)
			{
				SWITCH2.translateY -= static_cast<float>(1 * dt);
			}

			if(SWITCH2.translateY < -1)
			{
				SWITCH2.translateY = -1;
			}

			if(ShootingRangeTimer < 0)
			{
				ShootingRangeMode = false;
				engine->play2D(soundList[ST_BUZZER]);
				showScoreTimer = 10.f;
			}

			else if(ShootingRangeTimer < 30)
			{
				targetSpawnrate = 0.8f;
				targetLimit = 6;
			}
		}
		else
		{
			shootingRangeDelay -= static_cast<float>(dt);
		}

		if(GOp_Player->pos.x < - 650 || GOp_Player->pos.x > -400)
		{
			targetsHit = 0;
			engine->play2D(soundList[ST_BUZZER]);
			ShootingRangeMode = false;
			weaponsEnabled = false;
			resetShootingRange();
		}
	}
	else
	{
		if(showScoreTimer > 0)
		{
			showScoreTimer -= static_cast<float>(dt);
		}
		if(SWITCH2.translateY < 0)
		{
			SWITCH2.translateY = static_cast<float>(1 * dt);
		}

		if(SWITCH2.translateY > 0)
		{
			SWITCH2.translateY = 0;
		}
	}

	UpdateSound(dt);

	if(GOp_Player->pos.y > 1000)
	{
		SA_waterSprite->Update(dt);
	}

	CamRotationYaw = CalAnglefromPosition(FPC.target, FPC.position, true);
	CamRotationPitch = CalAnglefromPosition(FPC.target, FPC.position, false);
}

/******************************************************************************/
/*!
\brief
check if position is in view of the camera
*/
/******************************************************************************/
bool mainscene::isVisible(Vector3 &ObjPosition)
{
	if((ObjPosition - GOp_Player->pos).Length() < 1900)
	{
		float lookingOBJ = CalAnglefromPosition(ObjPosition, GOp_Player->pos, true);
		float cameraRotation = CamRotationYaw;
		float cameraRotationUp = CamRotationPitch;
		float viewM = 5.f;
		bool LO = false, CR = false;

		if(lookingOBJ < 0.f)
		{
			lookingOBJ += 360;
			LO = true;
		}

		if(cameraRotation < 0.f)
		{
			cameraRotation += 360;
			CR = true;
		}

		if(cameraRotationUp > 30.f || cameraRotationUp < -30.f)
		{
			viewM = 30.f;
		}

		if(cameraRotationUp > 70.f || cameraRotationUp < -70.f)
		{
			viewM = 60.f;
		}

		if(cameraRotation < 0.f)
		{
			cameraRotation = -cameraRotation;
		}

		if((lookingOBJ + f_fov + viewM > cameraRotation && lookingOBJ - f_fov - viewM < cameraRotation) || (Vector3(ObjPosition.x - GOp_Player->pos.x, 0, ObjPosition.z - GOp_Player->pos.z)).Length() < 40)
		{
			return true;
		}
		else
		{
			if(CR)
			{
				cameraRotation -= 360;
			}
			if(LO)
			{
				lookingOBJ -= 360;
			}
			if((lookingOBJ + f_fov + viewM > cameraRotation && lookingOBJ - f_fov - viewM < cameraRotation))
			{
				return true;
			}
		}
	}

	return false;
}

/******************************************************************************/
/*!
\brief
Rendering of game objects
*/
/******************************************************************************/
void mainscene::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_STORAGE_CUBE:
		{
			RenderCubes(false, go->pos.x, go->pos.y, go->pos.z, go->rotationY, 2);
			break;
		}
	case GameObject::GO_COMPANION_CUBE:
		{
			RenderCubes(true, go->pos.x, go->pos.y, go->pos.z, go->rotationY, 2);
			break;
		}
	case GameObject::GO_PLAYER_CHAR:
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos);
			modelStack.Rotate(go->rotationX, 1, 0, 0);
			modelStack.Rotate(go->rotationY, 0, 1, 0);
			modelStack.Rotate(go->rotationZ, 0, 0, 1);
			modelStack.Scale(go->scale);
			RenderMesh(meshList[GEO_WSC_SPHERE], false);
			modelStack.PopMatrix();
			break;
		}
	default:
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos);
			modelStack.Rotate(go->rotationX, 1, 0, 0);
			modelStack.Rotate(go->rotationY, 0, 1, 0);
			modelStack.Rotate(go->rotationZ, 0, 0, 1);
			modelStack.Scale(go->scale);
			if(go->obMesh)
			{
				RenderMesh(go->obMesh, true, true, &go->material);
			}
			modelStack.PopMatrix();
			break;
		}
	}
}

/******************************************************************************/
/*!
\brief
Rendering of particles
*/
/******************************************************************************/
void mainscene::RenderParticles(void)
{
	for(std::vector<Particle *>::iterator it = m_ParList.begin(); it != m_ParList.end(); ++it)
	{
		Particle *Par = (Particle *)*it;
		if(Par->active)
		{
			switch(Par->ParticleType)
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
			case Particle::PAR_GEL:
				{
					{
					modelStack.PushMatrix();
					modelStack.Translate(Par->Pos);
					modelStack.Scale(Par->Scale);
					RenderMesh(meshList[GEO_GEL], false);
					modelStack.PopMatrix();
					break;
				}
				}
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
	for(std::vector<BulletInfo *>::iterator it = BIv_BulletList.begin(); it != BIv_BulletList.end(); ++it)
	{
		BulletInfo *BI = (BulletInfo *)*it;
		if(BI->getStatus() && BI->getLife() < 5.95)
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
Rendering of target boards
*/
/******************************************************************************/
void mainscene::RenderTargetBoard(void)
{
	for(std::vector<targetBoard *>::iterator it = targetBList.begin(); it != targetBList.end(); ++it)
	{
		targetBoard *TB = (targetBoard *)*it;
		modelStack.PushMatrix();
		modelStack.Translate(TB->position);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_TARGETBASE], true);
		modelStack.Rotate(-TB->rotation, 1, 0, 0);
		RenderMesh(meshList[GEO_TARGET], true);
		modelStack.PopMatrix();
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
	if(size != 0)
	{
		modelStack.Scale(size, size, size);
	}

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	for(unsigned i = 0; i < Mesh::NUM_TEXTURES; ++i)
	{
		if(mesh->textureID[i] > 0)
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
void mainscene::RenderMesh(Mesh *mesh, bool enableLight, bool enableFog, Material *material)
{
	if(enableFOG && enableFog)
	{
		glUniform1i(m_parameters[U_FOG_ENABLED], 1);
	}
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	switch(m_renderPass)
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
			
			if(enableLight)
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

			for(unsigned i = 0; i < Mesh::NUM_TEXTURES; ++i)
			{
				if(mesh->textureID[i] > 0)
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

		if(enableLight)
		{
			glUniform1i(m_parameters[U_LIGHTENABLED_GBUFFER], 1);

			modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
			glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE_GBUFFER], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

			Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GBUFFER], 1, GL_FALSE, &lightDepthMVP.a[0]);

			//load material
			if(material == NULL)
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
		for(int i = 0; i < 1; ++i)
		{
			if(mesh->textureID[i] > 0)
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
	if(!mesh || mesh->textureID[0] <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
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
	if(!mesh || mesh->textureID[0] <= 0) //Proper error check
		return;

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
	for(unsigned i = 0; i < text.length(); ++i)
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
Renders objects that have alpha channels, having them rendered last to prevent disappearing objects
*/
/******************************************************************************/
void mainscene::RenderObjectsAlpha(void)
{
	//ROOM 1===========================================
	modelStack.PushMatrix();
	modelStack.Translate(200, 180, -400);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_GLASSR1], false, false);
	modelStack.PopMatrix();

	//ROOM 2===========================================
	modelStack.PushMatrix();
	modelStack.Translate(-2080, 180, 200);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_GLASSR1], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2080, 180, -200);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_GLASSR1], false, false);
	modelStack.PopMatrix();


	//Moss
	for(int i = 0; i < 5; ++i)
	{
		modelStack.PushMatrix();
		if(i == 0)
		{
			modelStack.Translate(399, 200, 297);
			modelStack.Rotate(-90, 0, 1, 0);
		}

		if(i == 1)
		{
			modelStack.Translate(200, 200, 481);
		}

		if(i == 2)
		{
			modelStack.Translate(-399, 200, -137);
			modelStack.Rotate(90, 0, 1, 0);
		}

		if(i == 3)
		{
			modelStack.Translate(105, 720, 481);
		}

		if(i == 4)
		{
			modelStack.Translate(-120, 500, 481);
		}

		modelStack.Scale(40, 40, 40);
		RenderMesh(meshList[GEO_MOSS1], false, false);
		modelStack.PopMatrix();
	}

	for(int i = 0; i < 2; i++)
	{
		modelStack.PushMatrix();
		if(i == 0)
		{
			modelStack.Translate(-120, 200, 399);
			modelStack.Rotate(180, 0, 1, 0);
		}
		if(i == 1)
		{
			modelStack.Translate(-200, 120, 481);
		}
		modelStack.Scale(40, 40, 40);
		RenderMesh(meshList[GEO_MOSS2], false, false);
		modelStack.PopMatrix();
	}

	for(int i = 0; i < 3; i++)
	{
		modelStack.PushMatrix();
		if(i == 0)
		{
			modelStack.Translate(103, 201, 399);
			modelStack.Rotate(180, 0, 1, 0);
		}
		if(i == 1)
		{
			modelStack.Translate(399, 201, -370);
			modelStack.Rotate(-90, 0, 1, 0);
		}
		if(i == 2)
		{
			modelStack.Translate(-16, 44, 481);
		}
		modelStack.Scale(40, 40, 40);
		RenderMesh(meshList[GEO_MOSS3], false, false);
		modelStack.PopMatrix();
	}
	//=================================================

	modelStack.PushMatrix();
	modelStack.Translate(0, 40, 0);
	modelStack.Rotate(CalAnglefromPosition(GOp_Player->pos, Vector3(0, 40, 0), true), 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_TREE_1], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 200, 2870);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(300, 300, 300);
	RenderMesh(meshList[GEO_APERTUREOLDLOGO], false);
	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Renders panels
*/
/******************************************************************************/
void mainscene::RenderPanel(Vector3 pos, float rotation, float doorangle)
{
	modelStack.PushMatrix();
	modelStack.Translate(pos);
	modelStack.Rotate(rotation, 0, 1, 0);
	modelStack.PushMatrix();
	modelStack.Rotate(doorangle, 1, 0, 0);

	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_PANELP1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -1, 40);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_UPPERH], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();

	modelStack.Translate(0, -40, 40);
	modelStack.Rotate(((-doorangle)*2) - 90, 1, 0, 0);
	modelStack.Translate(0, -30, 0);
	modelStack.Scale(18, 18, 18);
	RenderMesh(meshList[GEO_PANELP3], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 40);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_PANELP2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -50, 25);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(255 - doorangle, 1, 0, 0);
	modelStack.Scale(14, 14, 14);
	RenderMesh(meshList[GEO_LOWERH], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Code to render the main initial room
*/
/******************************************************************************/
void mainscene::RenderROOM1(void)
{
	modelStack.PushMatrix();
	modelStack.Translate(100, 200, -599);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_CHAMBER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-120, 80, 440);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_FLOOR_TILE_SQUARE], true);
	modelStack.PopMatrix();

	for(unsigned i = 0; i < 2; i++)
	{
		modelStack.PushMatrix();
		if(i == 0)
		{
			modelStack.Translate(-160, 40, 440);
			modelStack.Rotate(90, 0, 1, 0);
		}

		else if(i == 1)
		{
			modelStack.Translate(-80, 40, 440);
			modelStack.Rotate(-90, 0, 1, 0);
		}

		modelStack.Scale(40, 40, 40);
		RenderMesh(meshList[GEO_WALL_WORN_SQUARE], true);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.PushMatrix();
	modelStack.Translate(-120, 80, 440);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_FLOOR_TILE_SQUARE], true);
	modelStack.PopMatrix();

	for(unsigned i = 0; i < 2; i++)
	{
		modelStack.PushMatrix();
		if(i == 0)
		{
			modelStack.Translate(-160, 40, 440);
			modelStack.Rotate(90, 0, 1, 0);
		}

		else if(i == 1)
		{
			modelStack.Translate(-80, 40, 440);
			modelStack.Rotate(-90, 0, 1, 0);
		}

		modelStack.Scale(40, 40, 40);
		RenderMesh(meshList[GEO_WALL_WORN_SQUARE], true);
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	for(unsigned i = 0; i < 3; i++)
	{
		modelStack.PushMatrix();
		if(i == 0)
		{
			modelStack.Translate(200, 60, -400);
		}
		else if(i == 1)
		{
			modelStack.Translate(-200, 60, -400);
		}
		else if(i == 2)
		{
			modelStack.Translate(-200, 180, -400);
		}
		modelStack.Scale(100, 100, 100);
		RenderMesh(meshList[GEO_WALL2], true);
		modelStack.PopMatrix();
	}

	for(unsigned i = 0; i < 2; i++)
	{
		modelStack.PushMatrix();
		if(i == 0)
		{
			modelStack.Translate(-400, 200, -120);
		}

		else if(i == 1)
		{
			modelStack.Translate(-400, 120, -120);
		}
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(40, 40, 40);
		RenderMesh(meshList[GEO_WALL_PANEL_SQUARE], true);
		modelStack.PopMatrix();
	}//*/

	for(unsigned i = 0; i < 2; i++)
	{
		modelStack.PushMatrix();
		if(i == 0)
		{
			modelStack.Translate(-120, 200, 400);
		}

		else if(i == 1)
		{
			modelStack.Translate(-120, 120, 400);
		}
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Scale(40, 40, 40);
		RenderMesh(meshList[GEO_WALL_PANEL_SQUARE], true);
		modelStack.PopMatrix();
	}//

	modelStack.PushMatrix();
	modelStack.Translate(SWITCH2.translateX, 0, SWITCH2.translateZ);
	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_BUTTON_STAND], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, SWITCH2.translateY, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_BUTTON], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SWITCH3.translateX, 0, SWITCH3.translateZ);
	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_BUTTON_STAND], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, SWITCH3.translateY, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_BUTTON], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(350, 240, 350);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_DISPENSER], true);
	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Code to render the 2nd room
*/
/******************************************************************************/
void mainscene::RenderROOM2(void)
{
	for(unsigned i = 0; i < 2; i++)
	{
		modelStack.PushMatrix();
		if(i == 0)
		{
			modelStack.Translate(-480, 200, -120);
		}

		else if(i == 1)
		{
			modelStack.Translate(-480, 120, -120);
		}
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(40, 40, 40);
		RenderMesh(meshList[GEO_WALL_PANEL_SQUARE], true);
		modelStack.PopMatrix();
	}

	for(int i = 1; i < 20; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-650.f, -20.f, static_cast<float>((45 * i) - 440));
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_RAILING], true);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(-2400, 25, 200);
	RenderTurret();
	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Code to render the main viewing room (The one behind the glass)
*/
/******************************************************************************/
void mainscene::RenderViewingRoom1(void)
{
	for(unsigned i = 0; i < 2; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(200, 120, -600);

		if(i == 0)
		{
			modelStack.Rotate(-90, 1, 0, 0);
		}
		else if(i == 1)
		{
			modelStack.Translate(0, 120, 0);
			modelStack.Rotate(90, 1, 0, 0);
		}

		modelStack.Scale(200, 200, 200);
		RenderMesh(meshList[GEO_FLOOR_CONCRETE], true);
		modelStack.PopMatrix();
	}

	for(unsigned i = 0; i < 3; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(200, 180, -600);
		if(i == 0)
		{
			modelStack.Translate(-200, 0, 0);
			modelStack.Rotate(90, 0, 1, 0);
		}

		else if(i == 1)
		{
			modelStack.Translate(200, 0, 0);
			modelStack.Rotate(-90, 0, 1, 0);
		}

		modelStack.Scale(200, 200, 200);
		RenderMesh(meshList[GEO_WALL3], true);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(60, 120, -440);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_OBJTABLE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(350, 215, -600);
	modelStack.Scale(12, 12, 12);
	modelStack.Rotate(110, 1, 0, 0);
	modelStack.Rotate(10, 0, 0, 1);
	RenderMesh(meshList[GEO_OBJ_TV_A], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(400, 120, -550);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_DOORA], true);
	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Code to render the 2nd viewing room in the 2nd room 
*/
/******************************************************************************/
void mainscene::RenderViewingRoom2(void)
{
	modelStack.PushMatrix();
	modelStack.Translate(-2080, 180, 200);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_WALL2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2080, 180, -200);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_WALL2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2280, 0, -200);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_FLOOR_CONCRETE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2280, 0, 200);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_FLOOR_CONCRETE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2280, 120, -200);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_FLOOR_CONCRETE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2280, 120, 200);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_FLOOR_CONCRETE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2280, 120, -400);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_WALL3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2280, 120, 400);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_WALL3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2480, 120, 200);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_WALL3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2480, 120, -200);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_WALL3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2477, 60, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_CHAMBER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2478, 80, -340);
	modelStack.Scale(12, 12, 12);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(-82, 0, 0, 1);
	RenderMesh(meshList[GEO_OBJ_TV_A], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2477, 0, 320);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_DOORA], true);
	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Code to render the platform and opening in the ruins area
*/
/******************************************************************************/
void mainscene::RenderRuinsP1(void)
{
	for(unsigned i = 0; i < 5; ++i)
	{
		modelStack.PushMatrix();
		if(i == 0)
		{
			modelStack.Translate(-100, 0, 680);
			modelStack.Rotate(-90, 1, 0, 0);
		}

		else if(i == 1)
		{
			modelStack.Translate(-100, -200, 880);
		}

		else if(i == 2)
		{
			modelStack.Translate(100, -200, 680);
			modelStack.Rotate(90, 0, 1, 0);
		}

		else if(i == 3)
		{
			modelStack.Translate(-300, -200, 680);
			modelStack.Rotate(-90, 0, 1, 0);
		}

		else if(i == 4)
		{
			modelStack.Translate(-100, -400, 680);
			modelStack.Rotate(90, 1, 0, 0);
		}
		modelStack.Scale(200, 200, 200);
		RenderMesh(meshList[GEO_FLOOR_CONCRETE], true);
		modelStack.PopMatrix();
	}


	for(unsigned i = 0; i < 3; i++)
	{
		modelStack.PushMatrix();
		if(i == 0)
		{
			modelStack.Translate(280, 120, 480);
		}

		else if(i == 1)
		{
			modelStack.Translate(-280, 120, 480);
		}

		else if(i == 2)
		{
			modelStack.Translate(40, 120, 480);
		}

		modelStack.Scale(200, 200, 200);
		RenderMesh(meshList[GEO_WALL_WORN1], true);
		modelStack.PopMatrix();
	}

	for(unsigned i = 0; i < 2; i++)
	{
		modelStack.PushMatrix();
		if(i == 0)
		{
			modelStack.Translate(-120, 200, 480);
		}

		else if(i == 1)
		{
			modelStack.Translate(-120, 120, 480);
		}

		modelStack.Scale(40, 40, 40);
		RenderMesh(meshList[GEO_WALL_WORN_SQUARE], true);
		modelStack.PopMatrix();
	}

	for(unsigned i = 1; i < 10; ++i)
	{
		modelStack.PushMatrix();

		modelStack.Translate(95.f, 0, static_cast<float>(450 + (45 * i)));

		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_RAILING], true);
		modelStack.PopMatrix();
	}

	for(unsigned i = 1; i < 10; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-295.f, 0.f, static_cast<float>(450 + (45 * i)));
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_RAILING], true);
		modelStack.PopMatrix();
	}
}

/******************************************************************************/
/*!
\brief
Code to render the ruins area
*/
/******************************************************************************/
void mainscene::RenderRuinsP2(void)
{
	for(int k = 0; k < 6; ++k)//Rusted wall-------------------------------------
	{
		modelStack.PushMatrix();
		if(k == 0)
		{
			modelStack.Translate(0, 240, 0);
		}

		if(k == 1)
		{
			modelStack.Translate(0, -240, 0);
		}

		if(k == 2)
		{
			modelStack.Translate(0, 480, 0);
		}

		if(k == 3)
		{
			modelStack.Translate(0, -480, 0);
		}

		if(k == 4)
		{
			modelStack.Translate(0, 720, 0);
		}

		for(int j = 0; j < 3; ++j)
		{
			modelStack.PushMatrix();
			if(j == 0)
			{
				modelStack.Translate(0, 0, 800);
			}

			if(j == 1)
			{
				modelStack.Translate(0, 0, 1600);
			}

			for(int i = 0; i < 2; ++i)
			{
				modelStack.PushMatrix();
				if(i == 0)
				{
					modelStack.Translate(400, 120, 880);
					modelStack.Rotate(-90, 0, 1, 0);
				}
				if(i == 1)
				{
					modelStack.Translate(-400, 120, 880);
					modelStack.Rotate(90, 0, 1, 0);
				}
				modelStack.Scale(200, 200, 200);
				RenderMesh(meshList[GEO_WALLRUSTED1], true);
				modelStack.PopMatrix();
			}

			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}

	//Celling------------------------------------------------------------
	for(int j = 0; j < 2; ++j)
	{
		modelStack.PushMatrix();
		if(j == 0)
		{
			modelStack.Translate(0, 0, 1440);
		}
		for(int i = 0; i < 4; ++i)
		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 960, 600);
			if(i == 0)
			{
				modelStack.Translate(0, 0, 240);
			}

			if(i == 1)
			{
				modelStack.Translate(0, 0, 480);
			}

			if(i == 2)
			{
				modelStack.Translate(0, 0, 720);
			}

			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(200, 200, 200);
			RenderMesh(meshList[GEO_WALLRUSTED1], true);
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}
	//Celling END

	//FLOOR
	for(int j = 0; j < 2; ++j)
	{
		modelStack.PushMatrix();
		if(j == 0)
		{
			modelStack.Translate(0, 0, 960);
		}
		for(int i = 0; i < 4; ++i)
		{
			modelStack.PushMatrix();
			modelStack.Translate(0, -480, 600);
			if(i == 0)
			{
				modelStack.Translate(0, 0, 240);
			}

			if(i == 1)
			{
				modelStack.Translate(0, 0, 480);
			}

			if(i == 2)
			{
				modelStack.Translate(0, 0, 720);
			}

			if(i == 3)
			{
				modelStack.Translate(0, 0, 960);
			}

			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(200, 200, 200);
			RenderMesh(meshList[GEO_WALLRUSTED1], true);
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(0, -480, 2760);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_WALLRUSTED1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -480, 600);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_WALLRUSTED1], true);
	modelStack.PopMatrix();

	//FLOOR END

	//wall of the room------------------------------------------------
	for(int j = 0; j < 5; ++j)
	{
		modelStack.PushMatrix();
		if(j == 0)
		{
			modelStack.Translate(0, 240, 0);
		}

		if(j == 1)
		{
			modelStack.Translate(0, 480, 0);
		}

		if(j == 2)
		{
			modelStack.Translate(0, 720, 0);
		}

		if(j == 3)
		{
			modelStack.Translate(0, -240, 0);
		}

		if(j == 4)
		{
			modelStack.Translate(0, -480, 0);
		}

		for(int i = 0; i < 3; i++)
		{
			modelStack.PushMatrix();
			if(i == 0)
			{
				modelStack.Translate(280, 120, 480);
			}

			if(i == 1)
			{
				modelStack.Translate(-280, 120, 480);
			}

			if(i == 2)
			{
				modelStack.Translate(40, 120, 480);
			}

			modelStack.Scale(200, 200, 200);
			RenderMesh(meshList[GEO_WALL_WORN1], true);
			modelStack.PopMatrix();
		}

		for(int i = 0; i < 3; i++)
		{
			modelStack.PushMatrix();
			if(i == 0)
			{
				modelStack.Translate(-120, 200, 480);
			}

			if(i == 1)
			{
				modelStack.Translate(-120, 120, 480);
			}

			if(i == 2)
			{
				modelStack.Translate(-120, 40, 480);
			}

			modelStack.Scale(40, 40, 40);
			RenderMesh(meshList[GEO_WALL_WORN_SQUARE], true);
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}
	//wall of the room end

	modelStack.PushMatrix();
	modelStack.Translate(0, -480, 980);

	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_ELBOTTOM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 440, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_ELBOTTOM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 180, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_ELBOTTOM], true);
	modelStack.PopMatrix();

	/*modelStack.PushMatrix();
	modelStack.Translate(0, 320, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_ELSHAFT], true);
	modelStack.PopMatrix();*/


	for(int i = 2; i < 5; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, static_cast<float>(320*i), 0);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_ELSHAFT], true);
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Code to render the remainder of the ruins area (Elevator shaft and faraway wall)
*/
/******************************************************************************/
void mainscene::RenderRuinsP3(void)
{

	modelStack.PushMatrix();//Flickering light
	modelStack.Translate(lights[3].position.x - 8, lights[3].position.y, lights[3].position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(1, 5, 1);
	if(LightSwitch == false)
	{
		RenderMesh(meshList[GEO_FLight], true);
	}
	else
	{
		RenderMesh(meshList[GEO_FLight], false);
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(200, -480, 1680);

	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_ELBOTTOM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 1600, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_ELBOTTOM], true);
	modelStack.PopMatrix();

	for(int i = 1; i < 5; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, static_cast<float>(320*i), 0);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_ELSHAFT], true);
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	for(int i = 0; i < 2; ++i)
	{
		modelStack.PushMatrix();
		if(i == 0)
		{
			modelStack.Translate(0, 560, 2880);
		}
		if(i == 1)
		{
			modelStack.Translate(0, -240, 2880);
		}
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Scale(400, 400, 400);
		RenderMesh(meshList[GEO_WALL_WORN_RUINS1], true);
		modelStack.PopMatrix();
	}

	/*modelStack.PushMatrix();
	modelStack.Translate(0, 0, 2600);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_GLADOS], true);
	modelStack.PopMatrix();//*/

	modelStack.PushMatrix();
	modelStack.Translate(400, -480, 1680);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_DOORA], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(360, 1120, 1680);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_WALL_WORN_SQUARE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(320, 1080, 1680);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_WALL_WORN_SQUARE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(360, 1040, 1680);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_WALL_WORN_SQUARE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(360, 1080, 1640);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_WALL_WORN_SQUARE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(360, 1080, 1720);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_WALL_WORN_SQUARE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(LIFT.translateX, LIFT.translateY, LIFT.translateZ);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_ELEVATOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(LIFT2.translateX, LIFT2.translateY, LIFT2.translateZ);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_ELEVATOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SWITCH.translateX, 0, SWITCH.translateZ);
	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_BUTTON_STAND], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, SWITCH.translateY, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_BUTTON], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Code to render the exterior of the scene
*/
/******************************************************************************/
void mainscene::RenderExterior(void)
{
	//Terrain
	modelStack.PushMatrix();
	modelStack.Translate(monalisa.position);
	modelStack.Scale(monalisa.scale);
	RenderMesh(monalisa.mesh, false);
	modelStack.PopMatrix();

	//Water
	modelStack.PushMatrix();
	modelStack.Translate(monalisa.position);
	modelStack.Translate(4200, 10, -5000);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(10000, 10000, 10000);
	RenderMesh(meshList[GEO_WATER_SPRITE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(monalisa.position);
	modelStack.Translate(4200, 10, 5000);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(10000, 10000, 10000);
	RenderMesh(meshList[GEO_WATER_SPRITE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(monalisa.position);
	modelStack.Translate(-5800, 10, 5400);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(10000, 10000, 10000);
	RenderMesh(meshList[GEO_WATER_SPRITE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(monalisa.position);
	modelStack.Translate(-5800, 10, -5400);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(10000, 10000, 10000);
	RenderMesh(meshList[GEO_WATER_SPRITE], false);
	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Code to render the minimap
*/
/******************************************************************************/
void mainscene::RenderMinimap(void)
{
	float MapR = -CalAnglefromPosition(FPC.target, FPC.position, true);
	modelStack.PushMatrix();
	modelStack.Translate(Application::GetWindowWidth() * 0.1f - 12, Application::GetWindowHeight() * 0.1f - 12, 0);

	for(std::vector<targetBoard *>::iterator it = targetBList.begin(); it != targetBList.end(); ++it)
	{
		targetBoard *TB = (targetBoard *)*it;

		if(TB->active)
		{
			Vector3 targetMapOffset(0, 0, 0);
			modelStack.PushMatrix();
			targetMapOffset = Vector3(-TB->position.x, TB->position.z, 0) - Vector3(-FPC.position.x, FPC.position.z, 0);
			if(targetMapOffset.LengthSquared() > 720*720)
			{
				targetMapOffset = targetMapOffset.Normalize() * 720;
			}

			modelStack.Rotate(MapR, 0, 0, 1);
			modelStack.Translate(targetMapOffset*0.01f);
			modelStack.Translate(0, 0, 0.1f);
			RenderMeshin2D(meshList[GEO_BULLET], false);
			modelStack.PopMatrix();
		}
	}

	modelStack.PushMatrix();
	modelStack.Rotate(MapR, 0, 0, 1);
	modelStack.Scale(10, 10, 10);
	RenderMeshin2D(meshList[GEO_MINIMAP_BASE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0.2f);
	RenderMeshin2D(meshList[GEO_MINIMAP_PLAYER], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Code to render the turret
*/
/******************************************************************************/
void mainscene::RenderTurret(void)
{
	//===========================================================================
	modelStack.PushMatrix();//Turret Code
	//
	//              |
	//             /'\
	//            | O |
	//             \_/
	//            / | \
	//    THERE YOU ARE.....................PEW PEW PEW PEW PEW!
	//Turret translation code here===============================================
	modelStack.Scale(0.5, 0.5, 0.5);

	modelStack.Translate(0*2, 0*2, 0*2);
	modelStack.Rotate(TURRET.rotateA, 0, 1, 0);

	modelStack.PushMatrix();//Turret Antenna 1
	modelStack.Translate(-3, 28, 1);
	modelStack.Scale(1, 1.5, 1);
	RenderMesh(meshList[GEO_TURRETANTENNA], true);
	modelStack.PopMatrix();//TA1 END

	modelStack.PushMatrix();//Turret Antenna 2
	modelStack.Translate(-4, 28, 3);

	modelStack.PushMatrix();
	modelStack.Scale(1, 2, 1);
	RenderMesh(meshList[GEO_TURRETANTENNA], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 6.6f, -1);
	modelStack.Rotate(-50, 1, 0, 0);
	modelStack.Scale(1, 0.5f, 1);
	RenderMesh(meshList[GEO_TURRETANTENNA], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 10, -2);
	RenderMesh(meshList[GEO_TURRETANTENNA], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();//TA2 END



	modelStack.PushMatrix();//The light of the eyes
	modelStack.Translate(14, 0, 0);
	/*lights[2].position.x = modelStack.Top().a[12];
	lights[2].position.y = modelStack.Top().a[13];
	lights[2].position.z = modelStack.Top().a[14];*/
	modelStack.PopMatrix();

	modelStack.PushMatrix();//----------EYES
	modelStack.Translate(7.5, 0, 0);
	modelStack.Scale(4, 4, 4);
	RenderMesh(meshList[GEO_TURRETEYES1], true);
	modelStack.PopMatrix();//-----------EYES

	//glDisable(GL_CULL_FACE);//Render both sides
	modelStack.PushMatrix();
	//RIGHT LEG TRANSLATION CODE HERE...................................
	modelStack.Rotate(TLEG1.rotateA, 0, 1, 0);
	modelStack.Rotate(10, 0, 0, 1);
	modelStack.Translate(-3, 0, -1);

	//First connecting part of leg (black)(RIGHT)
	modelStack.PushMatrix();
	modelStack.Translate(10, -16, -4);
	modelStack.Rotate(-20, 0, 0, 1);
	modelStack.Rotate(30, 0, 1, 0);
	modelStack.Scale(28, 30, 6);
	RenderMesh(meshList[GEO_TURRETLEGSP1], true);
	modelStack.PopMatrix();

	//Second part (white)(RIGHT)
	modelStack.PushMatrix();
	modelStack.Translate(5, -40, -8);
	modelStack.Rotate(140, 0, 0, 1);
	modelStack.Rotate(-25, 0, 1, 0);
	modelStack.Scale(28, 30, 10);
	RenderMesh(meshList[GEO_TURRETLEGSP2], true);
	modelStack.PopMatrix();

	//Third part (black)(RIGHT)
	modelStack.PushMatrix();
	modelStack.Translate(5.f, -45.3f, -14.f);
	modelStack.Rotate(100, 0, 0, 1);
	modelStack.Rotate(-25, 0, 1, 0);
	modelStack.Scale(36, 32, 6);
	RenderMesh(meshList[GEO_TURRETLEGSP1], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();//LEGS RIGHT END

	modelStack.PushMatrix();
	//LEFT LEG TRANSLATION CODE HERE....................................
	modelStack.Rotate(TLEG2.rotateA, 0, 1, 0);
	modelStack.Rotate(10, 0, 0, 1);
	modelStack.Translate(-3, 0, 1);

	//First connecting part of leg (black)(LEFT)
	modelStack.PushMatrix();
	modelStack.Translate(10, -16, 4);
	modelStack.Rotate(-20, 0, 0, 1);
	modelStack.Rotate(-30, 0, 1, 0);
	modelStack.Scale(28, 30, 6);
	RenderMesh(meshList[GEO_TURRETLEGSP1], true);
	modelStack.PopMatrix();

	//Second part (white)(LEFT)
	modelStack.PushMatrix();
	modelStack.Translate(5, -40, 8);
	modelStack.Rotate(140, 0, 0, 1);
	modelStack.Rotate(25, 0, 1, 0);
	modelStack.Scale(28, 30, 10);
	RenderMesh(meshList[GEO_TURRETLEGSP2], true);
	modelStack.PopMatrix();

	//Third part (black)(RIGHT)
	modelStack.PushMatrix();
	modelStack.Translate(5.f, -45.3f, 14.f);
	modelStack.Rotate(100, 0, 0, 1);
	modelStack.Rotate(25, 0, 1, 0);
	modelStack.Scale(36, 32, 6);
	RenderMesh(meshList[GEO_TURRETLEGSP1], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();//LEGS LEFT END

	modelStack.PushMatrix();
	//REAR LEG TRANSLATION CODE HERE...................................

	//First part of leg connected to turret
	modelStack.PushMatrix();
	modelStack.Translate(-2, -8, 0);
	modelStack.Rotate(-40, 0, 0, 1);
	modelStack.Scale(40, 30, 6);
	RenderMesh(meshList[GEO_TURRETLEGSP1], true);
	modelStack.PopMatrix();
	//Second white part
	modelStack.PushMatrix();
	modelStack.Translate(-16, -24.5, 0);
	modelStack.Rotate(TLEG3.rotateA - 40, 0, 0, 1);
	modelStack.Scale(38, 30, 12);
	RenderMesh(meshList[GEO_TURRETLEGSP2], true);
	modelStack.PopMatrix();

	//Third white part
	modelStack.PushMatrix();
	modelStack.Rotate(TLEG3.rotateA, 0, 0, 1);
	modelStack.Translate(36, -10, 0);
	modelStack.Scale(58, 32, 6);
	RenderMesh(meshList[GEO_TURRETLEGSP1], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();//REAR LEGS END

	/*modelStack.PushMatrix();//TURRET SIDE 1
	//SIDE 1 TRANSLATION CODE====================================================

	modelStack.Translate(S1.translateX, S1.translateY, S1.translateZ);

	//SIDE 1 TRANSLATION CODE

	modelStack.PushMatrix();

	modelStack.Translate(0, 0, -0.5);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(20, 20, 50);
	RenderMesh(meshList[GEO_TURRETBODY], true);

	modelStack.PopMatrix();

	//Gun mount
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -6);
	modelStack.Scale(6, 14, 5);
	RenderMesh(meshList[GEO_TURRETGUNMOUNT1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();

	modelStack.Translate(0, 0, -3);
	modelStack.PushMatrix();
	modelStack.Scale(1, 1, 10);
	RenderMesh(meshList[GEO_TURRETGUNMOUNT2], true);
	modelStack.PopMatrix();

	modelStack.Translate(0, 3, 0);
	modelStack.Rotate(-30, 1, 0, 0);
	modelStack.Scale(1, 1, 10);
	RenderMesh(meshList[GEO_TURRETGUNMOUNT2], true);

	modelStack.PopMatrix();//Gun mount end

	for(unsigned short i = 0; i < 2; i++)
	{
	modelStack.PushMatrix();//GUN BARREL
	if(i == 0)
	{
	modelStack.Translate(2, 3, -6);
	}

	if(i == 1)
	{
	modelStack.Translate(2, -1, -6);
	}

	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 0, 1);
	RenderMesh(meshList[GEO_GUNBARREL1], true);

	modelStack.Translate(0, -3, 0);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshList[GEO_GUNBARREL2], true);

	modelStack.PushMatrix();//GUN BULLET------------------------------------------------------
	modelStack.Translate(0, BULLETR.translateY, 0);

	modelStack.Rotate(180, 1, 0, 0);
	modelStack.PushMatrix();
	modelStack.Scale(1.f, 0.3f, 1.f);
	RenderMesh(meshList[GEO_BULLETP1], true);
	modelStack.PopMatrix();

	modelStack.Translate(0, 0.9f, 0);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(0.92f, 1.f, 0.92f);
	RenderMesh(meshList[GEO_BULLETP2], true);

	modelStack.PopMatrix();//GUN BULLET-------------------------------------------------------
	modelStack.PopMatrix();
	modelStack.PopMatrix();//GUN BARREL END
	}
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -9);
	modelStack.Scale(7, 15, 2);
	RenderMesh(meshList[GEO_TURRETGUNMOUNT3], true);
	modelStack.PopMatrix();


	//TURRET SIDE 1 END
	modelStack.PopMatrix();*/


	/*modelStack.PushMatrix();
	//SIDE 2 TRANSLATION CODE==================================

	modelStack.Translate(S2.translateX, S2.translateY, S2.translateZ);

	//SIDE 2 TRANSLATION CODE
	//TURRET SIDE 2

	modelStack.PushMatrix();

	modelStack.Translate(0, 0, 0.5);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(20, 20, 50);
	RenderMesh(meshList[GEO_TURRETBODY], true);

	modelStack.PopMatrix();

	//Gun mount
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 6);
	modelStack.Scale(6, 14, 5);
	RenderMesh(meshList[GEO_TURRETGUNMOUNT1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 3);

	modelStack.PushMatrix();
	modelStack.Scale(1, 1, 10);
	RenderMesh(meshList[GEO_TURRETGUNMOUNT2], true);
	modelStack.PopMatrix();

	modelStack.Translate(0, 3, 0);
	modelStack.Rotate(30, 1, 0, 0);
	modelStack.Scale(1, 1, 10);
	RenderMesh(meshList[GEO_TURRETGUNMOUNT2], true);

	modelStack.PopMatrix();//Gun mount end
	//
	for(unsigned short i = 0; i < 2; i++)
	{
	modelStack.PushMatrix();//GUN BARREL
	if(i == 0)
	{
	modelStack.Translate(2, 3, 6);
	}

	if(i == 1)
	{
	modelStack.Translate(2, -1, 6);
	}

	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 0, 1);
	RenderMesh(meshList[GEO_GUNBARREL1], true);

	modelStack.Translate(0, -3, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[GEO_GUNBARREL2], true);

	modelStack.PushMatrix();//GUN BULLET------------------------------------------------------
	modelStack.Translate(0, BULLETL.translateY, 0);

	modelStack.Rotate(180, 1, 0, 0);
	modelStack.PushMatrix();
	modelStack.Scale(1, 0.3f, 1);
	RenderMesh(meshList[GEO_BULLETP1], true);
	modelStack.PopMatrix();

	modelStack.Translate(0.f, 0.9f, 0.f);
	modelStack.Rotate(180.f, 1.f, 0.f, 0.f);
	modelStack.Scale(0.92f, 1.f, 0.92f);
	RenderMesh(meshList[GEO_BULLETP2], true);

	modelStack.PopMatrix();//GUN BULLET-------------------------------------------------------

	modelStack.PopMatrix();
	modelStack.PopMatrix();//GUN BARREL END
	}


	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 9);
	modelStack.Scale(7, 15, 2);
	RenderMesh(meshList[GEO_TURRETGUNMOUNT3], true);
	modelStack.PopMatrix();

	glEnable(GL_CULL_FACE);
	modelStack.PopMatrix();//TURRET SIDE 2 END*/

	for(unsigned short i = 0; i < 2; i++)
	{
		modelStack.PushMatrix();
		if(i == 0)
		{
			modelStack.Translate(0, 0, 0.5);
		}
		if(i == 1)
		{
			modelStack.Translate(0, 0, -0.5);
			modelStack.Rotate(180, 0, 1, 0);
		}
		modelStack.Scale(20, 50, 20);

		RenderMesh(meshList[GEO_TURRETBODYMAIN], true);
		modelStack.PopMatrix();
	}
	//Mid Cylinder cut
	modelStack.PushMatrix();
	modelStack.Scale(0.4f, 1.f, 0.2f);
	modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
	RenderMesh(meshList[GEO_TURRETCYLINDER], true);
	modelStack.PopMatrix();
	//Mid Cylinder cut end
	modelStack.PopMatrix();//Turret code-------------
}

/******************************************************************************/
/*!
\brief
Code for rendering cubes
*/
/******************************************************************************/
void mainscene::RenderCubes(bool Companion, float x, float y, float z, float R, int RXYZ)
{
	//   ____      ____
	//	|    |____|    |
	//  | _/"______"\_ |
	//   |  /      \  |
	//   | |        | |        <3
	//   |_ \______/ _|
	//  |  \, ____ ,/  |
	//  |____|    |____|

	modelStack.PushMatrix();//CUBE push 1
	//Cube Translation here
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	modelStack.Translate(x*2.f, y*2.f, z*2.f);

	if(RXYZ == 1)
	{
		modelStack.Rotate(R, 1.f, 0.f, 0.f);
	}
	if(RXYZ == 2)
	{
		modelStack.Rotate(R, 0.f, 1.f, 0.f);
	}
	if(RXYZ == 3)
	{
		modelStack.Rotate(R, 0.f, 0.f, 1.f);
	}

	modelStack.Scale(1.2f, 1.2f, 1.2f);
	//Cube Translation here
	modelStack.PushMatrix();//Push 2
	modelStack.Scale(40, 40, 40);
	modelStack.PushMatrix();//Push 3
	if(Companion == true)
	{
		RenderMesh(meshList[GEO_WSC_SPHERE_COMPANION], true);
	}
	else
	{
		RenderMesh(meshList[GEO_WSC_SPHERE], true);
	}

	RenderMesh(meshList[GEO_WSC], true);

	modelStack.PopMatrix();//pop 3
	modelStack.PopMatrix();//Pop 2


	modelStack.PushMatrix();//Edge push
	for(unsigned short w = 0; w < 2; w++)
	{
		if(w == 1)
		{
			modelStack.Rotate(90, 0, 1, 0);
			modelStack.Translate(-40, 0, 0);
		}
		for(unsigned short i = 0; i < 2; i++)
		{
			if(i == 1)
			{
				modelStack.Translate(0, 0, -40);
			}

			modelStack.PushMatrix();//Push 2
			modelStack.Translate(0, 20, 20);
			modelStack.Scale(12, 5, 5);
			RenderMesh(meshList[GEO_WSC_MID_EDGE], true);
			modelStack.Translate(0, -8, 0);
			RenderMesh(meshList[GEO_WSC_MID_EDGE], true);
			modelStack.PopMatrix();//Pop 2
		}
	}
	modelStack.PopMatrix();//Edge pop

	//Edge 2 Push
	for(unsigned short i = 0; i < 2; i++)
	{
		modelStack.PushMatrix();
		if(i == 1)
		{
			modelStack.Translate(-40, 0, 0);
		}
		modelStack.Translate(20, 0, 20);
		modelStack.Scale(5, 12, 5);
		RenderMesh(meshList[GEO_WSC_MID_EDGE], true);
		modelStack.Translate(0, 0, -8);
		RenderMesh(meshList[GEO_WSC_MID_EDGE], true);
		modelStack.PopMatrix();
	}
	//Edge 2 Pop


	modelStack.PushMatrix();//Edge 3 push
	for(unsigned short w = 0; w < 2; w++)
	{
		if(w == 1)
		{
			modelStack.Translate(0, -34, 0);
		}

		for(unsigned short i = 0; i < 4; i++)
		{
			modelStack.PushMatrix();
			if(i == 0)
			{
				modelStack.Translate(17, 17, 17);
			}

			if(i == 1)
			{
				modelStack.Translate(-17, 17, 17);
			}

			if(i == 2)
			{
				modelStack.Translate(-17, 17, -17);
			}

			if(i == 3)
			{
				modelStack.Translate(17, 17, -17);
			}
			modelStack.PushMatrix();
			modelStack.Scale(12, 12, 12);
			RenderMesh(meshList[GEO_WSC_SIDE_EDGE], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}
	}

	modelStack.PopMatrix();//Edge 3 pop

	for(unsigned short L = 0; L < 6; L++)//Lines between sphere and edge
	{
		if(L == 0)
		{
			modelStack.Rotate(90, 0, 1, 0);
		}

		if(L == 1)
		{
			modelStack.Rotate(90, 0, 1, 0);
		}

		if(L == 2)
		{
			modelStack.Rotate(90, 0, 1, 0);
		}

		if(L == 3)
		{
			modelStack.Rotate(90, 0, 1, 0);
		}

		if(L == 4)
		{
			modelStack.Rotate(90, 0, 0, 1);
		}
		if(L == 5)
		{
			modelStack.Rotate(180, 0, 0, 1);
		}
		for(int i = 0; i < 2; i++)
		{
			modelStack.PushMatrix();
			if(i == 0)
			{
				modelStack.Rotate(90, 1, 0, 0);
			}
			modelStack.Translate(20, 0, 0);
			modelStack.Scale(1, 40, 1);
			if(Companion == true)
			{
				RenderMesh(meshList[GEO_WSC_LINES_COMPANION], true);
			}
			else
			{
				RenderMesh(meshList[GEO_WSC_LINES], true);
			}
			modelStack.PopMatrix();
		}
	}

	modelStack.PopMatrix();//pop 1 Cube

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
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			if(!go->dynamicRendering)
			{
				RenderGO(go);
			}
			else//Dynamic rendering
			{
				if(isVisible(go->pos))
				{
					RenderGO(go);
				}
			}
		}
	}

	//RenderTurret();

	RenderTargetBoard();

	modelStack.PushMatrix();
	modelStack.Translate(60, 151, -440);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_OBJCAKE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(10, 10, 10);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_OBJCAKE], true);
	modelStack.PopMatrix();

	RenderPanel(Vector3(PanelP1.translateX, PanelP1.translateY, PanelP1.translateZ), 0.f, PanelP1.rotateA);
	RenderPanel(Vector3(PanelP2.translateX, PanelP2.translateY, PanelP2.translateZ), -90.f, PanelP2.rotateA);

	RenderRuinsP1();
	RenderRuinsP2();
	RenderRuinsP3();
	
	if(GOp_Player->pos.y > 1000)
	{
		RenderExterior();
	}
	else
	{
		RenderROOM1();
		RenderROOM2();
		RenderViewingRoom1();
		RenderViewingRoom2();
	}

	RenderParticles();

	if(weaponsEnabled)
	{
		modelStack.PushMatrix();
		modelStack.Translate(GOp_Player->pos);
		modelStack.Rotate(CamRotationYaw, 0, 1, 0);
		modelStack.Rotate(-CamRotationPitch, 1, 0, 0);
		modelStack.Translate(weaponList[currentWeapon].currentpos);
		modelStack.Scale(weaponList[currentWeapon].scale);
		RenderMesh(weaponList[currentWeapon].mesh, true);
		if(currentWeapon == WT_L11A3 && renderScope == true)
		{
			RenderMesh(meshList[GEO_L11A3_2], true);
		}
		modelStack.PopMatrix();
	}
}

/******************************************************************************/
/*!
\brief
Renders the entire world without shadow
*/
/******************************************************************************/
void mainscene::RenderWorldNoShadow(void)
{
	if(renderAxis == true)
	{
		modelStack.PushMatrix();
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_AXES], false);
		modelStack.PopMatrix();
	}

	RenderSkybox();

	if(weaponsEnabled)
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
	if(weaponsEnabled)
	{
		if(currentWeapon == WT_L11A3 && renderScope == false)//Scope texture
		{
			RenderMeshin2D(meshList[GEO_SNIPER_CROSSHAIR_1], false, 100, Application::GetWindowWidth()*0.05f + ScopeAnim, Application::GetWindowHeight()*0.05f - ScopeAnim * 0.2f);
			RenderMeshin2D(meshList[GEO_SNIPER_CROSSHAIR_2], false, 120, Application::GetWindowWidth()*0.05f + ScopeAnim - 100, Application::GetWindowHeight()*0.05f - ScopeAnim * 0.2f);
		}
		else
		{
			RenderMinimap();
		}
		if(!AimDownSight && currentWeapon != WT_L11A3)
		{
			RenderMeshin2D(meshList[GEO_CROSSHAIR], false, 1, Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f - 1 - f_curRecoil * 0.5f);
			RenderMeshin2D(meshList[GEO_CROSSHAIR], false, 1, Application::GetWindowWidth()*0.05f + 1 + f_curRecoil * 0.5f, Application::GetWindowHeight()*0.05f, 90);
			RenderMeshin2D(meshList[GEO_CROSSHAIR], false, 1, Application::GetWindowWidth()*0.05f - 1 - f_curRecoil * 0.5f, Application::GetWindowHeight()*0.05f, 90);
			RenderMeshin2D(meshList[GEO_CROSSHAIR], false, 1, Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f + 1 + f_curRecoil * 0.5f);
		}

		RenderMeshin2D(meshList[GEO_UI_AMMO], false, 3.5, Application::GetWindowWidth()*0.1f - 4, 2);
		if(weaponList[currentWeapon].CurrentClip == 0)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "R", Color(0, 1, 1), 3, Application::GetWindowWidth()*0.1f - 5, 1.5);
		}
		else
		{
			RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(static_cast<long double>(weaponList[currentWeapon].CurrentClip)), Color(0, 1, 1), 3, Application::GetWindowWidth()*0.1f - 5, 1.5);
		}
	}

	if(ShootingRangeMode)
	{
		std::ostringstream ss1, ss2, ss3;
		ss1 << "Time Left: " << static_cast<int>(ShootingRangeTimer);
		ss3 << "Targets Hit: " << targetsHit;
		RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 1), 3, 1.5, Application::GetWindowHeight()*0.1f - 3);
		RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 1), 3, 1.5, Application::GetWindowHeight()*0.1f - 6);
		if(shootingRangeDelay > 0.1)
		{
			ss2 << "Get ready to shoot in: " << static_cast<int>(shootingRangeDelay);
			RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(1, 0.5, 0.5), 2, Application::GetWindowWidth()*0.05f - 20, Application::GetWindowHeight()*0.05f + 10);
		}
	}
	if(showScoreTimer > 0)
	{
		std::ostringstream ss4;
		ss4 << "Final Score: " << targetsHit;
		RenderTextOnScreen(meshList[GEO_TEXT], ss4.str(), Color(1, 0.5, 0.5), 2, Application::GetWindowWidth()*0.05f - 12, Application::GetWindowHeight()*0.05f + 12);
	}

	if(DisplayInfo == true)
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
	RenderMeshin2D(meshList[GEO_LIGHT_DEPTH_QUAD], false, 5,  Application::GetWindowWidth()*0.08f, Application::GetWindowHeight()*0.09f);
	

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

	if(lights[0].type == LIGHT_DIRECTIONAL)
	{
		//m_lightDepthProj.SetToOrtho(-100 - FPC.position.z*0.1, 100 - FPC.position.z*0.1, -100 - FPC.position.x*0.1, 100 - FPC.position.x*0.1, -200 + FPC.position.y*0.1, 400 + FPC.position.y*0.1);
		m_lightDepthProj.SetToOrtho(-100 - FPC.position.z*0.1, 100 - FPC.position.z*0.1, -100 - FPC.position.x*0.1, 100 - FPC.position.x*0.1, 0, 600);
	}
	else
	{
		m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);
	}
	//m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y + FPC.position.y*0.1, lights[0].position.z, 0, 0, 0, 0, 1, 0);
	m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);

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
	
	for(unsigned i = 0; i < GBuffer::GBUFFER_NUM_TEXTURES; ++i)
	{
		m_gBuffer.BindForReading(GL_TEXTURE0 + i, i);
		glUniform1i(m_parameters[U_POSITION_MAP_LIGHTPASS + i], 0 + i);
	}

	float screenSize[2] = {(float)Application::GetWindowWidth(), (float)Application::GetWindowHeight()};
	glUniform2fv(m_parameters[U_SCREEN_SIZE_LIGHTPASS], 1, &screenSize[0]);

	//Point light - local light without shadow
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
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
	}

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
	for(unsigned i = 0; i < 1; ++i)
	{
		if(lights[i].type == LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(lights[i].position.x, lights[i].position.y, lights[i].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + (i * NUM_LIGHT_PARAM)], 1, &lightDirection_cameraspace.x);
		}
		else if(lights[i].type == LIGHT_SPOT)
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
		FPC.position.x +  V3_CamShaker.x, FPC.position.y + V3_CamShaker.y, FPC.position.z +  V3_CamShaker.z,
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
	if(engine != NULL)
	{
		engine->drop();
	}

	while(BIv_BulletList.size() > 0)
	{
		BulletInfo *BI = BIv_BulletList.back();
		delete BI;
		BIv_BulletList.pop_back();
	}

	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}

	while(m_ParList.size() > 0)
	{
		Particle *Par = m_ParList.back();
		delete Par;
		m_ParList.pop_back();
	}

	while(targetBList.size() > 0)
	{
		targetBoard *TB = targetBList.back();
		delete TB;
		targetBList.pop_back();
	}

	if(GOp_Player)
	{
		delete GOp_Player;
		GOp_Player = NULL;
	}

	if(SA_waterSprite)
	{
		delete SA_waterSprite;
		SA_waterSprite = NULL;
	}

	glDeleteProgram(m_gPassShaderID);
	glDeleteProgram(m_gBufferShaderID);
	glDeleteProgram(m_lightShaderID);

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
	glDeleteProgram(m_gPassShaderID);
}