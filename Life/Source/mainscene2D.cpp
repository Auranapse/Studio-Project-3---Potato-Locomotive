/****************************************************************************/
/*!
\file mainscene2D.cpp
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Aperture Science Laboratories Underground
!*/
/****************************************************************************/
#include "mainscene2D.h"
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
mainscene2D Constructor
*/
/******************************************************************************/
mainscene2D::mainscene2D() : TESTMODE(false)
{

}

/******************************************************************************/
/*!
mainscene2D Destructor
*/
/******************************************************************************/
mainscene2D::~mainscene2D()
{

}

/******************************************************************************/
/*!
\brief
Loads save file from file
*/
/******************************************************************************/
void mainscene2D::assignSave(void)
{
	f_fov = static_cast<float>(std::stoi(*SF_1.Data[0]));
	f_mouseSensitivity = static_cast<float>(std::stoi(*SF_1.Data[14]) / 100.f);
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
void mainscene2D::Init()
{
	engine = createIrrKlangDevice(ESOD_AUTO_DETECT, ESEO_MULTI_THREADED | ESEO_LOAD_PLUGINS | ESEO_USE_3D_BUFFERS);
	//Control initialization--------------
	for (unsigned i = 0; i < E_CTRL_TOTAL; ++i)
	{
		us_control[i] = 0;
	}

	f_mouseSensitivity = 1;

	SF_1.init("GameData//playerdata.wtf");
	assignSave();

	InitShaders();

	//Set projection matrix to perspective mode
	editFOV(f_fov);
	f_currentfov = f_fov;
	X_camLimit = 1000.f;
	mainCamera.Set(10.f, 10.0f, 0.f);

	renderAxis = false;

	mouseEnabled = true;
	if (mouseEnabled == false)
	{
		Application::SetMouseinput(Application::GetWindowWidth()*0.5, Application::GetWindowHeight()*0.5);
		Application::SetCursor(false);
	}

	FPScounter = 0.f;

	DisplayInfo = true;

	for (unsigned short i = 0; i < 2; ++i)
	{
		BulletInfo* BI = new BulletInfo();
		BI->setStatus(false);
		BIv_BulletList.push_back(BI);
	}

	//Game physics is optimised for size of 3.0f, try not to change it or physics seem out of proportion, though most things will still work.
	tileSize = 3.0f;

	GOp_Player = new GameObject(GameObject::GO_PLAYER_CHAR);
	GOp_Player->enablePhysics = true;
	GOp_Player->mass = 50;
	GOp_Player->colEnable = true;
	GOp_Player->ColBox.Set(tileSize*0.5f, tileSize, tileSize);
	GOp_Player->scale.Set(tileSize, tileSize, tileSize);
	GOp_Player->pos.Set(0.f, 10.f, 0.f);
	GOp_Player->obMesh = meshList[GEO_CHAR_PLAYER];

	GOp_DoorExit = new GameObject(GameObject::GO_2D_DOOR_EXIT);
	GOp_DoorExit->active = false;
	GOp_DoorExit->enablePhysics = false;
	GOp_DoorExit->colEnable = true;
	GOp_DoorExit->ColBox.Set(tileSize, tileSize, tileSize);
	GOp_DoorExit->scale.Set(tileSize, tileSize, tileSize);
	GOp_DoorExit->pos.Set(0, 0, 0);
	GOp_DoorExit->obMesh = meshList[GEO_DOOR_EXIT];

	gravity_force.Set(0.f, -9.82f*6.5f, 0.f);

	inputDelay = 0.f;
	timer = 0.f;

	ISoundSource* backgroundAmbience;
	backgroundAmbience = engine->addSoundSourceFromFile("GameData//sounds//ambience//gun_ambient.wav");
	backgroundAmbience->setDefaultVolume(0.3f);
	engine->play2D(backgroundAmbience, true);


	soundList[ST_PANEL] = engine->addSoundSourceFromFile("GameData//sounds//other//panel.wav");
	soundList[ST_PANEL]->setDefaultVolume(4.5f);
	soundList[ST_PANEL]->setDefaultMinDistance(100.f);
	soundList[ST_JUMP] = engine->addSoundSourceFromFile("GameData//sounds//other//jump.wav");
	soundList[ST_STEP] = engine->addSoundSourceFromFile("GameData//sounds//other//step1.wav");
	soundList[ST_LAND] = engine->addSoundSourceFromFile("GameData//sounds//other//land.wav");
	soundList[ST_BUZZER] = engine->addSoundSourceFromFile("GameData//sounds//other//buzzer.wav");
	soundList[ST_PORTAL_SHOOT_BLUE] = engine->addSoundSourceFromFile("GameData//sounds//portal//shoot_blue.wav");
	soundList[ST_PORTAL_SHOOT_ORANGE] = engine->addSoundSourceFromFile("GameData//sounds//portal//shoot_orange.wav");

	soundList[ST_PORTAL_OPEN] = engine->addSoundSourceFromFile("GameData//sounds//portal//portal_open.wav");
	soundList[ST_PORTAL_OPEN]->setDefaultVolume(0.5f);

	soundList[ST_PORTAL_RESET] = engine->addSoundSourceFromFile("GameData//sounds//portal//portal_reset.wav");
	soundList[ST_PORTAL_RESET]->setDefaultVolume(0.5f);

	soundList[ST_PORTAL_INVALID_SURFACE] = engine->addSoundSourceFromFile("GameData//sounds//portal//portal_invalid_surface.wav");
	soundList[ST_PORTAL_INVALID_SURFACE]->setDefaultVolume(0.5f);

	soundList[ST_PORTAL_ENTER_PORTAL] = engine->addSoundSourceFromFile("GameData//sounds//portal//portal_enter.wav");
	soundList[ST_PORTAL_ENTER_PORTAL]->setDefaultVolume(0.5f);

	currentLevel = 1;
	loadLevel(currentLevel);

	PORTAL_GUN.BluePortal->scale.Set(tileSize*1.1f, tileSize*1.1f, tileSize*1.1f);
	PORTAL_GUN.OrangePortal->scale = PORTAL_GUN.BluePortal->scale;
	PORTAL_GUN.BluePortal->ColBox.Set(tileSize, tileSize, tileSize);
	PORTAL_GUN.OrangePortal->ColBox = PORTAL_GUN.BluePortal->ColBox;

	PORTAL_GUN.BluePortal->rotationZ = -90;
	PORTAL_GUN.OrangePortal->rotationZ = 90;
	PORTAL_GUN.BluePortal->obMesh = meshList[GEO_PORTAL_BLUE];
	PORTAL_GUN.OrangePortal->obMesh = meshList[GEO_PORTAL_ORANGE];

	PB_B = new GameObject(GameObject::GO_PORTAL_BULLET);
	PB_B->type = GameObject::GO_PORTAL_BULLET;
	PB_B->scale.Set(0.5, 0.5, 0.5);
	PB_B->obMesh = meshList[GEO_PORTAL_BULLET_BLUE];

	PB_O = new GameObject(GameObject::GO_PORTAL_BULLET);
	PB_O->type = GameObject::GO_PORTAL_BULLET;
	PB_O->scale.Set(0.5, 0.5, 0.5);
	PB_O->obMesh = meshList[GEO_PORTAL_BULLET_ORANGE];
}

/******************************************************************************/
/*!
\brief
Initialize shaders
*/
/******************************************************************************/
void mainscene2D::InitShaders()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
	// Init VBO here
	// Set background color to whatever
	//glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

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
	m_programID = LoadShaders("GameData//Shader//comg.vertexshader", "GameData//Shader//comg.fragmentshader");
	// Get a handle for our "colorTexture" uniform
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


	glUniform1i(m_parameters[U_NUMLIGHTS], 0);
	//Main Lighting (VIewing room sign)
	lights[0].type = LIGHT_POINT;
	lights[0].position.Set(100.f, 200.f, -550.f);
	lights[0].color.Set(1.f, 1.f, 1.f);
	lights[0].power = 1.5f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.001f;
	lights[0].kQ = 0.000001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(50.f));
	lights[0].cosInner = cos(Math::DegreeToRadian(20.f));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);
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
	lights[2].position.Set(14.f, 40.f, 0.f);
	lights[2].color.Set(1.f, 0.f, 0.f);
	lights[2].power = 2.f;
	lights[2].kC = 1.f;
	lights[2].kL = 0.3f;
	lights[2].kQ = 0.03f;
	lights[2].cosCutoff = cos(Math::DegreeToRadian(0.3f));
	lights[2].cosInner = cos(Math::DegreeToRadian(0.2f));
	lights[2].exponent = 3.f;
	lights[2].spotDirection.Set(-1.f, 0.f, 0.f);
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

	//Generate meshes------------------------------------------------------------------------
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("AXES", 10000.f, 10000.f, 10000.f);

	meshList[GEO_BACKGROUND] = MeshBuilder::GenerateQuad("Background", Color(1.0f, 1.0f, 1.0f), static_cast<float>(Application::GetWindowWidth() / 2)*0.1f, static_cast<float>(Application::GetWindowHeight() / 2)*0.1f, 1.0f);
	meshList[GEO_BACKGROUND]->textureID[0] = LoadTGA("GameData//2D//background.tga", true, false);

	meshList[GEO_CHAR_PLAYER] = MeshBuilder::GenerateQuad("Player Character", Color(1.0f, 0.f, 0.f), 1.f, 1.f, 1.0f);
	meshList[GEO_CHAR_PLAYER]->textureID[0] = LoadTGA("GameData//2D//player//PLAYER_BENDY.tga", true, true);

	meshList[GEO_PORTAL_GUN_BLUE] = MeshBuilder::GenerateQuad("Player Character", Color(1.0f, 0.f, 0.f), 1.f, 1.f, 1.0f);
	meshList[GEO_PORTAL_GUN_BLUE]->textureID[0] = LoadTGA("GameData//2D//portal//GUN_BLUE.tga", true, false);

	meshList[GEO_CHAR_PLAYER_LIMB] = MeshBuilder::GenerateQuad("Player Character's limbs", Color(0.f, 0.f, 0.f), 0.12f, 0.32f, 1.f);

	meshList[GEO_BLOCK_1_BLACK_PANEL] = MeshBuilder::GenerateQuad("Black panel", Color(1.0f, 0.f, 0.f), 1.f, 1.f, 1.0f);
	meshList[GEO_BLOCK_1_BLACK_PANEL]->textureID[0] = LoadTGA("GameData//2D//blocks//1_BLACK_PANEL.tga", true, false);

	meshList[GEO_BLOCK_2_WHITE_PANEL] = MeshBuilder::GenerateQuad("White panel", Color(1.0f, 0.f, 0.f), 1.f, 1.f, 1.0f);
	meshList[GEO_BLOCK_2_WHITE_PANEL]->textureID[0] = LoadTGA("GameData//2D//blocks//2_WHITE_PANEL.tga", true, false);

	meshList[GEO_DOOR_EXIT] = MeshBuilder::GenerateQuad("Door Exit", Color(1.0f, 0.f, 0.f), 1.f, 1.f, 1.0f);
	meshList[GEO_DOOR_EXIT]->textureID[0] = LoadTGA("GameData//2D//doors//Exit.tga", true, false);

	meshList[GEO_PORTAL_BLUE] = MeshBuilder::GenerateQuad("Blue Portal", Color(1.0f, 0.f, 0.f), 1.f, 1.f, 1.0f);
	meshList[GEO_PORTAL_BLUE]->textureID[0] = LoadTGA("GameData//2D//portal//BLUE.tga", true, false);

	meshList[GEO_PORTAL_ORANGE] = MeshBuilder::GenerateQuad("Orange Portal", Color(1.0f, 0.f, 0.f), 1.f, 1.f, 1.0f);
	meshList[GEO_PORTAL_ORANGE]->textureID[0] = LoadTGA("GameData//2D//portal//ORANGE.tga", true, false);

	meshList[GEO_PORTAL_BULLET_BLUE] = MeshBuilder::GenerateSphere("Blue Portal Projectile", Color(0.f, 0.f, 1.f), 16, 16, 1);
	meshList[GEO_PORTAL_BULLET_ORANGE] = MeshBuilder::GenerateSphere("Orange Portal Projectile", Color(1.f, 0.f, 0.f), 16, 16, 1);
	
	meshList[GEO_T_BUTTON] = MeshBuilder::GenerateQuad("Button", Color(1.0f, 0.f, 0.f), 1.f, 1.f, 1.0f);
	meshList[GEO_T_BUTTON]->textureID[0] = LoadTGA("GameData//2D//triggers//button.tga", true, false);

	meshList[GEO_T_GRILL] = MeshBuilder::GenerateQuad("Grill", Color(1.0f, 0.f, 0.f), 1.f, 1.f, 1.0f);
	meshList[GEO_T_GRILL]->textureID[0] = LoadTGA("GameData//2D//triggers//grill.tga", true, false);

	meshList[GEO_T_LASER] = MeshBuilder::GenerateQuad("Laser", Color(1.0f, 0.f, 0.f), 1.f, 1.f, 1.0f);
	meshList[GEO_T_LASER]->textureID[0] = LoadTGA("GameData//2D//triggers//laser.tga", true, false);

	//---------------------------------------------------------------------------------------
	//Text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID[0] = LoadTGA("GameData//Image//font//inputm.tga", false);
}

/******************************************************************************/
/*!
\brief
Function to load a level
*/
/******************************************************************************/
bool mainscene2D::loadLevel(int level)
{
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

	while (m_goTriggerList.size() > 0)
	{
		GameObjectTrigger *goT = m_goTriggerList.back();
		if (goT != NULL)
		{
			delete goT;
		}
		m_goTriggerList.pop_back();
	}

	while (m_goButtonList.size() > 0)
	{
		GameObjectButton *goB = m_goButtonList.back();
		if (goB != NULL)
		{
			delete goB;
		}
		m_goButtonList.pop_back();
	}

	GOp_DoorExit->active = false;
	GOp_Player->active = false;
	PORTAL_GUN.BP_AttachedTo = NULL;
	PORTAL_GUN.OP_AttachedTo = NULL;
	PORTAL_GUN.BluePortal->active = false;
	PORTAL_GUN.OrangePortal->active = false;

	std::cout << "Map Size: ";
	std::cout << GAME_MAP.map_width << ", " << GAME_MAP.map_height << "\n";

	for (unsigned y = GAME_MAP.map_height - 1; y > 0; --y)
	{
		for (unsigned x = 0; x < GAME_MAP.map_width; ++x)
		{
			if (GAME_MAP.map_data[y][x] == "SPAWN" && !GOp_Player->active)//Generate spawnpoint
			{
				GOp_Player->active = true;
				GOp_Player->pos.Set(x*tileSize*2.f, (GAME_MAP.map_height - y)*tileSize*2.f, 0.f);
			}
			else if (GAME_MAP.map_data[y][x] == "EXIT")//Generate exit
			{
				if (!GOp_DoorExit->active)
				{
					GOp_DoorExit->active = true;
					GOp_DoorExit->pos.Set(x*tileSize*2.f, (GAME_MAP.map_height - y)*tileSize*2.f, -0.1f);
				}
			}
			else if (GAME_MAP.map_data[y][x][0] == 'B')//Check for button
			{
				GameObjectButton *goB;
				goB = FetchGOButton();
				goB->scale.Set(tileSize, tileSize, tileSize);
				goB->ColBox.Set(tileSize*0.5f, tileSize, tileSize);
				goB->pos.Set(x*tileSize*2.f, (GAME_MAP.map_height - y)*tileSize*2.f, 0.5f);
				goB->ID = GAME_MAP.map_data[y][x][1];
				goB->obMesh = meshList[GEO_T_BUTTON];
			}
			else if (GAME_MAP.map_data[y][x][0] == 'T')//Check for trigger
			{
				GameObjectTrigger *goT;
				goT = FetchGOTrigger();
				goT->scale.Set(tileSize, tileSize, tileSize);
				goT->ColBox.Set(tileSize, tileSize, tileSize);
				goT->pos.Set(x*tileSize*2.f, (GAME_MAP.map_height - y)*tileSize*2.f, 0.5f);
				goT->ID = GAME_MAP.map_data[y][x][1];
				goT->TRIGGER_TYPE = GameObjectTrigger::T_GRILL;
				goT->triggerActive = true;
				goT->obMesh = meshList[GEO_T_GRILL];

				if (GAME_MAP.map_data[y][x][2] == 'L')
				{
					goT->TRIGGER_TYPE = GameObjectTrigger::T_LASER;
					goT->obMesh = meshList[GEO_T_LASER];
				}
				
				if (GAME_MAP.map_data[y][x][3] == 'F')
				{
					goT->triggerActive = false;
				}
			}
			else if (GAME_MAP.map_data[y][x] != "0")//Generate the rest of the world
			{
				GameObject *go;
				go = FetchGO();
				go->enablePhysics = false;
				go->colEnable = true;
				go->portalble = true;
				go->scale.Set(tileSize, tileSize, tileSize);
				go->ColBox.Set(tileSize, tileSize, tileSize);
				go->pos.Set(x*tileSize*2.f, (GAME_MAP.map_height - y)*tileSize*2.f, 0.5f);
				go->obMesh = meshList[GEO_BLOCK_1_BLACK_PANEL];//Prevent crashing

				if (GAME_MAP.map_data[y][x] == "1")
				{
					go->type = GameObject::GO_2D_STATIC_BLOCK;
					go->obMesh = meshList[GEO_BLOCK_1_BLACK_PANEL];
					go->portalble = false;
				}
				else if (GAME_MAP.map_data[y][x] == "2")
				{
					go->type = GameObject::GO_2D_STATIC_BLOCK;
					go->obMesh = meshList[GEO_BLOCK_2_WHITE_PANEL];
				}
				go->active = true;
				go->dynamicRendering = true;
			}
		}
	}

	//Generate world borders
	for (unsigned x = 0; x <= GAME_MAP.map_width; ++x)
	{
		GameObject *go;
		go = FetchGO();
		go->enablePhysics = false;
		go->colEnable = true;
		go->portalble = true;
		go->scale.Set(tileSize, tileSize, tileSize);
		go->ColBox.Set(tileSize, tileSize, tileSize);
		go->pos.Set(x*tileSize*2.f - tileSize*2.f, 0.f, 0.5f);
		go->obMesh = meshList[GEO_BLOCK_1_BLACK_PANEL];//Prevent crashing
		go->type = GameObject::GO_2D_STATIC_BLOCK;
		go->portalble = false;
		go->active = true;
		go->dynamicRendering = true;
	}

	for (unsigned x = 0; x <= GAME_MAP.map_width; ++x)
	{
		GameObject *go;
		go = FetchGO();
		go->enablePhysics = false;
		go->colEnable = true;
		go->portalble = true;
		go->scale.Set(tileSize, tileSize, tileSize);
		go->ColBox.Set(tileSize, tileSize, tileSize);
		go->pos.Set(x*tileSize*2.f - tileSize*2.f, GAME_MAP.map_height*tileSize*2.f, 0.5f);
		go->obMesh = meshList[GEO_BLOCK_1_BLACK_PANEL];//Prevent crashing
		go->type = GameObject::GO_2D_STATIC_BLOCK;
		go->portalble = false;
		go->active = true;
		go->dynamicRendering = true;
	}

	for (unsigned y = GAME_MAP.map_height - 1; y > 0; --y)
	{
		GameObject *go;
		go = FetchGO();
		go->enablePhysics = false;
		go->colEnable = true;
		go->portalble = true;
		go->scale.Set(tileSize, tileSize, tileSize);
		go->ColBox.Set(tileSize, tileSize, tileSize);
		go->pos.Set(-tileSize*2.f, y*tileSize*2.f, 0.5f);
		go->obMesh = meshList[GEO_BLOCK_1_BLACK_PANEL];//Prevent crashing
		go->type = GameObject::GO_2D_STATIC_BLOCK;
		go->portalble = false;
		go->active = true;
		go->dynamicRendering = true;
	}

	for (unsigned y = 0; y < GAME_MAP.map_height + 2; ++y)
	{
		GameObject *go;
		go = FetchGO();
		go->enablePhysics = false;
		go->colEnable = true;
		go->portalble = true;
		go->scale.Set(tileSize, tileSize, tileSize);
		go->ColBox.Set(tileSize, tileSize, tileSize);
		go->pos.Set(GAME_MAP.map_width*tileSize*2.f, y*tileSize*2.f - tileSize*2.f, 0.5f);
		go->obMesh = meshList[GEO_BLOCK_1_BLACK_PANEL];//Prevent crashing
		go->type = GameObject::GO_2D_STATIC_BLOCK;
		go->portalble = false;
		go->active = true;
		go->dynamicRendering = true;
	}

	X_camLimit = (GAME_MAP.map_width*tileSize * 2 - ((Application::GetWindowWidth()*0.1f)));

	if (!GOp_Player->active)
	{
		std::cout << "!!! Warning !!! No player spawn location is found. Defaulting to 0, 10, 0\n";
		GOp_Player->active = true;
		GOp_Player->pos.Set(0, 10, 0);
	}

	if (!GOp_DoorExit->active)
	{
		std::cout << "!!! Warning !!! No exit is found. Defaulting to edge of map\n";
		GOp_DoorExit->active = true;
		GOp_DoorExit->pos.Set(GAME_MAP.map_width*tileSize*2.f - tileSize*2.f, tileSize*2.f, -0.1f);
	}

	std::cout << "Map Successfully loaded\n";
	return true;
}

/******************************************************************************/
/*!
\brief
Function to edit fov
*/
/******************************************************************************/
void mainscene2D::editFOV(float newFOV)
{
	Mtx44 proj;
	proj.SetToPerspective(newFOV, static_cast<double>(Application::GetWindowWidth()) / static_cast<double>(Application::GetWindowHeight()), 0.1f, 10000.0f);
	projectionStack.LoadMatrix(proj);
}

/******************************************************************************/
/*!
\brief
Check if colliding in 2D space
*/
/******************************************************************************/
bool mainscene2D::collide2D(Vector3 &Position)
{
	if (GOcollide2D(Position) != NULL)
	{
		return true;
	}

	return false;
}

/******************************************************************************/
/*!
\brief
Check for collision between gameobject
*/
/******************************************************************************/
bool mainscene2D::GOcollide2D(GameObject *go1, GameObject *go2)
{
	if (intersect2D(go2->pos + go2->ColBox, go2->pos - go2->ColBox, go1->pos))
	{
		return true;
	}
	return false;
}

/******************************************************************************/
/*!
\brief
Check if colliding in 2D space with gameobject
*/
/******************************************************************************/
GameObject* mainscene2D::GOcollide2D(Vector3 &Position)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active && go->colEnable && go->pos != Position)
		{
			if (intersect2D(go->pos + go->ColBox + go->ColBoxOffset, go->pos - go->ColBox + go->ColBoxOffset, Position))
			{
				return go;
			}
		}
	}
	return NULL;
}

/******************************************************************************/
/*!
\brief
Check if colliding in 2D space with gameobject
*/
/******************************************************************************/
GameObject* mainscene2D::GOcollide2D(GameObject *go2)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active && go->colEnable && go->pos != go2->pos)
		{
			if (intersect2D(go->pos + go->ColBox + go->ColBoxOffset, go->pos - go->ColBox + go->ColBoxOffset, go2->pos))
			{
				return go;
			}
		}
	}
	return NULL;
}

/******************************************************************************/
/*!
\brief
Fetches new button trigger
*/
/******************************************************************************/
GameObjectTrigger* mainscene2D::FetchGOTrigger(void)
{
	for (std::vector<GameObjectTrigger *>::iterator it = m_goTriggerList.begin(); it != m_goTriggerList.end(); ++it)
	{
		GameObjectTrigger *goT = (GameObjectTrigger *)*it;
		if (!goT->active)
		{
			goT->active = true;
			return goT;
		}
	}
	for (unsigned i = 0; i < 10; ++i)
	{
		GameObjectTrigger *goT = new GameObjectTrigger();
		m_goTriggerList.push_back(goT);
	}
	GameObjectTrigger *goT = m_goTriggerList.back();
	goT->active = true;
	return goT;
}

/******************************************************************************/
/*!
\brief
Fetches new button gameobject
*/
/******************************************************************************/
GameObjectButton* mainscene2D::FetchGOButton(void)
{
	for (std::vector<GameObjectButton *>::iterator it = m_goButtonList.begin(); it != m_goButtonList.end(); ++it)
	{
		GameObjectButton *goB = (GameObjectButton *)*it;
		if (!goB->active)
		{
			goB->active = true;
			return goB;
		}
	}
	for (unsigned i = 0; i < 10; ++i)
	{
		GameObjectButton *goB = new GameObjectButton();
		m_goButtonList.push_back(goB);
	}
	GameObjectButton *goB = m_goButtonList.back();
	goB->active = true;
	return goB;
}

/******************************************************************************/
/*!
\brief
Fetches new gameobject
*/
/******************************************************************************/
GameObject* mainscene2D::FetchGO(void)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			return go;
		}
	}
	for (unsigned i = 0; i < 10; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_BALL);
		m_goList.push_back(go);
	}
	GameObject *go = m_goList.back();
	go->active = true;
	return go;
}

/******************************************************************************/
/*!
\brief
Updates camera position
*/
/******************************************************************************/
void mainscene2D::UpdateCamera(double dt)
{
	//if (mainCamera.x < X_camLimit)
	{
		if (mousePosX - mainCamera.x > Application::GetWindowWidth()*0.08f)
		{
			mainCamera.x += static_cast<float>(dt) * (mousePosX - Application::GetWindowWidth()*0.08f - mainCamera.x) * 4.f;
		}

		if (GOp_Player->pos.x - mainCamera.x > Application::GetWindowWidth()*0.08f)
		{
			mainCamera.x += static_cast<float>(dt) * (GOp_Player->pos.x - Application::GetWindowWidth()*0.08f - mainCamera.x) * 4.f;
		}
	}

	//if (mainCamera.x > -tileSize * 2)
	{
		if (mousePosX - mainCamera.x < Application::GetWindowWidth()*0.02f)
		{
			mainCamera.x += static_cast<float>(dt) * (mousePosX - Application::GetWindowWidth()*0.02f - mainCamera.x) * 4.f;
		}

		if (GOp_Player->pos.x - mainCamera.x < Application::GetWindowWidth()*0.02f)
		{
			mainCamera.x += static_cast<float>(dt) * (GOp_Player->pos.x - Application::GetWindowWidth()*0.02f - mainCamera.x) * 4.f;
		}
	}

	if (mousePosY - mainCamera.y > Application::GetWindowHeight()*0.08f)
	{
		mainCamera.y += static_cast<float>(dt) * (mousePosY - Application::GetWindowHeight()*0.08f - mainCamera.y) * 4.f;
	}

	if (GOp_Player->pos.y - mainCamera.y > Application::GetWindowHeight()*0.08f)
	{
		mainCamera.y += static_cast<float>(dt) * (GOp_Player->pos.y - Application::GetWindowHeight()*0.08f - mainCamera.y) * 4.f;
	}

	if (mousePosY - mainCamera.y < Application::GetWindowHeight()*0.02f)
	{
		mainCamera.y += static_cast<float>(dt) * (mousePosY - Application::GetWindowHeight()*0.02f - mainCamera.y) * 4.f;
	}

	if (GOp_Player->pos.y - mainCamera.y < Application::GetWindowHeight()*0.02f)
	{
		mainCamera.y += static_cast<float>(dt) * (GOp_Player->pos.y - Application::GetWindowHeight()*0.02f - mainCamera.y) * 4.f;
	}
}

/******************************************************************************/
/*!
\brief
Handles character control and physics
*/
/******************************************************************************/
void mainscene2D::UpdateCharacter(double dt)
{
	bool inAir = false;
	float MOVESPEED = 120.f;

	//Player Y Axis collision
	if (collide2D(GOp_Player->pos + Vector3(GOp_Player->ColBox.x*0.8f, GOp_Player->ColBox.y*0.8f, 0)) || collide2D(GOp_Player->pos + Vector3(-GOp_Player->ColBox.x*0.8f, GOp_Player->ColBox.y*0.8f, 0)))
	{
		if (GOp_Player->vel.y > 0)
		{
			GOp_Player->vel.y = 0.f;
		}
	}

	if (collide2D(GOp_Player->pos + Vector3(0, -GOp_Player->ColBox.y *0.9f, 0)))
	{
		GOp_Player->vel.y = 400.f * static_cast<float>(dt);
	}
	else if (collide2D(GOp_Player->pos + Vector3(GOp_Player->ColBox.x*0.8f, -GOp_Player->ColBox.y, 0)) || collide2D(GOp_Player->pos + Vector3(-GOp_Player->ColBox.x*0.8f, -GOp_Player->ColBox.y, 0)))
	{
		if (GOp_Player->vel.y != 0)
		{
			engine->play2D(soundList[ST_LAND]);
			GOp_Player->vel.y = 0.f;
		}
	}
	else
	{
		inAir = true;
		if (GOp_Player->vel.y > -100)//Limit velocity
		{
			GOp_Player->vel += gravity_force * static_cast<float>(dt);
		}
	}

	//Player Movement============================================

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_SPRINT]) && !inAir)
	{
		MOVESPEED *= 2.f;
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_RIGHT]))
	{
		if (!inAir)
		{
			GOp_Player->vel.x += MOVESPEED * static_cast<float>(dt);
		}
		else
		{
			GOp_Player->vel.x += MOVESPEED * static_cast<float>(dt) * 0.1f;
		}
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_LEFT]))
	{
		if (!inAir)
		{
			GOp_Player->vel.x -= MOVESPEED * static_cast<float>(dt);
		}
		else
		{
			GOp_Player->vel.x -= MOVESPEED * static_cast<float>(dt) * 0.1f;
		}
	}


	//Player X Axis collision
	if (collide2D(GOp_Player->pos + Vector3(GOp_Player->ColBox.x, GOp_Player->ColBox.y*0.6f, 0)) || collide2D(GOp_Player->pos + Vector3(GOp_Player->ColBox.x, -GOp_Player->ColBox.y*0.8f, 0)))
	{
		if (collide2D(GOp_Player->pos + Vector3(GOp_Player->ColBox.x*0.9f, 0, 0)))
		{
			GOp_Player->vel.x = -250.f * static_cast<float>(dt);
		}

		else if (GOp_Player->vel.x > 0)
		{
			GOp_Player->vel.x = 0;
		}
	}

	if (collide2D(GOp_Player->pos - Vector3(GOp_Player->ColBox.x, GOp_Player->ColBox.y*0.6f, 0)) || collide2D(GOp_Player->pos - Vector3(GOp_Player->ColBox.x, -GOp_Player->ColBox.y*0.8f, 0)))
	{
		if (collide2D(GOp_Player->pos - Vector3(GOp_Player->ColBox.x*0.9f, 0, 0)))
		{
			GOp_Player->vel.x = 250.f * static_cast<float>(dt);
		}

		else if (GOp_Player->vel.x < 0)
		{
			GOp_Player->vel.x = 0;
		}
	}

	//JUMP
	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_JUMP]))
	{
		if (!inAir)
		{
			GOp_Player->vel.y = 30;
			engine->play2D(soundList[ST_JUMP]);
		}
	}

	//Player slowing down
	if (!inAir)
	{
		if (GOp_Player->vel.x != 0)
		{
			float SForceX = 0 - GOp_Player->vel.x;
			GOp_Player->vel.x += SForceX * static_cast<float>(dt) * 8.0f;
		}
	}
	else
	{
		if (GOp_Player->vel.x != 0)
		{
			float SForceX = 0 - GOp_Player->vel.x;
			GOp_Player->vel.x += SForceX * static_cast<float>(dt) *0.9f;
		}
	}

	//Interaction
	static bool isInteractPressed = false;
	if (Application::IsKeyPressed(us_control[E_CTRL_INTERACT]) && !isInteractPressed)
	{
		isInteractPressed = true;
		if (GOcollide2D(GOp_Player, GOp_DoorExit))
		{
			++currentLevel;
			loadLevel(currentLevel);
		}

		for (std::vector<GameObjectButton *>::iterator it = m_goButtonList.begin(); it != m_goButtonList.end(); ++it)
		{
			GameObjectButton *goB = (GameObjectButton *)*it;
			if (goB->active)
			{
				if (GOcollide2D(GOp_Player, goB))
				{
					goB->Triggered = true;
				}
			}
		}
	}
	else if (!Application::IsKeyPressed(us_control[E_CTRL_INTERACT]) && isInteractPressed)
	{
		isInteractPressed = false;
	}

	A4_Player.Update(dt, GOp_Player->vel.x);
	GOp_Player->pos += GOp_Player->vel * static_cast<float>(dt);
}

/******************************************************************************/
/*!
\brief
Handles portal gun
*/
/******************************************************************************/
void mainscene2D::UpdatePortalGun(double dt)
{
	if (PORTAL_GUN.BluePortal->active && PORTAL_GUN.OrangePortal->active)
	{

		Mtx44 rotationOP, rotationBP;
		rotationOP.SetToRotation(PORTAL_GUN.OrangePortal->rotationZ, 0, 0, 1);
		rotationBP.SetToRotation(PORTAL_GUN.BluePortal->rotationZ, 0, 0, 1);
		PORTAL_GUN.OrangePortal->ColBoxOffset.Set(0, tileSize*1.8f, 0);
		PORTAL_GUN.BluePortal->ColBoxOffset = PORTAL_GUN.OrangePortal->ColBoxOffset;
		PORTAL_GUN.OrangePortal->ColBoxOffset = rotationOP * PORTAL_GUN.OrangePortal->ColBoxOffset;
		PORTAL_GUN.BluePortal->ColBoxOffset = rotationBP * PORTAL_GUN.BluePortal->ColBoxOffset;

		if (intersect2D(PORTAL_GUN.OrangePortal->pos + PORTAL_GUN.OrangePortal->ColBox*0.8f + PORTAL_GUN.OrangePortal->ColBoxOffset, PORTAL_GUN.OrangePortal->pos - PORTAL_GUN.OrangePortal->ColBox*0.8f + PORTAL_GUN.OrangePortal->ColBoxOffset, GOp_Player->pos))
		{
			PORTAL_GUN.OP_AttachedTo->colEnable = false;
		}
		else
		{
			PORTAL_GUN.OP_AttachedTo->colEnable = true;
		}

		if (intersect2D(PORTAL_GUN.BluePortal->pos + PORTAL_GUN.BluePortal->ColBox*0.8f + PORTAL_GUN.BluePortal->ColBoxOffset, PORTAL_GUN.BluePortal->pos - PORTAL_GUN.BluePortal->ColBox*0.8f + PORTAL_GUN.BluePortal->ColBoxOffset, GOp_Player->pos))
		{
			PORTAL_GUN.BP_AttachedTo->colEnable = false;
		}
		else
		{
			PORTAL_GUN.BP_AttachedTo->colEnable = true;
		}


		if (intersect2D(PORTAL_GUN.BluePortal->pos + PORTAL_GUN.BluePortal->ColBox, PORTAL_GUN.BluePortal->pos - PORTAL_GUN.BluePortal->ColBox, GOp_Player->pos))
		{
			if (PORTAL_GUN.BluePortal->colEnable)
			{
				PORTAL_GUN.enterPortal(GOp_Player, false);
				PORTAL_GUN.OrangePortal->colEnable = false;
				PORTAL_GUN.OP_AttachedTo->colEnable = false;
				engine->play2D(soundList[ST_PORTAL_ENTER_PORTAL]);
			}
		}
		else
		{
			PORTAL_GUN.BluePortal->colEnable = true;
		}

		if (intersect2D(PORTAL_GUN.OrangePortal->pos + PORTAL_GUN.OrangePortal->ColBox, PORTAL_GUN.OrangePortal->pos - PORTAL_GUN.OrangePortal->ColBox, GOp_Player->pos))
		{
			if (PORTAL_GUN.OrangePortal->colEnable)
			{
				PORTAL_GUN.enterPortal(GOp_Player, true);
				PORTAL_GUN.BluePortal->colEnable = false;
				PORTAL_GUN.BP_AttachedTo->colEnable = false;
				engine->play2D(soundList[ST_PORTAL_ENTER_PORTAL]);
			}
		}
		else
		{
			PORTAL_GUN.OrangePortal->colEnable = true;
		}
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_RELOAD]))
	{
		resetPortalGun();
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_ATTACK]) && !PB_B->active)
	{
		PB_B->active = true;
		PB_B->pos = GOp_Player->pos;
		PB_B->vel = (Vector3(mousePosX, mousePosY, 0) - GOp_Player->pos).Normalized() * 80.f;
		engine->play2D(soundList[ST_PORTAL_SHOOT_BLUE]);
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_AIM]) && !PB_O->active)
	{
		PB_O->active = true;
		PB_O->pos = GOp_Player->pos;
		PB_O->vel = (Vector3(mousePosX, mousePosY, 0) - GOp_Player->pos).Normalized() * 80.f;
		engine->play2D(soundList[ST_PORTAL_SHOOT_ORANGE]);
	}

	if (PB_B->active)
	{
		GameObject *bcolc = GOcollide2D(PB_B);
		if (bcolc)
		{
			PB_B->active = false;
			if (bcolc->portalble && bcolc != PORTAL_GUN.OP_AttachedTo)
			{
				if (PORTAL_GUN.BP_AttachedTo)
				{
					PORTAL_GUN.BP_AttachedTo->colEnable = true;
				}
				engine->play2D(soundList[ST_PORTAL_OPEN]);
				Vector3 temp = PB_B->pos - bcolc->pos;
				static bool XN = false;
				static bool YN = false;

				if (temp.x < 0)
				{
					temp.x = -temp.x;
					XN = true;
				}
				else
				{
					XN = false;
				}
				if (temp.y < 0)
				{
					temp.y = -temp.y;
					YN = true;
				}
				else
				{
					YN = false;
				}

				if (temp.x > temp.y)
				{
					if (XN)
					{
						PORTAL_GUN.BluePortal->rotationZ = 90.f;
					}
					else
					{
						PORTAL_GUN.BluePortal->rotationZ = -90.f;
					}
				}
				else
				{
					if (YN)
					{
						PORTAL_GUN.BluePortal->rotationZ = 180.f;
					}
					else
					{
						PORTAL_GUN.BluePortal->rotationZ = 0.f;
					}
				}

				PORTAL_GUN.BluePortal->active = true;
				PORTAL_GUN.BluePortal->pos = bcolc->pos;

				PORTAL_GUN.BP_AttachedTo = bcolc;
			}
			else
			{
				engine->play2D(soundList[ST_PORTAL_INVALID_SURFACE]);
			}
		}
		else
		{
			PB_B->pos += PB_B->vel * static_cast<float>(dt);
		}
	}

	if (PB_O->active)
	{
		GameObject *bcolc = GOcollide2D(PB_O);
		if (bcolc)
		{
			PB_O->active = false;
			if (bcolc->portalble && bcolc != PORTAL_GUN.BP_AttachedTo)
			{
				if (PORTAL_GUN.OP_AttachedTo)
				{
					PORTAL_GUN.OP_AttachedTo->colEnable = true;
				}
				engine->play2D(soundList[ST_PORTAL_OPEN]);
				Vector3 temp = PB_O->pos - bcolc->pos;

				static bool XN = false;
				static bool YN = false;

				if (temp.x < 0)
				{
					temp.x = -temp.x;
					XN = true;
				}
				else
				{
					XN = false;
				}
				if (temp.y < 0)
				{
					temp.y = -temp.y;
					YN = true;
				}
				else
				{
					YN = false;
				}

				if (temp.x > temp.y)
				{
					if (XN)
					{
						PORTAL_GUN.OrangePortal->rotationZ = 90.f;
					}
					else
					{
						PORTAL_GUN.OrangePortal->rotationZ = -90.f;
					}
				}
				else
				{
					if (YN)
					{
						PORTAL_GUN.OrangePortal->rotationZ = 180.f;
					}
					else
					{
						PORTAL_GUN.OrangePortal->rotationZ = 0.f;
					}
				}

				PORTAL_GUN.OrangePortal->active = true;
				PORTAL_GUN.OrangePortal->pos = bcolc->pos;

				PORTAL_GUN.OP_AttachedTo = bcolc;
			}
			else
			{
				engine->play2D(soundList[ST_PORTAL_INVALID_SURFACE]);
			}
		}
		else
		{
			PB_O->pos += PB_O->vel * static_cast<float>(dt);
		}
	}
}

/******************************************************************************/
/*!
\brief
Resets the portal gun portals
*/
/******************************************************************************/
void mainscene2D::resetPortalGun(void)
{
	if (PORTAL_GUN.BluePortal->active || PORTAL_GUN.OrangePortal->active || PB_B->active || PB_O->active)
	{
		PB_B->active = false;
		PB_O->active = false;
		PORTAL_GUN.BluePortal->active = false;
		PORTAL_GUN.OrangePortal->active = false;
		if (PORTAL_GUN.BP_AttachedTo != NULL)
		{
			PORTAL_GUN.BP_AttachedTo->colEnable = true;
			PORTAL_GUN.BP_AttachedTo = NULL;
		}

		if (PORTAL_GUN.OP_AttachedTo != NULL)
		{
			PORTAL_GUN.OP_AttachedTo->colEnable = true;
			PORTAL_GUN.OP_AttachedTo = NULL;
		}

		engine->play2D(soundList[ST_PORTAL_RESET]);
	}
}

/******************************************************************************/
/*!
\brief
Handles trigger effects
*/
/******************************************************************************/
void mainscene2D::UpdateGOTriggers(double dt)
{	
	for (std::vector<GameObjectTrigger *>::iterator it = m_goTriggerList.begin(); it != m_goTriggerList.end(); ++it)
	{
		GameObjectTrigger *goT = (GameObjectTrigger *)*it;
		if (goT->active && goT->triggerActive)
		{
			if (GOcollide2D(GOp_Player, goT))
			{
				switch (goT->TRIGGER_TYPE)
				{
				case GameObjectTrigger::T_GRILL:
				{
					resetPortalGun();
					break;
				}
				case GameObjectTrigger::T_LASER:
				{
					loadLevel(currentLevel);
					break;
				}
				}
				break;
			}
		}
	}
}

/******************************************************************************/
/*!
\brief
Handles button triggers for button
*/
/******************************************************************************/
void mainscene2D::UpdateGOButton(double dt)
{
	for (std::vector<GameObjectButton *>::iterator it = m_goButtonList.begin(); it != m_goButtonList.end(); ++it)
	{
		GameObjectButton *goB = (GameObjectButton *)*it;
		if (goB->active)
		{
			if (goB->Triggered)
			{
				for (std::vector<GameObjectTrigger *>::iterator it = m_goTriggerList.begin(); it != m_goTriggerList.end(); ++it)
				{
					GameObjectTrigger *goT = (GameObjectTrigger *)*it;
					if (goT->active)
					{
						if (goB->ID == goT->ID)
						{
							if (goT->triggerActive)
							{
								goT->triggerActive = false;
							}
							else
							{
								goT->triggerActive = true;
							}
						}
					}
				}

				goB->Triggered = false;
			}
		}
	}
}

/******************************************************************************/
/*!
\brief
Handles game object physics
*/
/******************************************************************************/
void mainscene2D::UpdateGO(double dt)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active && go->enablePhysics)
		{
			{
				go->vel += gravity_force * static_cast<float>(dt);
			}
		}
		go->pos += go->vel * static_cast<float>(dt);
	}
}

/******************************************************************************/
/*!
\brief
Fires bullet
*/
/******************************************************************************/
void mainscene2D::Shoot(Vector3 Pos, Vector3 Dir, float Speed, float Longevity, float dmg)
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
update player sound position
*/
/******************************************************************************/
void mainscene2D::UpdateSound(double dt)
{
	engine->setListenerPosition(vec3df(GOp_Player->pos.x, GOp_Player->pos.y, GOp_Player->pos.z), vec3df(1, 0, 0), vec3df(0, 0, 0), vec3df(0, 1, 0));
}

/******************************************************************************/
/*!
\brief
Animations, controls
*/
/******************************************************************************/
void mainscene2D::Update(double dt)
{
	timer += static_cast<float>(dt);
	double x, y;
	Application::GetMousePos(x, y);
	mousePosX = (static_cast<float>(x) / static_cast<float>(Application::GetWindowWidth()) * static_cast<float>(Application::GetWindowWidth())*0.1f) + mainCamera.x;
	mousePosY = ((static_cast<float>(Application::GetWindowHeight()) - static_cast<float>(y)) / static_cast<float>(Application::GetWindowHeight()) * static_cast<float>(Application::GetWindowHeight())*0.1f) + mainCamera.y;

	FPScounter = static_cast<float>(1 / dt);

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

	UpdateCharacter(dt);
	UpdatePortalGun(dt);
	UpdateGOTriggers(dt);
	UpdateGOButton(dt);
	UpdateGO(dt);
	UpdateCamera(dt);

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

	//UpdateSound(dt);

	if (TESTMODE)
	{
		if (Application::IsKeyPressed(VK_F3))
		{

		}
	}
}

/******************************************************************************/
/*!
\brief
Render another object if object/player is in between portals
*/
/******************************************************************************/
void mainscene2D::RenderPortalEffect(GameObject *go)
{
	bool o2b = true;

	if (o2b)
	{
		Mtx44 rotation;
		Vector3 objectOffset = PORTAL_GUN.OrangePortal->pos - go->pos;

		rotation.SetToRotation(PORTAL_GUN.BluePortal->rotationZ - PORTAL_GUN.OrangePortal->rotationZ, 0, 0, 1);
		objectOffset = PORTAL_GUN.BluePortal->pos + rotation*(GOp_Player->pos - PORTAL_GUN.OrangePortal->pos);

		PORTAL_GUN.pVelChange(objectOffset, o2b);
		modelStack.PushMatrix();
		modelStack.Translate(PORTAL_GUN.BluePortal->pos);
		modelStack.Rotate(PORTAL_GUN.BluePortal->rotationZ - PORTAL_GUN.OrangePortal->rotationZ, 0, 0, 1);
		modelStack.Translate(objectOffset);
		modelStack.Rotate(PORTAL_GUN.BluePortal->rotationZ - PORTAL_GUN.OrangePortal->rotationZ, 0, 0, 1);
		modelStack.Rotate(go->rotationZ, 0, 0, 1);
		modelStack.Scale(go->scale);
		RenderMeshin2D(go->obMesh, false);
		modelStack.PopMatrix();
	}
	else
	{
		Mtx44 rotation;
		Vector3 objectOffset = PORTAL_GUN.BluePortal->pos - go->pos;

		rotation.SetToRotation(PORTAL_GUN.OrangePortal->rotationZ - PORTAL_GUN.BluePortal->rotationZ, 0, 0, 1);
		objectOffset = PORTAL_GUN.OrangePortal->pos + rotation*(GOp_Player->pos - PORTAL_GUN.BluePortal->pos);

		PORTAL_GUN.pVelChange(objectOffset, !o2b);
		modelStack.PushMatrix();
		modelStack.Translate(PORTAL_GUN.OrangePortal->pos);
		modelStack.Rotate(PORTAL_GUN.OrangePortal->rotationZ - PORTAL_GUN.BluePortal->rotationZ, 0, 0, 1);
		modelStack.Translate(objectOffset);
		modelStack.Rotate(PORTAL_GUN.OrangePortal->rotationZ - PORTAL_GUN.BluePortal->rotationZ, 0, 0, 1);
		modelStack.Rotate(go->rotationZ, 0, 0, 1);
		modelStack.Scale(go->scale);
		RenderMeshin2D(go->obMesh, false);
		modelStack.PopMatrix();
	}
}

/******************************************************************************/
/*!
\brief
Rendering of game triggers
*/
/******************************************************************************/
void mainscene2D::RenderGOTriggers(void)
{
	for (std::vector<GameObjectTrigger *>::iterator it = m_goTriggerList.begin(); it != m_goTriggerList.end(); ++it)
	{
		GameObjectTrigger *goT = (GameObjectTrigger *)*it;
		if (goT->active && goT->triggerActive)
		{
			if (goT->obMesh != NULL)
			{
				modelStack.PushMatrix();
				modelStack.Translate(goT->pos);
				modelStack.Rotate(goT->rotationZ, 0, 0, 1);
				modelStack.Scale(goT->scale);
				RenderMeshin2D(goT->obMesh, false);
				modelStack.PopMatrix();
			}
		}
	}
}

/******************************************************************************/
/*!
\brief
Rendering of game buttons
*/
/******************************************************************************/
void mainscene2D::RenderGOButtons(void)
{
	for (std::vector<GameObjectButton *>::iterator it = m_goButtonList.begin(); it != m_goButtonList.end(); ++it)
	{
		GameObjectButton *goB = (GameObjectButton *)*it;
		if (goB->active)
		{
			if (goB->obMesh != NULL)
			{
				modelStack.PushMatrix();
				modelStack.Translate(goB->pos);
				modelStack.Rotate(goB->rotationZ, 0, 0, 1);
				modelStack.Scale(goB->scale);
				RenderMeshin2D(goB->obMesh, false);
				modelStack.PopMatrix();
			}
		}
	}
}

/******************************************************************************/
/*!
\brief
Rendering of game objects
*/
/******************************************************************************/
void mainscene2D::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_PLAYER_CHAR:
	{
		RenderBendy();
		break;
	}
	default:
	{
		if (go->obMesh != NULL)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos);
			modelStack.Rotate(go->rotationZ, 0, 0, 1);
			modelStack.Scale(go->scale);
			RenderMeshin2D(go->obMesh, false);
			modelStack.PopMatrix();
		}
	}
	}
}

/******************************************************************************/
/*!
\brief
Render player character, bendy
*/
/******************************************************************************/
void mainscene2D::RenderBendy(void)
{
	modelStack.PushMatrix();
	modelStack.Translate(GOp_Player->pos);
	modelStack.Rotate(GOp_Player->rotationX, 1, 0, 0);
	modelStack.Rotate(GOp_Player->rotationY, 0, 1, 0);
	modelStack.Rotate(GOp_Player->rotationZ, 0, 0, 1);

	modelStack.Scale(GOp_Player->scale*0.8f);

	modelStack.PushMatrix();//Right Leg
	modelStack.Translate(0.2f, -0.5f, 0);
	modelStack.Rotate(A4_Player.RIGHT_LEG, 0, 0, 1);
	modelStack.Translate(0, -0.4f, 0);
	RenderMeshin2D(meshList[GEO_CHAR_PLAYER_LIMB], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();//Left Leg
	modelStack.Translate(-0.2f, -0.5f, 0);
	modelStack.Rotate(A4_Player.LEFT_LEG, 0, 0, 1);
	modelStack.Translate(0, -0.4f, 0);
	RenderMeshin2D(meshList[GEO_CHAR_PLAYER_LIMB], false);
	modelStack.PopMatrix();

	RenderMeshin2D(meshList[GEO_CHAR_PLAYER], false);

	modelStack.PushMatrix();//Arms
	modelStack.Translate(0, -0.1f, 0);
	modelStack.Rotate(CalAnglefromPosition2D(Vector3(mousePosX, mousePosY), GOp_Player->pos) - 90.f, 0, 0, 1);
	modelStack.Translate(0, 0.4f, 0);
	RenderMeshin2D(meshList[GEO_CHAR_PLAYER_LIMB], false);

	modelStack.Translate(0, 0.3f, 0.1f);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	RenderMeshin2D(meshList[GEO_PORTAL_GUN_BLUE], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Rendering of bullets
*/
/******************************************************************************/
void mainscene2D::RenderBullet(void)
{
	for (std::vector<BulletInfo *>::iterator it = BIv_BulletList.begin(); it != BIv_BulletList.end(); ++it)
	{
		BulletInfo *BI = (BulletInfo *)*it;
		if (BI->getStatus() && BI->getLife() < 5.95)
		{
			modelStack.PushMatrix();
			modelStack.Translate(BI->getPosition());
			//RenderMesh(meshList[GEO_BULLET], false);
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
void mainscene2D::RenderMeshin2D(Mesh *mesh, bool enableLight, float size, float x, float y, float rotation)
{
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
void mainscene2D::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);

		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

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
	glBindTexture(GL_TEXTURE_2D, 0);
}

/******************************************************************************/
/*!
\brief
Renders text
*/
/******************************************************************************/
void mainscene2D::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID[0] <= 0) //Proper error check
		return;

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
void mainscene2D::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID[0] <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

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
Renders the scene
*/
/******************************************************************************/
void mainscene2D::Render(void)
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (lights[0].type == LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[0].type == LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (lights[1].type == LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[1].type == LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (lights[2].type == LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[2].position.x, lights[2].position.y, lights[2].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[2].type == LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[2].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (lights[3].type == LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[3].position.x, lights[3].position.y, lights[3].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[3].type == LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[3].position;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[3].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[3].position;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
	}

	viewStack.LoadIdentity();

	viewStack.LookAt(
		0, 0, 1,
		0, 0, 0,
		0, 1, 0
		);
	modelStack.LoadIdentity();

	if (renderAxis)
	{
		modelStack.PushMatrix();
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_AXES], false);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(-mainCamera*0.1f);
	modelStack.Translate(static_cast<float>(Application::GetWindowWidth())*0.05f, static_cast<float>(Application::GetWindowHeight())*0.05f, -5.0f);
	modelStack.Scale(2, 2, 2);
	RenderMeshin2D(meshList[GEO_BACKGROUND], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();//=========================================================================================================================================================================

	modelStack.Translate(-mainCamera);

	RenderGO(GOp_DoorExit);
	RenderGO(GOp_Player);

	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if (go->pos.x + tileSize > mainCamera.x && go->pos.x - tileSize < mainCamera.x + Application::GetWindowWidth()*0.1f)//Only render if in screen
			{
				if (go->pos.y + tileSize > mainCamera.y && go->pos.y - tileSize < mainCamera.y + Application::GetWindowHeight()*0.1f)//Only render if in screen
				{
					RenderGO(go);
				}
			}
		}
	}

	if (PORTAL_GUN.BluePortal->active)
	{
		RenderGO(PORTAL_GUN.BluePortal);
	}
	if (PORTAL_GUN.OrangePortal->active)
	{
		RenderGO(PORTAL_GUN.OrangePortal);
	}
	if (PB_B->active)
	{
		RenderGO(PB_B);
	}
	if (PB_O->active)
	{
		RenderGO(PB_O);
	}

	RenderGOButtons();
	RenderGOTriggers();

	/*modelStack.PushMatrix();
	modelStack.Translate(mousePosX, mousePosY, 0);
	RenderMeshin2D(meshList[GEO_CHAR_PLAYER], false);
	modelStack.PopMatrix();//*/

	modelStack.PopMatrix();//==========================================================================================================================================================================

	if (DisplayInfo == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(static_cast<long double>(FPScounter)), Color(0, 1, 1), 2, 1, 2);
	}
}

/******************************************************************************/
/*!
\brief
Clears memory upon exit
*/
/******************************************************************************/
void mainscene2D::Exit(void)
{
	if (engine != NULL)
	{
		engine->drop();
	}

	if (GOp_Player != NULL)
	{
		delete GOp_Player;
	}

	if (PB_B != NULL)
	{
		delete PB_B;
	}
	if (PB_O != NULL)
	{
		delete PB_O;
	}
	if (GOp_DoorExit != NULL)
	{
		delete GOp_DoorExit;
	}

	for (unsigned i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
		{
			delete meshList[i];
		}
	}

	while (BIv_BulletList.size() > 0)
	{
		BulletInfo *BI = BIv_BulletList.back();
		delete BI;
		BIv_BulletList.pop_back();
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

	while (m_goTriggerList.size() > 0)
	{
		GameObjectTrigger *goT = m_goTriggerList.back();
		if (goT != NULL)
		{
			delete goT;
		}
		m_goTriggerList.pop_back();
	}

	while (m_goButtonList.size() > 0)
	{
		GameObjectButton *goB = m_goButtonList.back();
		if (goB != NULL)
		{
			delete goB;
		}
		m_goButtonList.pop_back();
	}

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}