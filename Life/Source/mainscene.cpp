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
//#include <vld.h>

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
\param save
if true will overwrite files, if not will load
*/
/******************************************************************************/
void mainscene::assignSave(bool save)
{
	SH_1.assign(f_fov, 70.f, 1, save);
	if (save)
	{
		f_mouseSensitivity *= 100.f;
	}

	SH_1.assign(f_mouseSensitivity, 100.f, 2, save);
	f_mouseSensitivity *= 0.01f;
	SH_1.assign(us_control[E_CTRL_MOVE_FRONT], 'W', 3, save);
	SH_1.assign(us_control[E_CTRL_MOVE_BACK], 'S', 4, save);
	SH_1.assign(us_control[E_CTRL_MOVE_LEFT], 'A', 5, save);
	SH_1.assign(us_control[E_CTRL_MOVE_RIGHT], 'D', 6, save);
	SH_1.assign(us_control[E_CTRL_MOVE_SPRINT], VK_SHIFT, 7, save);
	SH_1.assign(us_control[E_CTRL_MOVE_WALK], VK_CONTROL, 8, save);
	SH_1.assign(us_control[E_CTRL_MOVE_JUMP], VK_SPACE, 9, save);
	SH_1.assign(us_control[E_CTRL_INTERACT], 'E', 10, save);
	SH_1.assign(us_control[E_CTRL_THROW], VK_RBUTTON, 11, save);
	SH_1.assign(us_control[E_CTRL_ATTACK], VK_LBUTTON, 12, save);
	SH_1.assign(us_control[E_CTRL_AIM], VK_MBUTTON, 13, save);
	SH_1.assign(us_control[E_CTRL_ABILITY_1], 'V', 14, save);
	SH_1.assign(us_control[E_CTRL_ABILITY_2], 'B', 15, save);
	SH_1.assign(Graphics, GRA_MAX, 16, save);
	SH_1.assign(currentLevel, 1, 17, save);

	SH_1.saveData();
}

/******************************************************************************/
/*!
\brief
Initialize menus
*/
/******************************************************************************/
void mainscene::InitMenus(void)
{
	UIColor.Set(0.9f, 0.9f, 0.9f);
	UIColorPressed.Set(0.5f, 0.5f, 0.5f);

	//Pause Menu--------------------------------------------------------
	TextButton* S_MB;

	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.022f, Application::GetWindowHeight()*0.05f, 0.1f);
	S_MB->scale.Set(2, 2, 2);
	S_MB->text = "Back to game";
	S_MB->gamestate = GS_PAUSED;
	v_buttonList.push_back(S_MB);

	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.022f, Application::GetWindowHeight()*0.05f - 4.f, 0.1f);
	S_MB->scale.Set(2, 2, 2);
	S_MB->text = "Previous level";
	S_MB->gamestate = GS_PAUSED;
	v_buttonList.push_back(S_MB);

	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.022f, Application::GetWindowHeight()*0.05f - 8.f, 0.1f);
	S_MB->scale.Set(2, 2, 2);
	S_MB->text = "Skip to next level";
	S_MB->gamestate = GS_PAUSED;
	v_buttonList.push_back(S_MB);

	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.022f, Application::GetWindowHeight()*0.05f - 12.f, 0.1f);
	S_MB->scale.Set(2, 2, 2);
	S_MB->text = "Return to menu";
	S_MB->gamestate = GS_PAUSED;
	v_buttonList.push_back(S_MB);

	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.022f, Application::GetWindowHeight()*0.05f - 16.f, 0.1f);
	S_MB->scale.Set(2, 2, 2);
	S_MB->text = "Quit";
	S_MB->gamestate = GS_PAUSED;
	v_buttonList.push_back(S_MB);

	//End Menu----------------------------------------------------------
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.022f, Application::GetWindowHeight()*0.05f, 0.1f);
	S_MB->scale.Set(2, 2, 2);
	S_MB->text = "Return to menu";
	S_MB->gamestate = GS_END;
	v_buttonList.push_back(S_MB);

	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.022f, Application::GetWindowHeight()*0.05f - 4.f, 0.1f);
	S_MB->scale.Set(2, 2, 2);
	S_MB->text = "Play again";
	S_MB->gamestate = GS_END;
	v_buttonList.push_back(S_MB);

	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.022f, Application::GetWindowHeight()*0.05f - 8.f, 0.1f);
	S_MB->scale.Set(2, 2, 2);
	S_MB->text = "Quit";
	S_MB->gamestate = GS_END;
	v_buttonList.push_back(S_MB);

	//Death Menu--------------------------------------------------------
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.022f, Application::GetWindowHeight()*0.05f, 0.1f);
	S_MB->scale.Set(2, 2, 2);
	S_MB->text = "Try again";
	S_MB->gamestate = GS_DEATH;
	v_buttonList.push_back(S_MB);

	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.022f, Application::GetWindowHeight()*0.05f - 4.f, 0.1f);
	S_MB->scale.Set(2, 2, 2);
	S_MB->text = "Return to menu";
	S_MB->gamestate = GS_DEATH;
	v_buttonList.push_back(S_MB);

	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.022f, Application::GetWindowHeight()*0.05f - 8.f, 0.1f);
	S_MB->scale.Set(2, 2, 2);
	S_MB->text = "Quit";
	S_MB->gamestate = GS_DEATH;
	v_buttonList.push_back(S_MB);
}

/******************************************************************************/
/*!
\brief
Fetches a button with the same name
\param name
the name of the button
\return
returns the button with the same name
*/
/******************************************************************************/
TextButton* mainscene::FetchBUTTON(std::string name)
{
	for (std::vector<TextButton*>::iterator it = v_buttonList.begin(); it != v_buttonList.end(); ++it)
	{
		TextButton *S_MB = (TextButton *)*it;
		if (S_MB->text == name && S_MB->gamestate == GAMESTATE)
		{
			return S_MB;
		}
	}

	return NULL;
}

/******************************************************************************/
/*!
\brief
Initialize default variables, create meshes, lighting
*/
/******************************************************************************/
void mainscene::Init()
{
	SE_Engine.Init();
	Graphics = GRA_MAX;
	currentLevel = 1;

	//Control initialization--------------
	for (unsigned i = 0; i < E_CTRL_TOTAL; ++i)
	{
		us_control[i] = 0;
	}

	for (unsigned i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	f_fov = 70.f;
	f_mouseSensitivity = 1.f;

	SH_1.init("GameData//ImportantData.GoddamnitQuen");
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

	switch (Graphics)
	{
	case GRA_MAX:
		m_lightDepthFBO.Init(8192, 8192);
		break;
	case GRA_MEDIUM:
		m_lightDepthFBO.Init(4096, 4096);
		break;
	case GRA_LOW:
		m_lightDepthFBO.Init(2048, 2048);
		break;
	case GRA_SHIT:
		m_lightDepthFBO.Init(16, 16);
		break;
	default:
		m_lightDepthFBO.Init(2048, 2048);
		break;
	}

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	lights[0].type = LIGHT_POINT;
	lights[0].position.Set(10.f, 100.f, 0.f);
	lights[0].color.Set(.85f, .92f, 1.f);
	lights[0].kC = 0.f;
	lights[0].kL = 0.f;
	lights[0].kQ = 0.f;
	lights[0].power = 1.f;


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
	m_parameters[U_LENS_EFFECT] = glGetUniformLocation(m_programID, "lenseffect");

	glUniform1i(m_parameters[U_LENS_EFFECT], static_cast<GLint>(0));

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

	if (Graphics <= GRA_MEDIUM)
	{
		enableFOG = true;
	}
	else
	{
		enableFOG = false;
	}

	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &FogCol.r);
	glUniform1i(m_parameters[U_FOG_START], static_cast<GLint>(f_start));
	glUniform1i(m_parameters[U_FOG_END], static_cast<GLint>(f_end));
	glUniform1i(m_parameters[U_FOG_DENSITY], static_cast<GLint>(f_density));
	glUniform1i(m_parameters[U_FOG_TYPE], static_cast<GLint>(f_type));
	glUniform1i(m_parameters[U_FOG_ENABLED], enableFOG);

	//Set projection matrix to perspective mode
	editFOV(f_fov);
	f_targetfov = f_fov;
	f_defaultfov = f_fov;

	//Generate meshes------------------------------------------------------------------------
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("AXES", 10000.f, 10000.f, 10000.f);
	meshList[GEO_REDLINE] = MeshBuilder::GenerateLine("Red Line", Color(1, 0, 0), 0.f, 0.f, 1.f);
	meshList[GEO_GREENLINE] = MeshBuilder::GenerateLine("Green Line", Color(0, 1, 0), 0.f, 0.f, 1.f);
	meshList[GEO_BLUELINE] = MeshBuilder::GenerateLine("Blue Line", Color(0, 0, 1), 0.f, 0.f, 1.f);
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("Crosshair", Color(0.f, 1.f, 1.f), 0.1f, 0.5f, 1.f);

	meshList[GEO_WORLD_CUBE] = MeshBuilder::GenerateCubeT2("World Cube", Color(1.f, 1.f, 1.f), 1, 1, 1);
	meshList[GEO_WORLD_CUBE]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_WORLD_CUBE]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_WORLD_CUBE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WORLD_CUBE]->material.kShininess = 1.0f;

	meshList[GEO_WORLD_QUAD] = MeshBuilder::GenerateQuad("World Quad", Color(1.f, 1.f, 1.f), 1, 1, 1);
	meshList[GEO_WORLD_QUAD]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_WORLD_QUAD]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_WORLD_QUAD]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WORLD_QUAD]->material.kShininess = 1.0f;

	meshList[GEO_LIGHT] = MeshBuilder::GenerateSphere("THE MYSTERIOUS MAGIC AMAZING LIGHT", Color(1.0, 1.0, 1.0), 9, 18, 1);

	//Load OBJ Models
	meshList[GEO_OBJCAKE] = MeshBuilder::GenerateOBJ("CAKE", "GameData//OBJ//CAKE.obj");
	meshList[GEO_OBJCAKE]->textureID[0] = LoadTGA("GameData//Image//OBJ//Cake_UV.tga", true);
	meshList[GEO_OBJCAKE]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_OBJCAKE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_OBJCAKE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_OBJCAKE]->material.kShininess = 1.0f;

	meshList[GEO_SECURITYCAMERA] = MeshBuilder::GenerateOBJ("CAKE", "GameData//OBJ//other//SecurityCamera.obj");
	meshList[GEO_SECURITYCAMERA]->textureID[0] = LoadTGA("GameData//Image//OBJ//SecurityCamera_UV.tga", true);
	meshList[GEO_SECURITYCAMERA]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_SECURITYCAMERA]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SECURITYCAMERA]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SECURITYCAMERA]->material.kShininess = 10.0f;

	//WEAPONS

	meshList[GEO_BULLET] = MeshBuilder::GenerateSphere("Gun bullet", Color(1.f, 0.8f, 0.5f), 4, 4, 0.53f);
	meshList[GEO_BULLET]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_BULLET]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_BULLET]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BULLET]->material.kShininess = 10.0f;

	meshList[GEO_M9] = MeshBuilder::GenerateOBJ("M9", "GameData//OBJ//weapons//M9.obj");
	meshList[GEO_M9]->textureID[0] = LoadTGA("GameData//Image//weapons//M9.tga", true);

	meshList[GEO_MP5K] = MeshBuilder::GenerateOBJ("MP5K", "GameData//OBJ//weapons//MP5K.obj");
	meshList[GEO_MP5K]->textureID[0] = LoadTGA("GameData//Image//weapons//MP5K.tga", true);

	meshList[GEO_SPAS12] = MeshBuilder::GenerateOBJ("SPAS-12", "GameData//OBJ//weapons//SPAS12.obj");
	meshList[GEO_SPAS12]->textureID[0] = LoadTGA("GameData//Image//weapons//SPAS12.tga", true);

	meshList[GEO_KATANA] = MeshBuilder::GenerateOBJ("Katana", "GameData//OBJ//weapons//Katana.obj");
	meshList[GEO_KATANA]->textureID[0] = LoadTGA("GameData//Image//weapons//Katana.tga", true);

	meshList[GEO_SCALPLE] = MeshBuilder::GenerateOBJ("Katana", "GameData//OBJ//weapons//Scalple.obj");
	meshList[GEO_SCALPLE]->textureID[0] = LoadTGA("GameData//Image//weapons//Scalple.tga", true);

	meshList[GEO_ITEM_SYRINGE] = MeshBuilder::GenerateOBJ("Syringe", "GameData//OBJ//Items//Syringe.obj");
	meshList[GEO_ITEM_SYRINGE]->textureID[0] = LoadTGA("GameData//Image//Items//Syringe.tga", true);


	meshList[GEO_TABLE] = MeshBuilder::GenerateOBJ("Syringe", "GameData//OBJ//Other//Table.obj");
	meshList[GEO_TABLE]->textureID[0] = LoadTGA("GameData//Image//OBJ//Table.tga", true);

	meshList[GEO_KEYCARD] = MeshBuilder::GenerateOBJ("Keycard", "GameData//OBJ//Items//keycard.obj");
	meshList[GEO_KEYCARD]->textureID[0] = LoadTGA("GameData//Image//Items//keycard.tga", true);

	meshList[GEO_DOOR] = MeshBuilder::GenerateOBJ("door", "GameData//OBJ//door.obj");
	meshList[GEO_DOOR]->textureID[0] = LoadTGA("GameData//Image//door.tga");


	meshList[GEO_EXIT] = MeshBuilder::GenerateOBJ("door", "GameData//OBJ//door.obj");
	meshList[GEO_EXIT]->textureID[0] = LoadTGA("GameData//Image//doorexit.tga");

	meshList[GEO_KEYCOUNT] = MeshBuilder::GenerateQuad("keycount",Color(0,1,0), 1.0, 1.0);
	meshList[GEO_KEYCOUNT]->textureID[0] = LoadTGA("GameData//Image//card.tga", false);


	meshList[GEO_M9]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_M9]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_M9]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_M9]->material.kShininess = 10.0f;

	meshList[GEO_SPAS12]->material = meshList[GEO_M9]->material;
	meshList[GEO_MP5K]->material = meshList[GEO_M9]->material;
	meshList[GEO_KATANA]->material = meshList[GEO_M9]->material;
	meshList[GEO_SCALPLE]->material = meshList[GEO_M9]->material;
	meshList[GEO_ITEM_SYRINGE]->material = meshList[GEO_M9]->material;
	meshList[GEO_TABLE]->material = meshList[GEO_M9]->material;

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

	meshList[GEO_EMISSIVE_QUAD] = MeshBuilder::GenerateQuad("Emissive map", Color(1, 1, 1), 1.f, 1.f, 1.f);
	meshList[GEO_EMISSIVE_QUAD]->textureID[0] = m_gBuffer.GetTexture(GBuffer::GBUFFER_TEXTURE_TYPE_EMISSIVE);

	meshList[GEO_SCREEN_OVERLAY] = MeshBuilder::GenerateQuad("Screen tint", Color(1, 1, 1), 1.f, 1.f, 1.f);


	renderAxis = false;

	mouseEnabled = true;
	if (mouseEnabled)
	{
		Application::SetMouseinput(Application::GetWindowWidth()*0.5, Application::GetWindowHeight()*0.5);
		Application::SetCursor(false);
	}

	FPScounter = 0.f;

	DisplayInfo = true;

	P_Player.Init(Vector3(0, 100.f, 0), Vector3(0, 10, -1), "GameData//Image//player//PlayerSkin.tga");
	P_Player.scale.Set(10, 10, 10);
	P_Player.collisionMesh.Type = CollisionBox::CT_AABB;
	P_Player.collisionMesh.Position = P_Player.pos;
	P_Player.collisionMesh.ColBox.Set(5, 5, 5);
	P_Player.collisionMesh.ColOffset = P_Player.ModelPos + P_Player.HeadPos;

	b_step = false;

	gravity_force.Set(0.f, -9.82f * 25, 0.f);

	InitMenus();
	initWeapons();

	PowerActive = false;
	CurrentPower = PT_SLOWMO;
	f_powerTint = 0.f;
	f_powerTintSet = f_powerTint;


	inputDelay = 0.f;
	timer = 0.f;

	soundList[ST_BACKGROUND] = SE_Engine.preloadSound("GameData//sounds//ambience//background.wav");
	soundList[ST_BACKGROUND]->setDefaultVolume(0.3f);

	soundList[ST_SLOWMO_ENTER] = SE_Engine.preloadSound("GameData//sounds//effects//slowmo_enter.mp3");
	soundList[ST_SLOWMO_EXIT] = SE_Engine.preloadSound("GameData//sounds//effects//slowmo_exit.mp3");

	soundList[ST_HEARTBEAT] = SE_Engine.preloadSound("GameData//sounds//effects//heartbeat.mp3");
	soundList[ST_BREATHING] = SE_Engine.preloadSound("GameData//sounds//effects//breathing.mp3");
	soundList[ST_DEATH] = SE_Engine.preloadSound("GameData//sounds//effects//death.mp3");

	soundList[ST_WALL_POWER_ENTER] = SE_Engine.preloadSound("GameData//sounds//effects//wall_power_enter.mp3");
	soundList[ST_WALL_POWER_ENTER]->setDefaultVolume(0.5f);
	soundList[ST_WALL_POWER_EXIT] = SE_Engine.preloadSound("GameData//sounds//effects//wall_power_exit.mp3");
	soundList[ST_WALL_POWER_EXIT]->setDefaultVolume(0.5f);

	soundList[ST_STEP] = SE_Engine.preloadSound("GameData//sounds//other//step1.wav");
	soundList[ST_STEP]->setDefaultVolume(0.3f);

	soundList[ST_JUMP] = SE_Engine.preloadSound("GameData//sounds//other//jump.wav");
	soundList[ST_JUMP]->setDefaultVolume(0.8f);

	soundList[ST_LAND] = SE_Engine.preloadSound("GameData//sounds//other//land.wav");
	soundList[ST_LAND]->setDefaultVolume(0.8f);

	soundList[ST_ALERT] = SE_Engine.preloadSound("GameData//sounds//other//alert.wav");

	soundList[ST_OBJ_BREAK] = SE_Engine.preloadSound("GameData//sounds//effects//objbreak.wav");
	soundList[ST_OBJ_BREAK]->setDefaultVolume(0.3f);
	soundList[ST_OBJ_BREAK]->setDefaultMinDistance(150.f);

	soundList[ST_WEAPON_M9_SHOOT] = SE_Engine.preloadSound("GameData//sounds//weapons//M9//FIRE.wav");
	soundList[ST_WEAPON_M9_SHOOT]->setDefaultVolume(0.3f);
	soundList[ST_WEAPON_M9_SHOOT]->setDefaultMinDistance(500.f);

	soundList[ST_WEAPON_KATANA] = SE_Engine.preloadSound("GameData//sounds//weapons//Katana.mp3");

	soundList[ST_WEAPON_CLICK] = SE_Engine.preloadSound("GameData//sounds//weapons//empty.wav");

	soundList[ST_AI_DEATH] = SE_Engine.preloadSound("GameData//sounds//other//ai_hit.mp3");
	soundList[ST_AI_DEATH]->setDefaultMinDistance(100.f);

	soundList[ST_AI_ALERT] = SE_Engine.preloadSound("GameData//sounds//other//ai_alert.mp3");

	soundList[ST_CAMERA_SPOTTED] = SE_Engine.preloadSound("GameData//sounds//other//EnemySpotted.mp3");
	soundList[ST_CAMERA_FOUND] = SE_Engine.preloadSound("GameData//sounds//other//Alarm.mp3");

	soundList[ST_STATUS] = SE_Engine.preloadSound("GameData//sounds//other//buzzer.wav");
	soundList[ST_STATUS]->setDefaultVolume(0.06f);

	GAMESTATE = GS_PLAY;

	
	loadLevel(currentLevel);

	PlayerSound = new SoundDetect(P_Player.pos, 100);

	//Dialogue Test
	CollisionBox DBox1;
	DBox1.Type = CollisionBox::CT_SPHERE;
	DBox1.Position = Vector3(40, 5, 280);
	DBox1.radius = 130;
	PressurePlate *Dialogue1 = new PressurePlate(DBox1, 1000);
	std::string S1_1 = "Welcome To Gene!";
	std::string S2_1 = "Press B - See Through Walls!";
	std::vector<std::string>Messages1;
	Messages1.push_back(S1_1);
	Messages1.push_back(S2_1);
	double T1_1 = 900;
	double T2_1 = 100;
	std::vector<double>MsgSeq1;
	MsgSeq1.push_back(T1_1);
	MsgSeq1.push_back(T2_1);
	Dialogue1->setDialogue(Messages1, MsgSeq1);
	Dialogues[0].push_back(Dialogue1);

	//Dialogue Test
	CollisionBox DBox2;
	DBox2.Type = CollisionBox::CT_SPHERE;
	DBox2.Position = Vector3(50, 60, 70);
	DBox2.radius = 100;
	PressurePlate *Dialogue2 = new PressurePlate(DBox2, 1000);
	std::string S1_2 = "Too Loud! You'll get caught!";
	std::string S2_2 = "Press Ctrl to Stealth Walk!";
	std::vector<std::string>Messages2;
	Messages2.push_back(S1_2);
	Messages2.push_back(S2_2);
	double T1_2 = 900;
	double T2_2 = 100;
	std::vector<double>MsgSeq2;
	MsgSeq2.push_back(T1_2);
	MsgSeq2.push_back(T2_2);
	Dialogue2->setDialogue(Messages2, MsgSeq2);
	Dialogues[0].push_back(Dialogue2);

	KeyRotate = 0;
	KeyCount = 0;
	DoorRotate = 0;
	bombCount = 1;
	
}

/******************************************************************************/
/*!
\brief
Switch scene back to main
*/
/******************************************************************************/
void mainscene::InitShaders()
{
	e_nextScene = Application::E_SCENE_MENU;
}

/******************************************************************************/
/*!
\brief
Loads a level map
\param level
the level to load
\return
returns true if loadmap is sucessful
*/
/******************************************************************************/
bool mainscene::loadLevel(int level)
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

	Floor = NULL;
	Celling = NULL;
	SWALL1 = NULL;
	SWALL2 = NULL;
	SWALL3 = NULL;
	SWALL4 = NULL;

	f_targetfov = f_defaultfov;
	P_Player.active = false;
	P_Player.vel.SetZero();
	P_Player.DropObject();
	PowerActive = false;
	f_powerTintSet = 0.f;
	f_poweramount = 60.f;
	f_playerHealthTint = 0.f;

	//Dialogue Reload
	for (unsigned i = 0; i < 5; ++i)
	{
		for (unsigned k = 0; k < Dialogues[i].size(); ++k)
		{
			Dialogues[i][k]->setTimer(1000);
		}
	}
	std::cout << "Dialogues Cleared\n";

	//Door Clear

	while (Doors.size() > 0)
	{
		Doors.pop_back();
	}

	//Keys Clear
	while (Keys.size() > 0)
	{
		Keys.pop_back();
	}

	while (PulseBombs.size() > 0)
	{
		PulseBombs.pop_back();
	}
	while (Stopper.size() > 0)
	{
		Stopper.pop_back();
	}

	std::cout << "Keys Cleared\n";
	KeyCount = 0;

	//Status Clear
	status = "\0";
	statusTimer = 0;

	//PulseBombs Clear
	bombCount = 0;
	std::cout<<"Traps Cleared\n";

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

	WO_END = NULL;

	float worldsize = static_cast<float>(GAME_MAP.worldSize);
	float worldHeight = static_cast<float>(GAME_MAP.worldHeight);

	std::cout << "Map Size: ";
	std::cout << GAME_MAP.map_width << ", " << GAME_MAP.map_height << "\n";

	for (unsigned y = GAME_MAP.map_height - 1; y > 0; --y)
	{
		for (unsigned x = 0; x < GAME_MAP.map_width; ++x)
		{
			if (GAME_MAP.map_data[y][x] == "." || GAME_MAP.map_data[y][x] == "" || GAME_MAP.map_data[y][x].size() <= 0)
			{
				continue;
			}

			if (GAME_MAP.map_data[y][x] == "SPAWN")//Generate spawnpoint
			{
				P_Player.active = true;
				P_Player.pos.Set(x*worldsize*2.f, 10.f, y*worldsize*2.f);
				P_Player.collisionMesh.Position = P_Player.pos;
			}
			else if (GAME_MAP.map_data[y][x][0] == 'I')
			{
				loadLevel_GenerateOBJ(GAME_MAP.map_data[y][x], Vector3(x*worldsize*2.f, 10.f, y*worldsize*2.f));
			}
			else if (GAME_MAP.map_data[y][x][0] == 'W')
			{
				std::string temp_str_X, temp_str_Y, temp_str_Z;
				temp_str_X = "";
				temp_str_Y = "";
				temp_str_Z = "";

				float SizeX, SizeY, SizeZ;
				SizeX = 0.f;
				SizeY = 0.f;
				SizeZ = 0.f;
				int temp_int_1 = 0;

				for (unsigned i = 1; GAME_MAP.map_data[y][x][i] != 'x'; ++i)
				{
					temp_str_X += GAME_MAP.map_data[y][x][i];
					temp_int_1 = i + 2;
				}

				for (unsigned i = temp_int_1; GAME_MAP.map_data[y][x][i] != 'y'; ++i)
				{
					temp_str_Y += GAME_MAP.map_data[y][x][i];
					temp_int_1 = i + 2;
				}

				for (unsigned i = temp_int_1; GAME_MAP.map_data[y][x][i] != 'z'; ++i)
				{
					temp_str_Z += GAME_MAP.map_data[y][x][i];
				}

				SizeX = static_cast<float>(std::atoi(temp_str_X.c_str()));
				SizeY = static_cast<float>(std::atoi(temp_str_Y.c_str()));
				SizeZ = static_cast<float>(std::atoi(temp_str_Z.c_str()));

				if (SizeX > worldsize)
				{
					SizeX = worldsize;
				}

				if (SizeY > worldHeight)
				{
					SizeY = worldHeight;
				}

				if (SizeZ > worldsize)
				{
					SizeZ = worldsize;
				}

				WorldObject *WO;
				WO = new WorldObject();
				WO->active = true;
				WO->colEnable = true;
				WO->scale.Set(SizeX, SizeY, SizeZ);
				WO->pos.Set(x*worldsize*2.f, SizeY, y*worldsize*2.f);
				WO->collisionMesh.Type = CollisionBox::CT_AABB;
				WO->collisionMesh.ColBox.Set(SizeX, SizeY, SizeZ);
				WO->dynamicRendering = true;
				WO->mesh = meshList[GEO_WORLD_CUBE];
				m_goList.push_back(WO);
			}
			else if (GAME_MAP.map_data[y][x][0] == 'S')
			{
				if (GAME_MAP.map_data[y][x][1] == 'W')
				{
					std::string temp_str_X, temp_str_Y, temp_str_Z;
					temp_str_X = "";
					temp_str_Y = "";
					temp_str_Z = "";

					float SizeX, SizeY, SizeZ;
					SizeX = 0.f;
					SizeY = 0.f;
					SizeZ = 0.f;
					int temp_int_1 = 1;

					for (unsigned i = 1; GAME_MAP.map_data[y][x][i] != 'x'; ++i)
					{
						temp_str_X += GAME_MAP.map_data[y][x][i];
						temp_int_1 = i + 2;
					}

					for (unsigned i = temp_int_1; GAME_MAP.map_data[y][x][i] != 'y'; ++i)
					{
						temp_str_Y += GAME_MAP.map_data[y][x][i];
						temp_int_1 = i + 2;
					}

					for (unsigned i = temp_int_1; GAME_MAP.map_data[y][x][i] != 'z'; ++i)
					{
						temp_str_Z += GAME_MAP.map_data[y][x][i];
					}

					SizeX = static_cast<float>(std::atoi(temp_str_X.c_str()));
					SizeY = static_cast<float>(std::atoi(temp_str_Y.c_str()));
					SizeZ = static_cast<float>(std::atoi(temp_str_Z.c_str()));

					if (SizeX > worldsize)
					{
						SizeX = worldsize;
					}

					if (SizeY > worldHeight)
					{
						SizeY = worldHeight;
					}

					if (SizeZ > worldsize)
					{
						SizeZ = worldsize;
					}

					CollisionBox Stop;
					Stop.Type = CollisionBox::CT_AABB;
					Stop.ColBox.Set(SizeX, SizeY, SizeZ);
					Stop.Position = Vector3(x*worldsize*2.f, SizeY, y*worldsize*2.f);
					Stopper.push_back(Stop);
				}
			}
			else if (GAME_MAP.map_data[y][x][0] == 'K')
			{
				CollisionBox Key;
				Key.Type = CollisionBox::CT_SPHERE;
				Key.Position = Vector3(x * worldsize * 2.f, 15, y * worldsize * 2.f);
				Key.radius = 10;
				Keys.push_back(Key);
			}
			else if (GAME_MAP.map_data[y][x][0] == 'D')
			{
				CollisionBox doorBound;
				doorBound.Type = CollisionBox::CT_SPHERE;
				doorBound.Position = Vector3(x * worldsize * 2.f, 50, y * worldsize * 2.f);
				doorBound.radius = worldsize;

				Doors.push_back(doorBound);
			}
			else if (GAME_MAP.map_data[y][x][0] == 'A')
			{
				if (GAME_MAP.map_data[y][x].size() > 1)
				{
					AI *ai;
					if (GAME_MAP.map_data[y][x][1] == '1')
					{
						ai = new AI(AI::WALKING, AI::AI_SCIENTIST);
						ai->Init(Vector3(x*worldsize*2.f, 0, y*worldsize*2.f), Vector3(0, 0, 0), "GameData//Image//player//scientist.tga");

						ItemObject *IO;
						IO = new ItemObject(IO_presetList[IO_SYRINGE]);
						ai->HoldObject(IO);
						m_goList.push_back(IO);
					}
					else if (GAME_MAP.map_data[y][x][1] == '2')
					{
						ai = new AI(AI::WALKING, AI::AI_SECURITY);
						ai->Init(Vector3(x*worldsize*2.f, 0, y*worldsize*2.f), Vector3(0, 0, 0), "GameData//Image//player//security.tga");

						WeaponsObject *WO;
						WO = new WeaponsObject(WO_presetList[WO_M9]);
						ai->HoldObject(WO);
						m_goList.push_back(WO);
					}

					if (ai != NULL)
					{
						ai->Lookat = ai->pos + Vector3(0, 0, 10);
						ai->scale.Set(10, 10, 10);
						ai->collisionMesh.Type = CollisionBox::CT_AABB;
						ai->collisionMesh.Position = ai->pos;
						ai->collisionMesh.ColBox.Set(6, 15, 6);
						ai->collisionMesh.ColOffset.Set(0, 15, 0);

						m_goList.push_back(ai);
					}
				}
			}
			else if (GAME_MAP.map_data[y][x][0] == 'S')
			{
				std::string temp_str_1, temp_str_2;
				temp_str_1 = "";
				temp_str_2 = "";

				int textOffset = 0;
				for (unsigned i = 1; GAME_MAP.map_data[y][x][i] != 'A'; ++i)
				{
					temp_str_1 += GAME_MAP.map_data[y][x][i];
					textOffset = i + 1;
				}

				for (unsigned a = textOffset + 1; GAME_MAP.map_data[y][x][a] != 'D'; ++a)
				{
					temp_str_2 += GAME_MAP.map_data[y][x][a];
				}
				float SCAngle;
				float rotationAngle;
				SCAngle = static_cast<float>(std::stoi(temp_str_1));
				rotationAngle = static_cast<float>(std::stoi(temp_str_2));

				SecurityCam *SC;
				SC = new SecurityCam();
				SC->active = true;
				SC->pos.Set(x*worldsize*2.f, worldHeight*2.f, y*worldsize*2.f);
				SC->colEnable = true;
				SC->collisionMesh.Type = CollisionBox::CT_AABB;
				SC->collisionMesh.ColBox.Set(3.f, 3.f, 3.f);
				SC->rotation.y = 180.f;
				SC->scale.Set(6, 6, 6);
				SC->isHeld = false;
				SC->enablePhysics = false;
				SC->setRotationAngle(rotationAngle);
				Mtx44 rotationMtx;
				rotationMtx.SetToRotation(SCAngle, 0, 1, 0);
				Vector3 templookat(0, 0, -1);
				SC->offsetY = worldHeight;
				SC->Lookat = (rotationMtx * templookat) + SC->pos;
				SC->dynamicRendering = true;
				SC->mesh = meshList[GEO_SECURITYCAMERA];

				m_goList.push_back(SC);
			}
			else if (GAME_MAP.map_data[y][x][0] == 'O')
			{
				WorldObject *WO;

				if (GAME_MAP.map_data[y][x].size() > 1)
				{
					if (GAME_MAP.map_data[y][x][1] == 'T')
					{
						WO = new WorldObject();
						WO->active = true;
						WO->colEnable = true;
						WO->scale.Set(5, 5, 5);
						WO->collisionMesh.Type = CollisionBox::CT_AABB;

						if (GAME_MAP.map_data[y][x][2] == '1')
						{
							WO->collisionMesh.ColBox.Set(14, 20, 24);
						}
						else
						{
							WO->rotation.y = 90.f;
							WO->collisionMesh.ColBox.Set(24, 20, 14);
						}

						WO->mesh = meshList[GEO_TABLE];

						if (GAME_MAP.map_data[y][x].size() > 3)
						{
							if (GAME_MAP.map_data[y][x][4] == '1')
							{
								loadLevel_GenerateOBJ("II_SYRINGE", Vector3(x*worldsize*2.f, 20.f, y*worldsize*2.f));

								if (GAME_MAP.map_data[y][x][2] == '1')
								{
									loadLevel_GenerateOBJ("II_SYRINGE", Vector3(x*worldsize*2.f, 20.f, y*worldsize*2.f + 5));
									loadLevel_GenerateOBJ("II_SYRINGE", Vector3(x*worldsize*2.f, 20.f, y*worldsize*2.f + 10));
									loadLevel_GenerateOBJ("II_SYRINGE", Vector3(x*worldsize*2.f, 20.f, y*worldsize*2.f - 10));
									loadLevel_GenerateOBJ("IW_SCALPLE", Vector3(x*worldsize*2.f, 20.f, y*worldsize*2.f) - 5);
								}
								else
								{
									loadLevel_GenerateOBJ("II_SYRINGE", Vector3(x*worldsize*2.f + 5, 20.f, y*worldsize*2.f));
									loadLevel_GenerateOBJ("IW_SCALPLE", Vector3(x*worldsize*2.f - 5, 20.f, y*worldsize*2.f));
									loadLevel_GenerateOBJ("II_SYRINGE", Vector3(x*worldsize*2.f + 10, 20.f, y*worldsize*2.f));
									loadLevel_GenerateOBJ("II_SYRINGE", Vector3(x*worldsize*2.f - 10, 20.f, y*worldsize*2.f));
								}
							}
							else if (GAME_MAP.map_data[y][x][4] == '2')
							{
								if (GAME_MAP.map_data[y][x][2] == '1')
								{
									loadLevel_GenerateOBJ("IW_KATANA", Vector3(x*worldsize*2.f, 20.f, y*worldsize*2.f), Vector3(90, 0, 0));
								}
								else
								{
									loadLevel_GenerateOBJ("IW_KATANA", Vector3(x*worldsize*2.f, 20.f, y*worldsize*2.f), Vector3(0, 0, 90));
								}
							}
							else if (GAME_MAP.map_data[y][x][4] == '3')
							{
								loadLevel_GenerateOBJ("IW_SCALPLE", Vector3(x*worldsize*2.f, 20.f, y*worldsize*2.f), Vector3(0, 0, 0));
							}
						}
					}
				}

				if (WO != NULL)
				{
					WO->pos.Set(x*worldsize*2.f, 0, y*worldsize*2.f);
					WO->dynamicRendering = true;
					m_goList.push_back(WO);
				}
			}
			else if (GAME_MAP.map_data[y][x] == "EXIT")
			{
				WorldObject *WO;
				WO = new WorldObject();
				WO->active = true;
				WO->colEnable = false;
				WO->scale.Set(20.f, 25.f, 20.f);
				WO->pos.Set(x*worldsize*2.f, 1, y*worldsize*2.f);
				WO->collisionMesh.Type = CollisionBox::CT_SPHERE;
				WO->collisionMesh.radius = 30.f;
				WO->collisionMesh.Position = WO->pos;
				WO->dynamicRendering = true;
				WO->mesh = meshList[GEO_EXIT];
				WO_END = WO;
				m_goList.push_back(WO);
			}
		}
	}

	//World Floor
	WorldObject *WO;
	WO = new WorldObject();
	WO->pos.Set(GAME_MAP.map_width*0.5f*worldsize, 0.f, (GAME_MAP.map_height*0.5f)*worldsize);
	WO->scale.Set(GAME_MAP.map_width * worldsize * 2.f, 1, GAME_MAP.map_height * worldsize * 2.f);
	WO->collisionMesh.Type = CollisionBox::CT_AABB;
	WO->collisionMesh.ColBox.Set(GAME_MAP.map_width * worldsize * 2.f, 5, GAME_MAP.map_height * worldsize * 2.f);
	WO->active = true;
	WO->enablePhysics = false;
	WO->colEnable = true;
	WO->mesh = meshList[GEO_WORLD_CUBE];
	WO->dynamicRendering = false;
	Floor = WO;
	m_goList.push_back(WO);
	//World Celling
	WO = new WorldObject();
	WO->pos.Set(GAME_MAP.map_width*0.5f*worldsize, worldHeight*2.f, (GAME_MAP.map_height*0.5f)*worldsize);
	WO->rotation.x = 90.f;
	WO->scale.Set(GAME_MAP.map_width * worldsize * 2.f, GAME_MAP.map_height * worldsize * 2.f, 1);
	WO->collisionMesh.Type = CollisionBox::CT_AABB;
	WO->collisionMesh.ColBox.Set(GAME_MAP.map_width * worldsize * 2.f, 5, GAME_MAP.map_height * worldsize * 2.f);
	WO->active = true;
	WO->enablePhysics = false;
	WO->colEnable = true;
	WO->mesh = meshList[GEO_WORLD_QUAD];
	WO->dynamicRendering = false;
	Celling = WO;
	m_goList.push_back(WO);

	WO = new WorldObject();
	WO->pos.Set(worldsize * 2.f, worldHeight, GAME_MAP.map_height * worldsize * 2.f);
	WO->scale.Set(GAME_MAP.map_width * worldsize * 2.f, worldHeight*1.2f, worldsize);
	WO->collisionMesh.Type = CollisionBox::CT_AABB;
	WO->collisionMesh.ColBox.Set(GAME_MAP.map_width * worldsize * 2.f, worldHeight*1.2f, worldsize);
	WO->active = true;
	WO->enablePhysics = false;
	WO->colEnable = true;
	WO->mesh = meshList[GEO_WORLD_CUBE];
	WO->dynamicRendering = false;
	SWALL1 = WO;
	m_goList.push_back(WO);

	WO = new WorldObject();
	WO->pos.Set(worldsize * 2.f, worldHeight, 0.f);
	WO->scale.Set(GAME_MAP.map_width * worldsize * 2.2f, worldHeight*1.2f, worldsize);
	WO->collisionMesh.Type = CollisionBox::CT_AABB;
	WO->collisionMesh.ColBox.Set(GAME_MAP.map_width * worldsize * 2.2f, worldHeight*1.2f, worldsize);
	WO->active = true;
	WO->enablePhysics = false;
	WO->colEnable = true;
	WO->mesh = meshList[GEO_WORLD_CUBE];
	WO->dynamicRendering = false;
	SWALL2 = WO;
	m_goList.push_back(WO);

	WO = new WorldObject();
	WO->pos.Set(-worldsize * 2.f, worldHeight, GAME_MAP.map_height * worldsize);
	WO->scale.Set(worldsize, worldHeight*1.2f, GAME_MAP.map_height * worldsize * 2.f);
	WO->collisionMesh.Type = CollisionBox::CT_AABB;
	WO->collisionMesh.ColBox.Set(worldsize, worldHeight*1.2f, GAME_MAP.map_height * worldsize * 2.f);
	WO->active = true;
	WO->enablePhysics = false;
	WO->colEnable = true;
	WO->mesh = meshList[GEO_WORLD_CUBE];
	WO->dynamicRendering = false;
	SWALL3 = WO;
	m_goList.push_back(WO);

	WO = new WorldObject();
	WO->pos.Set(GAME_MAP.map_width * worldsize * 2.f, worldHeight, GAME_MAP.map_height * worldsize);
	WO->scale.Set(worldsize, worldHeight*1.2f, GAME_MAP.map_height * worldsize * 2.f);
	WO->collisionMesh.Type = CollisionBox::CT_AABB;
	WO->collisionMesh.ColBox.Set(worldsize, worldHeight*1.2f, GAME_MAP.map_height * worldsize * 2.f);
	WO->active = true;
	WO->enablePhysics = false;
	WO->colEnable = true;
	WO->mesh = meshList[GEO_WORLD_CUBE];
	WO->dynamicRendering = false;
	SWALL4 = WO;
	m_goList.push_back(WO);

	lights[0].position.y = worldHeight * 2.5f;

	if (!P_Player.active)
	{
		std::cout << "!!! Warning !!! No player spawn location is found.\n";
	}

	if (WO_END == NULL)
	{
		std::cout << "!!! Warning !!! No exit is found.\n";
	}

	FPC.Init(P_Player.pos + P_Player.CamOffset, P_Player.pos + P_Player.CamOffset + Vector3(0.f, 0.f, -1.f), Vector3(0.f, 1.f, 0.f), f_mouseSensitivity);
	std::cout << "Map Successfully loaded\n";
	Application::waitforload();
	return true;
}

/******************************************************************************/
/*!
\brief
Generates an object at a certain position
\param object
name of the object to load
\param Position
the position of the object to spawn in
*/
/******************************************************************************/
void mainscene::loadLevel_GenerateOBJ(std::string object, Vector3 &Position, Vector3 &rotation)
{
	if (object[0] == 'I')
	{
		if (object[1] == 'W')
		{
			WeaponsObject *WO;
			if (object == "IW_M9")
			{
				WO = new WeaponsObject(WO_presetList[WO_M9]);
			}
			else if (object == "IW_KATANA")
			{
				WO = new WeaponsObject(WO_presetList[WO_KATANA]);
			}
			else if (object == "IW_SCALPLE")
			{
				WO = new WeaponsObject(WO_presetList[WO_SCALPLE]);
			}
			if (WO != NULL)
			{
				WO->pos = Position;
				WO->rotation = rotation;
				m_goList.push_back(WO);
				return;
			}
		}
		else if (object[1] == 'I')
		{
			ItemObject *IO;
			if (object == "II_SYRINGE")
			{
				IO = new ItemObject(IO_presetList[IO_SYRINGE]);
			}

			if (IO != NULL)
			{
				IO->pos = Position;
				IO->rotation = rotation;
				m_goList.push_back(IO);
				return;
			}
		}
	}
}

/******************************************************************************/
/*!
\brief
Gets an unsused particle in the vector
\return
returns an unactive particle
*/
/******************************************************************************/
Particle* mainscene::FetchParticle(void)
{
	for (unsigned i = 0; i < m_ParList.size(); ++i)
	{
		if (!m_ParList[i]->active)
		{
			m_ParList[i]->active = false;
			m_ParList[i]->ParticleType = Particle::PAR_SPARKS;
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
	return m_ParList.back();


}

/******************************************************************************/
/*!
\brief
Gets an unsused bulletobject in the gameobject vector
\return
returns an unactive gameobject
*/
/******************************************************************************/
BulletObject* mainscene::FetchBullet(void)
{
	for (unsigned i = 0; i < m_goList.size(); ++i)
	{
		BulletObject *BO = dynamic_cast<BulletObject*>(m_goList[i]);
		if (BO != NULL)
		{
			if (!BO->active)
			{
				BO->active = false;
				BO->gravityEnabled = false;
				return dynamic_cast<BulletObject*>(m_goList[i]);
				break;
			}
		}
	}

	for (unsigned i = 0; i < 10; ++i)
	{
		BulletObject *BO;
		BO = new BulletObject();
		BO->active = false;
		BO->gravityEnabled = false;
		m_goList.push_back(BO);
	}
	return dynamic_cast<BulletObject*>(m_goList.back());
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

	//ItemObject *IO;
	IO_presetList[IO_SYRINGE].active = true;
	IO_presetList[IO_SYRINGE].pos.Set(-20, 10, 0);
	IO_presetList[IO_SYRINGE].pos1.Set(-5, -4, 5);
	IO_presetList[IO_SYRINGE].scale.Set(0.5f, 0.5f, 0.5f);
	IO_presetList[IO_SYRINGE].collisionMesh.Type = CollisionBox::CT_AABB;
	IO_presetList[IO_SYRINGE].collisionMesh.ColBox.Set(1, 1, 1);
	IO_presetList[IO_SYRINGE].collisionMesh.ColOffset.Set(0, 1, 0);
	IO_presetList[IO_SYRINGE].enablePhysics = true;
	IO_presetList[IO_SYRINGE].holdable = true;
	IO_presetList[IO_SYRINGE].mesh = meshList[GEO_ITEM_SYRINGE];
	IO_presetList[IO_SYRINGE].isWeapon = false;
	IO_presetList[IO_SYRINGE].isGun = false;

	WO_presetList[WO_M9].active = true;
	WO_presetList[WO_M9].adsZoom = 1.5f;
	WO_presetList[WO_M9].mesh = meshList[GEO_M9];
	WO_presetList[WO_M9].attackRate = 0.5f;
	WO_presetList[WO_M9].scale.Set(0.03f, 0.03f, 0.03f);
	WO_presetList[WO_M9].shootvelocity = 400.f;
	WO_presetList[WO_M9].pos.Set(0, 10, 0);
	WO_presetList[WO_M9].pos1.Set(-5, -4, 9);
	WO_presetList[WO_M9].pos2.Set(0, -2.1f, 8);
	WO_presetList[WO_M9].CurrentClip = 2;
	WO_presetList[WO_M9].recoilEffect = 30.f;
	WO_presetList[WO_M9].isGun = true;
	WO_presetList[WO_M9].isWeapon = true;
	WO_presetList[WO_M9].enablePhysics = true;
	WO_presetList[WO_M9].colEnable = true;
	WO_presetList[WO_M9].collisionMesh.Type = CollisionBox::CT_AABB;
	WO_presetList[WO_M9].collisionMesh.ColBox.Set(3, 3, 3);
	WO_presetList[WO_M9].AttackSound = ST_WEAPON_M9_SHOOT;
	WO_presetList[WO_M9].range = 5.f;

	WO_presetList[WO_KATANA].active = true;
	WO_presetList[WO_KATANA].mesh = meshList[GEO_KATANA];
	WO_presetList[WO_KATANA].attackRate = 0.05f;
	WO_presetList[WO_KATANA].AnimSpeed = 9.f;
	WO_presetList[WO_KATANA].scale.Set(0.1f, 0.1f, 0.1f);
	WO_presetList[WO_KATANA].pos.Set(20, 10, 0);
	WO_presetList[WO_KATANA].pos1.Set(4, -7.5f, 9);
	WO_presetList[WO_KATANA].pos2.Set(10, -9.f, 12);
	WO_presetList[WO_KATANA].Rotation1.Set(5, 0, 45);
	WO_presetList[WO_KATANA].Rotation2.Set(90, 180, 90);
	WO_presetList[WO_KATANA].isGun = false;
	WO_presetList[WO_KATANA].isWeapon = true;
	WO_presetList[WO_KATANA].enablePhysics = true;
	WO_presetList[WO_KATANA].colEnable = true;
	WO_presetList[WO_KATANA].collisionMesh.Type = CollisionBox::CT_AABB;
	WO_presetList[WO_KATANA].collisionMesh.ColBox.Set(3, 3, 3);
	WO_presetList[WO_KATANA].AttackSound = ST_WEAPON_KATANA;
	WO_presetList[WO_KATANA].range = 0.06f;

	WO_presetList[WO_SCALPLE].active = true;
	WO_presetList[WO_SCALPLE].mesh = meshList[GEO_SCALPLE];
	WO_presetList[WO_SCALPLE].attackRate = 0.05f;
	WO_presetList[WO_SCALPLE].AnimSpeed = 10.f;
	WO_presetList[WO_SCALPLE].scale.Set(0.3f, 0.3f, 0.3f);
	WO_presetList[WO_SCALPLE].pos.Set(0, 10, 0);
	WO_presetList[WO_SCALPLE].pos1.Set(-4.f, -4.f, 6.f);
	WO_presetList[WO_SCALPLE].pos2.Set(-4.f, -4.f, 12.f);
	WO_presetList[WO_SCALPLE].Rotation1.Set(70, 0, 0);
	WO_presetList[WO_SCALPLE].Rotation2.Set(90, 0, 0);
	WO_presetList[WO_SCALPLE].isGun = false;
	WO_presetList[WO_SCALPLE].isWeapon = true;
	WO_presetList[WO_SCALPLE].enablePhysics = true;
	WO_presetList[WO_SCALPLE].colEnable = true;
	WO_presetList[WO_SCALPLE].collisionMesh.Type = CollisionBox::CT_AABB;
	WO_presetList[WO_SCALPLE].collisionMesh.ColBox.Set(3, 3, 3);
	WO_presetList[WO_SCALPLE].AttackSound = ST_WEAPON_KATANA;
	WO_presetList[WO_SCALPLE].range = 0.04f;

	f_curRecoil = 0.f;
}

/******************************************************************************/
/*!
\brief
Function to edit fov
\param newFOV
the new fov to change to
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
\param dt
delta time
*/
/******************************************************************************/
void mainscene::UpdatePlayer(double &dt)
{
	static bool inAir = false;

	double tempDT;

	if (d_dt != d_dt2)
	{
		tempDT = (d_dt + d_dt2) * 0.5f;
	}
	else
	{
		tempDT = d_dt;
	}

	//Y axis collision handling
	if (!collide(Vector3(P_Player.pos)))
	{
		if (collide(Vector3(P_Player.pos + P_Player.ModelPos + P_Player.HeadPos)))
		{
			if (P_Player.vel.y > 0)
			{
				P_Player.vel.y = 0;
			}
		}

		P_Player.vel += gravity_force * static_cast<float>(tempDT);

		inAir = true;
	}
	else
	{
		if (collide(Vector3(P_Player.pos + Vector3(0.f, 4.f, 0.f))))//This is to prevent floor clipping, or rather, to make it bounce back up if it's clipping
		{
			P_Player.vel.y = 2000 * static_cast<float>(tempDT);
		}

		else if (collide(Vector3(P_Player.pos + Vector3(0.f, 2.f, 0.f))))
		{
			P_Player.vel.y = 1000 * static_cast<float>(tempDT);
		}
		else if (P_Player.vel.y != 0)
		{
			P_Player.vel.y = 0.f;
			SE_Engine.playSound2D(soundList[ST_LAND]);
		}
		else if (P_Player.vel.y == 0)
		{
			inAir = false;
		}
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_SPRINT]))
	{
		P_Player.f_movementSpeed = P_Player.f_move_run;
	}
	else if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_WALK]))
	{
		P_Player.f_movementSpeed = P_Player.f_move_crawl;
	}
	else
	{
		P_Player.f_movementSpeed = P_Player.f_move_walk;
	}

	if (f_poweramount < 20)
	{
		P_Player.f_movementSpeed *= f_poweramount * 0.025f;
	}

	//Player movement
	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_FRONT]) && !Application::IsKeyPressed(us_control[E_CTRL_MOVE_BACK]))
	{
		P_Player.movementFB(tempDT);
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_BACK]) && !Application::IsKeyPressed(us_control[E_CTRL_MOVE_FRONT]))
	{
		P_Player.movementFB(tempDT, false);
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_LEFT]) && !Application::IsKeyPressed(us_control[E_CTRL_MOVE_RIGHT]))
	{
		P_Player.movementLR(tempDT);
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_RIGHT]) && !Application::IsKeyPressed(us_control[E_CTRL_MOVE_LEFT]))
	{
		P_Player.movementLR(tempDT, false);
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_JUMP]))
	{
		if (inAir == false)
		{
			P_Player.vel.y += 120;
			SE_Engine.playSound2D(soundList[ST_JUMP]);
		}
	}

	//smooth slowing down
	if (P_Player.vel.x != 0)
	{
		float SForceX = 0 - P_Player.vel.x;
		P_Player.vel.x += SForceX * static_cast<float>(tempDT)* 6.f;
	}

	if (P_Player.vel.z != 0)
	{
		float SForceZ = 0 - P_Player.vel.z;
		P_Player.vel.z += SForceZ * static_cast<float>(tempDT)* 6.f;
	}


	//Collision handling
	if (collide(Vector3(P_Player.pos + Vector3(10.f, 10.f, 0.f))) || collide(Vector3(P_Player.pos + Vector3(10.f, 20.f, 0.f))))
	{
		if (P_Player.vel.x > 0)
		{
			P_Player.vel.x = 0;
		}
	}

	if (collide(Vector3(P_Player.pos + Vector3(0.f, 10.f, 10.f))) || collide(Vector3(P_Player.pos + Vector3(0.f, 20.f, 10.f))))
	{
		if (P_Player.vel.z > 0)
		{
			P_Player.vel.z = 0;
		}
	}

	if (collide(Vector3(P_Player.pos + Vector3(-10.f, 10.f, 0.f))) || collide(Vector3(P_Player.pos + Vector3(-10.f, 20.f, 0.f))))
	{
		if (P_Player.vel.x < 0)
		{
			P_Player.vel.x = 0;
		}
	}

	if (collide(Vector3(P_Player.pos + Vector3(0.f, 10.f, -10.f))) || collide(Vector3(P_Player.pos + Vector3(0.f, 20.f, -10.f))))
	{
		if (P_Player.vel.z < 0)
		{
			P_Player.vel.z = 0;
		}
	}

	static bool isinteractPressed = false;
	if (Application::IsKeyPressed(us_control[E_CTRL_INTERACT]) && !isinteractPressed)
	{
		isinteractPressed = true;
	}
	else if (!Application::IsKeyPressed(us_control[E_CTRL_INTERACT]) && isinteractPressed)
	{
		isinteractPressed = false;
		if (P_Player.holding == NULL)
		{
			for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				ItemObject *IO = (ItemObject *)*it;
				if (IO->active && IO->holdable)
				{
					if (isVisible(FPC.position, FPC.target, 15, IO->pos))
					{
						if (isVisible2(FPC.position, FPC.target, 15, IO->pos))
						{
							if ((IO->pos - P_Player.pos).LengthSquared() < 1500)
							{
								P_Player.HoldObject(IO);
								break;
							}
						}
					}
				}
			}
		}
	}

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if (CollisionBox::checkCollision(P_Player.collisionMesh, go->collisionMesh))
			{
				if (go->vel.LengthSquared() > 2000)
				{
					BulletObject *BO = dynamic_cast<BulletObject*>(go);
					if (BO != NULL)
					{
						f_poweramount -= 90.f;
					}
				}
			}
		}
	}

	if (f_poweramount <= 0)
	{
		f_playerHealthTint = 100.f;
		SE_Engine.stopAllSounds();
		SE_Engine.effectDistortion(false);
		SE_Engine.playSound2D(soundList[ST_DEATH]);
		Application::SetCursor(true);
		GAMESTATE = GS_DEATH;
	}

	FPC = FPC + (P_Player.vel * static_cast<float>(tempDT));
	P_Player.Lookat = FPC.target;
	P_Player.Update(tempDT);
	P_Player.collisionMesh.Position = P_Player.pos;

	if (!inAir)
	{
		if (P_Player.getAnimation().LEFT_LEG < 0 && b_step)
		{
			SE_Engine.playSound2D(soundList[ST_STEP]);
			b_step = false;
		}
		else if (P_Player.getAnimation().LEFT_LEG > 0 && !b_step)
		{
			SE_Engine.playSound2D(soundList[ST_STEP]);
			b_step = true;
		}
	}
}

/******************************************************************************/
/*!
\brief
Handles player powers
\param dt
delta time
*/
/******************************************************************************/
void mainscene::UpdatePlayerPower(double &dt)
{
	static bool abilityPressed_1 = false;
	if (Application::IsKeyPressed(us_control[E_CTRL_ABILITY_1]) && !abilityPressed_1)
	{
		abilityPressed_1 = true;

		if (!PowerActive && f_poweramount > 20)
		{
			PowerActive = true;
			CurrentPower = PT_SLOWMO;
			f_powerTintSet = 25.f;
			c_powerColor.Set(0.1f, 0.f, 0.f);
			SE_Engine.playSound2D(soundList[ST_SLOWMO_ENTER]);
		}
		else if (PowerActive && CurrentPower == PT_SLOWMO)
		{
			PowerActive = false;
			f_powerTintSet = 0.f;
			SE_Engine.playSound2D(soundList[ST_SLOWMO_EXIT]);
		}
	}
	else if (!Application::IsKeyPressed(us_control[E_CTRL_ABILITY_1]) && abilityPressed_1)
	{
		abilityPressed_1 = false;
	}

	static bool abilityPressed_2 = false;
	if (Application::IsKeyPressed(us_control[E_CTRL_ABILITY_2]) && !abilityPressed_2)
	{
		abilityPressed_2 = true;
		if (!PowerActive && f_poweramount > 20)
		{
			for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				GameObject *go = (GameObject *)*it;
				if (go->active)
				{
					WorldObject *WO = dynamic_cast<WorldObject*>(go);
					if (WO != NULL && WO != Celling && WO != Floor && WO != SWALL1 && WO != SWALL2 && WO != SWALL3 && WO != SWALL4)
					{
						WO->Opacity = 10.f;
					}
				}
			}
			f_powerTintSet = 25.f;
			c_powerColor.Set(0.f, 0.f, 0.3f);
			CurrentPower = PT_SUPERVISION;
			SE_Engine.playSound2D(soundList[ST_WALL_POWER_ENTER]);
			PowerActive = true;
		}
		else if (PowerActive && CurrentPower == PT_SUPERVISION)
		{
			for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				GameObject *go = (GameObject *)*it;
				if (go->active)
				{
					WorldObject *WO = dynamic_cast<WorldObject*>(go);
					if (WO != NULL && WO != Celling && WO != Floor && WO != SWALL1 && WO != SWALL2 && WO != SWALL3 && WO != SWALL4)
					{
						WO->Opacity = 100.f;
					}
				}
			}
			PowerActive = false;
			f_powerTintSet = 0.f;
			SE_Engine.playSound2D(soundList[ST_WALL_POWER_EXIT]);
		}
	}
	else if (!Application::IsKeyPressed(us_control[E_CTRL_ABILITY_2]) && abilityPressed_2)
	{
		abilityPressed_2 = false;
	}

	if (PowerActive)
	{
		switch (CurrentPower)
		{
		case mainscene::PT_SLOWMO:
			dt *= 0.05;

			if (P_Player.vel != 0)
			{
				float temp = P_Player.vel.LengthSquared() *0.002f;
				if (temp > 1)
				{
					dt *= temp;
				}

				if (dt > d_dt)
				{
					dt = d_dt;
				}
			}

			if (!SE_Engine.isSoundPlaying(soundList[ST_BREATHING]))
			{
				SE_Engine.playSound2D(soundList[ST_BREATHING]);
			}

			if (f_poweramount <= 0)
			{
				PowerActive = false;
				f_powerTintSet = 0.f;
				SE_Engine.playSound2D(soundList[ST_SLOWMO_EXIT]);
			}

			f_poweramount -= static_cast<float>(d_dt)* 6.f;
			break;
		case mainscene::PT_SUPERVISION:
			if (f_poweramount <= 0)
			{
				for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
				{
					GameObject *go = (GameObject *)*it;
					if (go->active)
					{
						WorldObject *WO = dynamic_cast<WorldObject*>(go);
						if (WO != NULL && WO != Celling && WO != Floor && WO != SWALL1 && WO != SWALL2 && WO != SWALL3 && WO != SWALL4)
						{
							WO->Opacity = 100.f;
						}
					}
				}
				PowerActive = false;
				f_powerTintSet = 0.f;
				SE_Engine.playSound2D(soundList[ST_WALL_POWER_EXIT]);
			}
			f_poweramount -= static_cast<float>(d_dt)* 12.f;
			break;
		default:
			break;
		}
	}
	else
	{
		if (f_poweramount < 20)
		{
			f_poweramount += static_cast<float>(d_dt)* 1.2f;
		}
		else if (f_poweramount < 100)
		{
			f_poweramount += static_cast<float>(d_dt)* 3.f;
		}
	}

	if (f_poweramount < 20)
	{
		if (!SE_Engine.isSoundPlaying(soundList[ST_HEARTBEAT]))
		{
			SE_Engine.playSound2D(soundList[ST_HEARTBEAT], false, true, true);
		}

		f_playerHealthTint = 100.f - (f_poweramount * 5.f);
	}

	if (f_powerTint != f_powerTintSet)
	{
		float diff = f_powerTintSet - f_powerTint;

		if (diff < 0.01 && diff > -0.01)
		{
			f_powerTint = f_powerTintSet;
		}
		else
		{
			f_powerTint += diff * static_cast<float>(d_dt)* 10.f;
		}
	}

	PlayerSound->setSource(P_Player.pos);
}

/******************************************************************************/
/*!
\brief
Updates gameobjects
\param dt
delta time
*/
/******************************************************************************/
void mainscene::UpdateGO(double &dt)
{
	for (unsigned i = 0; i < m_goList.size(); ++i)
	{
		GameObject *go = m_goList[i];
		if (go->active)
		{
			go->collisionMesh.Position = go->pos;

			BulletObject *BO = dynamic_cast<BulletObject*>(go);
			if (BO != NULL)
			{
				UpdateBO(BO, dt);
				continue;
			}

			SecurityCam *SC = dynamic_cast<SecurityCam*>(go);
			if (SC != NULL)
			{
				SC->update(dt, P_Player.pos, m_goList);
				continue;
			}

			CharacterObject *CO = dynamic_cast<CharacterObject*>(go);
			if (CO != NULL)
			{
				UpdateCO(CO, dt);
				continue;
			}

			if (go->enablePhysics && !go->isHeld)
			{
				go->colEnable = false;

				if (collide(Vector3(go->pos.x, go->pos.y - go->collisionMesh.ColBox.y, go->pos.z)))
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

					if (collide(Vector3(go->pos.x, go->pos.y - go->collisionMesh.ColBox.y *0.8f, go->pos.z)))
					{
						go->vel.y = 500.f * static_cast<float>(dt);
					}
				}
				else
				{
					if (go->gravityEnabled)
					{
						go->vel += gravity_force * static_cast<float>(dt);
					}
				}

				if (collide(Vector3(go->pos.x + go->collisionMesh.ColBox.x, go->pos.y, go->pos.z)))
				{
					if (go->vel.x > 0)
					{
						go->vel.x = 0;
					}

					if (collide(Vector3(go->pos.x, go->pos.y + go->collisionMesh.ColBox.x *0.8f, go->pos.z)))
					{
						go->vel.x = -500 * static_cast<float>(dt);
					}
				}

				if (collide(Vector3(go->pos.x - go->collisionMesh.ColBox.x, go->pos.y, go->pos.z)))
				{
					if (go->vel.x < 0)
					{
						go->vel.x = 0;
					}

					if (collide(Vector3(go->pos.x, go->pos.y - go->collisionMesh.ColBox.x *0.8f, go->pos.z)))
					{
						go->vel.x = 500 * static_cast<float>(dt);
					}
				}

				if (collide(Vector3(go->pos.x, go->pos.y, go->pos.z + go->collisionMesh.ColBox.z)))
				{
					if (go->vel.z > 0)
					{
						go->vel.z = 0;
					}

					if (collide(Vector3(go->pos.x, go->pos.y + go->collisionMesh.ColBox.z *0.8f, go->pos.z)))
					{
						go->vel.z = -500 * static_cast<float>(dt);
					}
				}

				if (collide(Vector3(go->pos.x, go->pos.y, go->pos.z - go->collisionMesh.ColBox.z)))
				{
					if (go->vel.z < 0)
					{
						go->vel.z = 0;
					}

					if (collide(Vector3(go->pos.x, go->pos.y - go->collisionMesh.ColBox.z *0.8f, go->pos.z)))
					{
						go->vel.z = 500 * static_cast<float>(dt);
					}
				}

				if (collide(Vector3(go->pos.x, go->pos.y + go->collisionMesh.ColBox.y, go->pos.z)))
				{
					if (go->vel.y > 0)
					{
						go->vel.y = 0;
					}
				}

				go->colEnable = true;
				go->pos += go->vel * static_cast<float>(dt);
			}
			else
			{
				go->Update(d_dt);
			}
		}
	}
}

/******************************************************************************/
/*!
\brief
Updates bulletobject
\param BO
Bulletobject to update
\param dt
delta time
*/
/******************************************************************************/
void mainscene::UpdateBO(BulletObject *BO, double &dt)
{
	if (collide(BO->pos))
	{
		for (unsigned i = 0; i < 5; ++i)
		{
			generateParticle(BO->pos, Vector3(0.2f, 0.2f, 0.2f), Vector3(Math::RandFloatMinMax(-70, 70), Math::RandFloatMinMax(-5, 70), Math::RandFloatMinMax(-70, 70)) - BO->vel*0.01f, Vector3(0.f, 0.f, 0.f), Particle::PAR_SPARKS, 1.0f);
		}

		BO->active = false;
	}

	if (BO->life > 0)
	{
		BO->life -= static_cast<float>(dt);
	}
	else
	{
		BO->active = false;
	}

	BO->pos += BO->vel * static_cast<float>(dt);
}

/******************************************************************************/
/*!
\brief
Updates characterobject
\param CP
characterobject to update
\param dt
delta time
*/
/******************************************************************************/
void mainscene::UpdateCO(CharacterObject *CO, double &dt)
{
	AI *ai = dynamic_cast<AI*>(CO);
	if (ai != NULL)
	{
		if(isVisible(ai->pos, ai->Lookat, static_cast<float>(ai->getDetectionAngle()), ai->getDestination()) && (ai->getDestination() - ai->pos).LengthSquared() < ai->getDetectionRange_Max())
		{
			ai->b_isDestinationWithinFOV = true;
			ai->b_isDestinationVisible = false;
		}
		else
		{
			ai->b_isDestinationWithinFOV = false;
			ai->b_isDestinationVisible = false;
		}

		if(ai->b_isDestinationWithinFOV)
		{
			if (CollisionBetween(ai->pos + ai->ModelPos + ai->HeadPos, ai->getDestination() + Vector3(0, 10, 0)))
			{
				ai->b_isDestinationVisible = false;
			}
			else
			{
				ai->b_isDestinationVisible = true;
			}
		}

		if(ai->getState() == AI::ATTACK)
		{
			if (ai->holding != NULL)
			{
				WeaponsObject *WO = dynamic_cast<WeaponsObject*>(ai->holding);
				if (WO != NULL)
				{
					if (WO->isGun)
					{
						if (ai->attackrate + WO->attackRate < timer)
						{
							ai->attackrate = timer;

							if(ai->getShootGun() == true)
							{
								SE_Engine.playSound3D(soundList[WO->AttackSound], ai->pos);
								Shoot(ai->pos + ai->HeadPos + ai->ModelPos + (ai->getDirection(true).Normalize() * 20), ((P_Player.pos + P_Player.ModelPos + P_Player.HeadPos) - (ai->pos + ai->HeadPos + ai->ModelPos)).Normalize(), WO->shootvelocity, WO->range);
							}
						}
					}
				}
				else
				{
					if ((P_Player.pos - ai->pos).LengthSquared() < 800)
					{
						f_poweramount -= 80.f * static_cast<float>(dt);
					}
				}
			}
			else
			{
				if ((P_Player.pos - ai->pos).LengthSquared() < 800)
				{
					f_poweramount -= 30.f * static_cast<float>(dt);
				}
			}
		}

		ai->Update(dt, P_Player.pos, m_goList);

		//Temp collision for AI to weapon
		for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if (go->active)
			{
				if (CollisionBox::checkCollision(ai->collisionMesh, go->collisionMesh))
				{
					if (go->vel.LengthSquared() > 2000)
					{
						BulletObject *BO = dynamic_cast<BulletObject*>(go);
						if (BO != NULL)
						{
							//AI DIES
							alertDeath(CO->pos, 100);
							SE_Engine.playSound3D(soundList[ST_AI_DEATH], CO->pos);
							BO->active = false;
							CO->DropObject();
							CO->active = false;
							generateCharacterParticle(CO, go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)), go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)), go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)), go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)), go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)), go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)));
						}
						else
						{
							if (CO->holding == NULL)
							{
								alertDeath(CO->pos, 100);
								SE_Engine.playSound3D(soundList[ST_AI_DEATH], CO->pos);
								SE_Engine.playSound3D(soundList[ST_OBJ_BREAK], CO->pos);
								go->active = false;
								CO->DropObject();
								CO->active = false;
								generateCharacterParticle(CO, go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)), go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)), go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)), go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)), go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)), go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)));
							}
							else
							{
								SE_Engine.playSound3D(soundList[ST_OBJ_BREAK], CO->pos);
								go->active = false;
								CO->DropObject(Vector3(-go->vel.x*0.1f, Math::RandFloatMinMax(20, 120), -go->vel.z*0.1f));
							}							
						}
					}
				}
			}
		}
	}

	if (CO->holding != NULL)
	{
		CO->holding->Update(dt);
	}
}

/******************************************************************************/
/*!
\brief
Generates particles at position
*/
/******************************************************************************/
void mainscene::generateParticle(Vector3 &Pos, Vector3 &scale, Vector3 &Velocity, Vector3 &Rotation, int type, float lifetime, Mesh *mesh)
{
	Particle *Par = FetchParticle();
	Par->ParticleType = type;
	Par->Pos = Pos;
	Par->Rotation = Rotation;
	Par->Scale = scale;
	Par->Vel = Velocity;
	Par->lifetime = lifetime;
	Par->mesh = mesh;
	Par->active = true;
}

/******************************************************************************/
/*!
\brief
Generates particles at position
*/
/******************************************************************************/
void mainscene::generateCharacterParticle(CharacterObject *CO, Vector3 &HeadVel, Vector3 &ArmLeftVel, Vector3 &ArmRightVel, Vector3 &LegLeftVel, Vector3 &LegRightVel, Vector3 &BodyVel)
{
	float CharRotation = CalAnglefromPosition(CO->Lookat, CO->pos, true);
	generateParticle(CO->pos + CO->ModelPos + CO->HeadPos, CO->scale, HeadVel, Vector3(0, CharRotation, 0), Particle::PAR_MESH, 2.f, CO->Head);

	Mtx44 Rotation;
	Rotation.SetToRotation(CharRotation, 0, 1, 0);
	Vector3 tempArm = CO->ArmPos;
	tempArm = Rotation * tempArm;
	generateParticle(CO->pos + CO->ModelPos + tempArm, CO->scale, ArmRightVel, Vector3(0, CharRotation, 0), Particle::PAR_MESH, 2.f, CO->Arm_right);
	tempArm = Vector3(-CO->ArmPos.x, CO->ArmPos.y, CO->ArmPos.z);
	tempArm = Rotation * tempArm;
	generateParticle(CO->pos + CO->ModelPos + tempArm, CO->scale, ArmLeftVel, Vector3(0, CharRotation, 0), Particle::PAR_MESH, 2.f, CO->Arm_left);

	generateParticle(CO->pos + CO->ModelPos + CO->LegPos, CO->scale, LegLeftVel, Vector3(0, CharRotation, 0), Particle::PAR_MESH, 2.f, CO->Leg_left);

	generateParticle(CO->pos + CO->ModelPos + CO->LegPos, CO->scale, LegRightVel, Vector3(0, CharRotation, 0), Particle::PAR_MESH, 2.f, CO->Leg_right);

	generateParticle(CO->pos + CO->ModelPos, CO->scale, BodyVel, Vector3(0, CharRotation, 0), Particle::PAR_MESH, 2.f, CO->Chest);

	for (unsigned i = 0; i < 64; ++i)
	{
		float bloodsize = Math::RandFloatMinMax(0.1f, .8f);
		generateParticle(CO->pos + CO->ModelPos, Vector3(bloodsize, bloodsize, bloodsize), Vector3(Math::RandFloatMinMax(-70, 70), Math::RandFloatMinMax(-5, 70), Math::RandFloatMinMax(-70, 70)) + BodyVel, Vector3(0.f, 0.f, 0.f), Particle::PAR_BLOOD, 2.f);
	}
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
Fires bullet
*/
/******************************************************************************/
void mainscene::Shoot(const Vector3 &Pos, const Vector3 &Dir, float Speed, float Longevity, bool melee)
{
	BulletObject *BO;
	BO = FetchBullet();
	BO->active = true;
	BO->pos = Pos;
	BO->vel = Dir * Speed;
	BO->life = Longevity;
	BO->collisionMesh.Type = CollisionBox::CT_SPHERE;
	BO->colEnable = true;
	BO->collisionMesh.radius = 1.f;
	BO->scale.Set(0.5f, 0.5f, 0.5f);
	if (!melee)
	{
		BO->mesh = meshList[GEO_BULLET];
	}
	else
	{
		BO->collisionMesh.radius = 3.f;
		BO->mesh = NULL;
	}
}

/******************************************************************************/
/*!
\brief
Handles held items
*/
/******************************************************************************/
void mainscene::weaponsUpdate(double &dt)
{
	if (P_Player.holding != NULL)
	{
		static bool isAttackPressed = false;
		if (Application::IsKeyPressed(us_control[E_CTRL_THROW]) || !P_Player.holding->isWeapon && Application::IsKeyPressed(us_control[E_CTRL_ATTACK]) && !isAttackPressed)
		{
			isAttackPressed = true;

			if (P_Player.holding->isWeapon)
			{
				WeaponsObject *WO = dynamic_cast<WeaponsObject*>(P_Player.holding);
				if (WO != NULL)
				{
					if (!WO->animState)
					{
						WO->toggleAnimation();
						f_targetfov = f_defaultfov;
					}
				}
			}
			firerate = 0.f;
			P_Player.DropObject(P_Player.getDirection().Normalized() * 400.f);
		}
		else if (P_Player.holding->isWeapon)
		{
			WeaponsObject *WO = dynamic_cast<WeaponsObject*>(P_Player.holding);
			if (WO != NULL)
			{
				if (Application::IsKeyPressed(us_control[E_CTRL_ATTACK]))
				{
					if (P_Player.holding->isGun)
					{
						if (WO->CurrentClip > 0 && WO->attackRate + firerate < timer)
						{
							firerate = timer;
							Vector3 ShootVector = FPC.target - FPC.position;
							ShootVector.Normalize();
							FPC.rotateCamVertical(static_cast<float>(dt)* WO->recoilEffect);
							Shoot(FPC.position + ShootVector * 14.f, ShootVector, WO->shootvelocity, WO->range);
							WO->rotation.x -= WO->recoilEffect *0.1f;
							WO->pos.z -= WO->recoilEffect*0.02f;
							SE_Engine.playSound2D(soundList[WO->AttackSound]);
							f_curRecoil += WO->recoilEffect * 0.05f;
							--WO->CurrentClip;
						}
						else if (WO->CurrentClip <= 0 && !isAttackPressed)
						{
							isAttackPressed = true;
							SE_Engine.playSound2D(soundList[ST_WEAPON_CLICK]);
						}
					}
					else
					{
						if (WO->isAnimationComplete() && firerate + WO->attackRate < timer && WO->animState && !isAttackPressed)
						{
							isAttackPressed = true;
							firerate = timer;
							WO->toggleAnimation();

							Vector3 ShootVector = FPC.target - FPC.position;
							ShootVector.Normalize();
							Shoot(FPC.position + ShootVector * 14.f, ShootVector, 350.f, WO->range, true);

							SE_Engine.playSound2D(soundList[WO->AttackSound]);
						}
					}
				}
				else if (!Application::IsKeyPressed(us_control[E_CTRL_ATTACK]) && isAttackPressed)
				{
					isAttackPressed = false;
				}

				if (P_Player.holding->isGun)
				{
					static bool isAimPressed = false;
					if (Application::IsKeyPressed(us_control[E_CTRL_AIM]) && !isAimPressed)
					{
						isAimPressed = true;
						WO->toggleAnimation();
						if (!WO->animState)
						{
							f_targetfov = f_targetfov / WO->adsZoom;
						}
						else
						{
							f_targetfov = f_defaultfov;
						}
					}
					else if (!Application::IsKeyPressed(us_control[E_CTRL_AIM]) && isAimPressed)
					{
						isAimPressed = false;
					}

					if (f_curRecoil > 0)
					{
						f_curRecoil -= f_curRecoil * 0.1f;
					}
				}
			}
		}
		else if (!Application::IsKeyPressed(us_control[E_CTRL_ATTACK]) && isAttackPressed)
		{
			isAttackPressed = false;
		}

		static bool isinteractPressed = false;
		if (Application::IsKeyPressed(us_control[E_CTRL_INTERACT]) && !isinteractPressed)
		{
			isinteractPressed = true;
		}
		else if (!Application::IsKeyPressed(us_control[E_CTRL_INTERACT]) && isinteractPressed)
		{
			isinteractPressed = false;
			firerate = 0.f;
			P_Player.DropObject();
		}
	}
}

/******************************************************************************/
/*!
\brief
Check collision
*/
/******************************************************************************/
bool mainscene::collide(Vector3 &Position)
{
	//Game object collisions
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active && go->colEnable && go->pos != Position)
		{
			CollisionBox temp;
			temp.Type = CollisionBox::CT_POINT;
			temp.Position = Position;
			if (CollisionBox::checkCollision(go->collisionMesh, temp))
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

/******************************************************************************/
/*!
\brief
Check collision between gameobject
*/
/******************************************************************************/
bool mainscene::collideGO(GameObject *go, GameObject *go2)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go2 = (GameObject *)*it;
		if (CollisionBox::checkCollision(go->collisionMesh, go2->collisionMesh))
		{
			return true;
		}
	}
	return false;
}

/******************************************************************************/
/*!
\brief
update player sound position
*/
/******************************************************************************/
void mainscene::UpdateSound(double &dt)
{
	if (d_dt != d_dt2)
	{
		SE_Engine.effectDistortion(true, (static_cast<float>(d_dt - dt) *3.f) - 30.f);
	}
	else
	{
		SE_Engine.effectDistortion(false);
	}

	SE_Engine.UpdateListenerPosition(FPC.position, (FPC.target - FPC.position), FPC.up);
}

/******************************************************************************/
/*!
\brief
update menu buttons
*/
/******************************************************************************/
void mainscene::UpdateButtons(void)
{
	for (std::vector<TextButton*>::iterator it = v_buttonList.begin(); it != v_buttonList.end(); ++it)
	{
		TextButton *S_MB = (TextButton *)*it;
		if (S_MB->gamestate == GAMESTATE)
		{
			if (intersect2D((S_MB->pos + Vector3(S_MB->text.length() * (S_MB->scale.x) - S_MB->scale.x, S_MB->scale.y*0.4f, 0)), S_MB->pos + Vector3(-S_MB->scale.x*0.5f, -(S_MB->scale.y*0.4f), 0), Vector3(mousePosX, mousePosY, 0)))
			{
				S_MB->active = true;
				S_MB->color = UIColorPressed;
			}
			else
			{
				S_MB->active = false;
				S_MB->color = UIColor;
			}
		}
	}
}

/******************************************************************************/
/*!
\brief
Animations, controls
*/
/******************************************************************************/
void mainscene::Update(double dt)
{
	//std::cout << this->m_<< std::endl;
	d_dt = dt;
	FPScounter = static_cast<float>(1 / dt);

	double x, y;
	Application::GetMousePos(x, y);
	mousePosX = (static_cast<float>(x) / static_cast<float>(Application::GetWindowWidth()) * static_cast<float>(Application::GetWindowWidth())*0.1f);
	mousePosY = ((static_cast<float>(Application::GetWindowHeight()) - static_cast<float>(y)) / static_cast<float>(Application::GetWindowHeight()) * static_cast<float>(Application::GetWindowHeight())*0.1f);

	if (f_fov != f_targetfov)
	{
		float diff = f_targetfov - f_fov;

		if (diff < 0.01 && diff > -0.01)
		{
			f_fov = f_targetfov;
		}
		else
		{
			f_fov += diff * static_cast<float>(d_dt)* 10.f;
		}

		editFOV(f_fov);
	}

	if (TESTMODE)
	{
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
	}	

	UpdateButtons();

	static bool isEscPressed = false;
	static bool isLmbPressed = false;

	switch (GAMESTATE)
	{
	case mainscene::GS_PLAY:
		{
			UpdatePlayerPower(dt);
			d_dt2 = dt;
			timer += static_cast<float>(dt);

			UpdatePlayer(dt);
			UpdateGO(dt);
			UpdateParticles(dt);
			FPC.Update(dt);

			weaponsUpdate(dt);
			UpdateSound(dt);

			Vector3 newRad = P_Player.vel;
			if (P_Player.vel.y == 0)
			{
				PlayerSound->setSoundRadius(newRad.Length()*0.55f);
				CheckPlayerSound();
			}
			KeyRotate += 25 * static_cast<float>(dt);
			if (KeyRotate >= 360)
				KeyRotate = 0;

			DoorRotate += 60 * static_cast<float>(dt);
			if (DoorRotate >= 360)
				DoorRotate = 0;

			checkDoor();
			checkKey();
			checkStatus();
			pushPlayer();
			stopAI(2.0f);

			if (WO_END != NULL)
			{
				if (CollisionBox::checkCollision(P_Player.collisionMesh, WO_END->collisionMesh))
				{
					++currentLevel;
					if (!loadLevel(currentLevel))
					{
						Application::SetCursor(true);
						GAMESTATE = GS_END;
					}
				}
			}

			if (Application::IsKeyPressed(VK_ESCAPE) && !isEscPressed)
			{
				isEscPressed = true;
			}
			else if (!Application::IsKeyPressed(VK_ESCAPE) && isEscPressed)
			{
				isEscPressed = false;
				Application::SetCursor(true);
				GAMESTATE = GS_PAUSED;
			}
			break;
		}
	case mainscene::GS_PAUSED:
		if (Application::IsKeyPressed(VK_ESCAPE) && !isEscPressed)
		{
			isEscPressed = true;
		}
		else if (!Application::IsKeyPressed(VK_ESCAPE) && isEscPressed)
		{
			isEscPressed = false;
			Application::SetCursor(false);
			Application::SetMouseinput(Application::GetWindowWidth()*0.5, Application::GetWindowHeight()*0.5);
			GAMESTATE = GS_PLAY;
		}

		if (Application::IsKeyPressed(VK_LBUTTON) && !isLmbPressed)
		{
			isLmbPressed = true;
		}
		else if (!Application::IsKeyPressed(VK_LBUTTON) && isLmbPressed)
		{
			isLmbPressed = false;
			if (FetchBUTTON("Return to menu")->active)
			{
				e_nextScene = Application::E_SCENE_MENU;
			}
			{
				isLmbPressed = false;
				if (FetchBUTTON("Return to menu")->active)
				{
					e_nextScene = Application::E_SCENE_MENU;
				}
				else if (FetchBUTTON("Back to game")->active)
				{
					Application::SetCursor(false);
					Application::SetMouseinput(Application::GetWindowWidth()*0.5, Application::GetWindowHeight()*0.5);
					GAMESTATE = GS_PLAY;
				}
				else if (FetchBUTTON("Previous level")->active)
				{
					--currentLevel;
					if (!loadLevel(currentLevel))
					{
						++currentLevel;
					}

					Application::SetCursor(false);
					Application::SetMouseinput(Application::GetWindowWidth()*0.5, Application::GetWindowHeight()*0.5);
					GAMESTATE = GS_PLAY;
				}
				else if (FetchBUTTON("Skip to next level")->active)
				{
					++currentLevel;
					if (!loadLevel(currentLevel))
					{
						--currentLevel;
					}

					Application::SetCursor(false);
					Application::SetMouseinput(Application::GetWindowWidth()*0.5, Application::GetWindowHeight()*0.5);
					GAMESTATE = GS_PLAY;
				}
				else if (FetchBUTTON("Quit")->active)
				{
					e_nextScene = Application::E_SCENE_QUIT;
				}
			}
		}
		break;
	case GS_END:
		if (Application::IsKeyPressed(VK_ESCAPE) && !isEscPressed)
		{
			isEscPressed = true;
		}
		else if (!Application::IsKeyPressed(VK_ESCAPE) && isEscPressed)
		{
			isEscPressed = false;
			e_nextScene = Application::E_SCENE_MENU;
		}

		if (Application::IsKeyPressed(VK_LBUTTON) && !isLmbPressed)
		{
			isLmbPressed = true;
		}
		else if (!Application::IsKeyPressed(VK_LBUTTON) && isLmbPressed)
		{
			isLmbPressed = false;
			if (FetchBUTTON("Return to menu")->active)
			{
				e_nextScene = Application::E_SCENE_MENU;
			}
			else if (FetchBUTTON("Play again")->active)
			{
				currentLevel = 1;
				loadLevel(currentLevel);
				Application::SetCursor(false);
				Application::SetMouseinput(Application::GetWindowWidth()*0.5, Application::GetWindowHeight()*0.5);
				GAMESTATE = GS_PLAY;
			}
			else if (FetchBUTTON("Quit")->active)
			{
				e_nextScene = Application::E_SCENE_QUIT;
			}
		}
		break;
	case GS_DEATH:
		if (Application::IsKeyPressed(VK_ESCAPE) && !isEscPressed)
		{
			isEscPressed = true;
		}
		else if (!Application::IsKeyPressed(VK_ESCAPE) && isEscPressed)
		{
			isEscPressed = false;
			e_nextScene = Application::E_SCENE_MENU;
		}

		if (Application::IsKeyPressed(VK_LBUTTON) && !isLmbPressed)
		{
			isLmbPressed = true;
		}
		else if (!Application::IsKeyPressed(VK_LBUTTON) && isLmbPressed)
		{
			isLmbPressed = false;
			if (FetchBUTTON("Return to menu")->active)
			{
				e_nextScene = Application::E_SCENE_MENU;
			}
			else if (FetchBUTTON("Try again")->active)
			{
				loadLevel(currentLevel);
				Application::SetCursor(false);
				Application::SetMouseinput(Application::GetWindowWidth()*0.5, Application::GetWindowHeight()*0.5);
				GAMESTATE = GS_PLAY;
			}
			else if (FetchBUTTON("Quit")->active)
			{
				e_nextScene = Application::E_SCENE_QUIT;
			}
		}
		break;
	default:
		break;
	}

	/*
	if (Application::IsKeyPressed('T'))
	layTrap(P_Player.pos);
	if (Application::IsKeyPressed('G'))
	activateTrap();
	*/
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

		SecurityCam * SC = dynamic_cast<SecurityCam*>(go);
		if (SC != NULL)
		{
			float YRotation = CalAnglefromPosition(SC->Lookat, SC->pos, true);
			modelStack.Rotate(YRotation, 0, 1, 0);

			if (TESTMODE)
			{
				RenderSCDebugging(SC);
			}
		}
		modelStack.Scale(go->scale);
		if (go->mesh)
		{
			RenderMesh(go->mesh, true, true, go->Opacity);
		}
		modelStack.PopMatrix();
	}
}

/******************************************************************************/
/*!
\brief
Rendering of AI debugging range
*/
/******************************************************************************/
void mainscene::RenderAIDebugging(CharacterObject * CO)
{
	AI *ai = dynamic_cast<AI*>(CO);

	if (ai != NULL)
	{
		if (ai->getState() == AI::WALKING || ai->getState() == AI::ALERT)
		{
			//Detection Range
			modelStack.PushMatrix();
			modelStack.Rotate(static_cast<float>(ai->getDetectionAngle()), 0, 1, 0);
			modelStack.Scale(0, 0, static_cast<float>(sqrt(ai->getDetectionRange())));
			RenderMesh(meshList[GEO_REDLINE], false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Rotate(static_cast<float>(-ai->getDetectionAngle()), 0, 1, 0);
			modelStack.Scale(0, 0, static_cast<float>(sqrt(ai->getDetectionRange())));
			RenderMesh(meshList[GEO_REDLINE], false);
			modelStack.PopMatrix();

			//Alert Range
			if (ai->getState() == AI::WALKING)
			{
				modelStack.PushMatrix();
				modelStack.Translate(0, -1, 0);
				modelStack.Rotate(static_cast<float>(ai->getDetectionAngle()), 0, 1, 0);
				modelStack.Scale(0, 0, static_cast<float>(sqrt(ai->getDetectionRange_Max())));
				RenderMesh(meshList[GEO_BLUELINE], false);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0, -1, 0);
				modelStack.Rotate(static_cast<float>(-ai->getDetectionAngle()), 0, 1, 0);
				modelStack.Scale(0, 0, static_cast<float>(sqrt(ai->getDetectionRange_Max())));
				RenderMesh(meshList[GEO_BLUELINE], false);
				modelStack.PopMatrix();
			}
			//Destination
			else
			{
				modelStack.PushMatrix();
				modelStack.Translate(0, -17, 0);
				modelStack.Scale(0, 0, (ai->pos - ai->getDestination()).Length());
				RenderMesh(meshList[GEO_REDLINE], false);
				modelStack.PopMatrix();
			}
		}
		//Attack State
		else
		{
			modelStack.PushMatrix();
			modelStack.Scale(0, 0, (P_Player.pos - ai->pos).Length());
			RenderMesh(meshList[GEO_GREENLINE], false);
			modelStack.PopMatrix();
		}
	}
}

/******************************************************************************/
/*!
\brief
Rendering of Security Camera debugging range
*/
/******************************************************************************/
void mainscene::RenderSCDebugging(SecurityCam * SC)
{
	modelStack.PushMatrix();
	modelStack.Translate(0, -GAME_MAP.worldHeight * 2 + 2.f, 0);
	modelStack.Rotate(SC->getCameraFOV(), 0, 1, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(0, 0, sqrt(SC->getCameraRange()));
	RenderMesh(meshList[GEO_REDLINE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -GAME_MAP.worldHeight * 2 + 2.f, 0);
	modelStack.Rotate(-SC->getCameraFOV(), 0, 1, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(0, 0, sqrt(SC->getCameraRange()));
	RenderMesh(meshList[GEO_REDLINE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(0, 0, Vector3(0, -GAME_MAP.worldHeight * 2 + 2.f, 0).Length());
	RenderMesh(meshList[GEO_GREENLINE], false);
	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Rendering of character objects
*/
/******************************************************************************/
void mainscene::RenderCharacter(CharacterObject *CO)
{
	float YRotation = CalAnglefromPosition(CO->Lookat, CO->pos, true);
	float Pitch;
	if (CO == &P_Player)
	{
		Pitch = -CalAnglefromPosition(CO->Lookat, CO->pos + CO->CamOffset, false);
	}
	else
	{
		Pitch = -CalAnglefromPosition(CO->Lookat, CO->pos, false);
	}

	if (CO->holding != NULL)
	{
		modelStack.PushMatrix();
		modelStack.Translate(CO->pos);
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

	if (CO == &P_Player && m_renderPass != RENDER_PASS_PRE)
	{
		return;
	}

	modelStack.PushMatrix();
	modelStack.Translate(CO->pos);
	modelStack.Translate(CO->ModelPos);
	modelStack.Rotate(YRotation, 0, 1, 0);
	if (TESTMODE)
	{
		RenderAIDebugging(CO);
	}

	modelStack.PushMatrix();
	modelStack.Scale(CO->scale);
	RenderMesh(CO->Chest, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(CO->HeadPos);
	modelStack.Scale(CO->scale);
	RenderMesh(CO->Head, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-CO->ArmPos.x, CO->ArmPos.y, CO->ArmPos.z);
	modelStack.Scale(CO->scale);
	RenderMesh(CO->Arm_left, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(CO->ArmPos);
	modelStack.Scale(CO->scale);
	RenderMesh(CO->Arm_right, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(CO->LegPos);
	modelStack.Rotate(CO->getAnimation().LEFT_LEG, 1, 0, 0);
	modelStack.Scale(CO->scale);
	RenderMesh(CO->Leg_left, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(CO->LegPos);
	modelStack.Rotate(CO->getAnimation().RIGHT_LEG, 1, 0, 0);
	modelStack.Scale(CO->scale);
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
			case Particle::PAR_SPARKS:
				{
					modelStack.PushMatrix();
					modelStack.Translate(Par->Pos);
					modelStack.Rotate(Par->Rotation.x, 1, 0, 0);
					modelStack.Rotate(Par->Rotation.y, 0, 1, 0);
					modelStack.Rotate(Par->Rotation.z, 0, 0, 1);
					modelStack.Scale(Par->Scale);
					RenderMesh(meshList[GEO_BULLET], false, false, 100, 100, Color(1.f, 0.9f, 0.5f));
					modelStack.PopMatrix();
					break;
				}
			case Particle::PAR_BLOOD:
				{
					modelStack.PushMatrix();
					modelStack.Translate(Par->Pos);
					modelStack.Rotate(Par->Rotation.x, 1, 0, 0);
					modelStack.Rotate(Par->Rotation.y, 0, 1, 0);
					modelStack.Rotate(Par->Rotation.z, 0, 0, 1);
					modelStack.Scale(Par->Scale);
					RenderMesh(meshList[GEO_BULLET], false, false, 100, 100, Color(1.f, 0.f, 0.f));
					modelStack.PopMatrix();
					break;
				}
			case Particle::PAR_MESH:
				{
					if (Par->mesh != NULL)
					{
						modelStack.PushMatrix();
						modelStack.Translate(Par->Pos);
						modelStack.Rotate(Par->Rotation.x, 1, 0, 0);
						modelStack.Rotate(Par->Rotation.y, 0, 1, 0);
						modelStack.Rotate(Par->Rotation.z, 0, 0, 1);
						modelStack.Scale(Par->Scale);
						RenderMesh(Par->mesh, true);
						modelStack.PopMatrix();
					}
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
Renders mesh in 3D
*/
/******************************************************************************/
void mainscene::RenderMeshin2D(Mesh *mesh, bool enableLight, float visibility, float glow, Color &glowColor)
{
	if (visibility <= 0)
	{
		return;
	}
	glUniform1i(m_parameters[U_GLOW], static_cast<GLint>(glow));
	glUniform3fv(m_parameters[U_GLOW_COLOR], 1, &glowColor.r);
	glUniform1i(m_parameters[U_TRANSPARENCY], static_cast<GLint>(visibility));
	glUniform1i(m_parameters[U_FOG_ENABLED], 0);
	glUniform1i(m_parameters[U_LIGHTENABLED], static_cast<GLint>(enableLight));

	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::GetWindowWidth()*0.1, 0, Application::GetWindowHeight()*0.1, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	//modelStack.LoadIdentity();


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
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Renders mesh
*/
/******************************************************************************/
void mainscene::RenderMesh(Mesh *mesh, bool enableLight, bool enableFog, float visibility, float glow, Color &glowColor, Material *material)
{
	if (visibility <= 0)
	{
		return;
	}
	glUniform1i(m_parameters[U_GLOW], static_cast<GLint>(glow));
	glUniform3fv(m_parameters[U_GLOW_COLOR], 1, &glowColor.r);
	glUniform1i(m_parameters[U_TRANSPARENCY], static_cast<GLint>(visibility));

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
	glUniform1i(m_parameters[U_TRANSPARENCY], 100);

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
	glUniform1i(m_parameters[U_TRANSPARENCY], 100);

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
	//modelStack.LoadIdentity(); //Reset modelStack
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
				CharacterObject *CO = dynamic_cast<CharacterObject*>(go);
				if (CO != NULL)
				{
					if (CurrentPower == PT_SUPERVISION && PowerActive)
					{
						glDisable(GL_DEPTH_TEST);
						RenderCharacter(CO);
						glEnable(GL_DEPTH_TEST);
					}
					else
					{
						RenderCharacter(CO);
					}
				}
				else
				{
					RenderGO(go);
				}
			}
			else
			{
				if (isVisible(FPC.position, FPC.target, f_defaultfov, go->pos) || (Vector3(FPC.position.x - go->pos.x, 0, FPC.position.z - go->pos.z)).LengthSquared() < 12000)//Dynamic rendering
				{
					CharacterObject *CO = dynamic_cast<CharacterObject*>(go);
					if (CO != NULL)
					{
						if (CurrentPower == PT_SUPERVISION && PowerActive)
						{
							glDisable(GL_DEPTH_TEST);
							RenderCharacter(CO);
							glEnable(GL_DEPTH_TEST);
						}
						else
						{
							RenderCharacter(CO);
						}
					}
					else
					{
						RenderGO(go);
					}
				}
			}
		}
	}

	RenderCharacter(&P_Player);
	RenderParticles();



	//Keycards
	for (unsigned i = 0; i < Keys.size(); ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(Keys[i].Position.x, 3, Keys[i].Position.z);
		modelStack.Rotate(KeyRotate, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_KEYCARD], false);
		modelStack.PopMatrix();
	}

	//Doors
	for (unsigned i = 0; i < Doors.size(); ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(Doors[i].Position.x, -1, Doors[i].Position.z);
		modelStack.Rotate(DoorRotate, 0, 1, 0);
		modelStack.Scale(20, 25, 20);
		RenderMesh(meshList[GEO_DOOR], false);
		modelStack.PopMatrix();
	}

	/*
	//Bombs
	for (unsigned i = 0; i < PulseBombs.size(); ++i)
	{
	modelStack.PushMatrix();
	modelStack.Translate(PulseBombs[i].Position.x, PulseBombs[i].Position.y, PulseBombs[i].Position.z);
	modelStack.Scale(20, 25, 20);
	RenderMesh(meshList[GEO_OBJCAKE], false);
	modelStack.PopMatrix();
	}
	*/
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
}

/******************************************************************************/
/*!
\brief
Renders the ingameUI
*/
/******************************************************************************/
void mainscene::RenderUI(void)
{
	switch (GAMESTATE)
	{
	case mainscene::GS_DEATH:
		modelStack.PushMatrix();
		modelStack.Translate(Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f, 0.1f);
		modelStack.Scale(static_cast<float>(Application::GetWindowWidth()*0.05f), static_cast<float>(Application::GetWindowHeight()*0.05f), 0.f);
		RenderMeshin2D(meshList[GEO_SCREEN_OVERLAY], false, f_playerHealthTint, 10.f, Color(0.5f, 0.f, 0.f));
		modelStack.PopMatrix();
		break;
	case mainscene::GS_PLAY:
		{
			//Dialogues On Screen
			for (unsigned i = 0; i < Dialogues[currentLevel - 1].size(); ++i)
			{
				std::string Result = Dialogues[currentLevel - 1][i]->inEffect(&P_Player, 1);
				//std::cout<<Result;
				if (Result == "TIME")
					delete Dialogues[currentLevel - 1][i];
				else if (Result != "")
					RenderTextOnScreen(meshList[GEO_TEXT], Result, Color(0, 1, 1), 3.5f, 15.f, 10.f);
			}

			if (status != "\0")
				RenderTextOnScreen(meshList[GEO_TEXT], status, Color(0, 1, 1), 5.f, 15.f, 60.f);

			modelStack.PushMatrix();
			modelStack.Translate(8, 4.5,0);
			modelStack.Scale(5.5,3.5,0);
			RenderMeshin2D(meshList[GEO_KEYCOUNT], false);
			modelStack.PopMatrix();

			std::stringstream ss;
			ss << KeyCount;
			std::string keys = ss.str();
			RenderTextOnScreen(meshList[GEO_TEXT], keys, Color(1,1,0), 4.0f, 9.0f, 3.0f);

			if (P_Player.holding != NULL)
			{
				if (P_Player.holding->isGun)
				{
					WeaponsObject *WO = dynamic_cast<WeaponsObject*>(P_Player.holding);
					if (WO->animState)
					{
						Color c_crosshair(0.f, 1.f, 1.f);

						if (WO->CurrentClip <= 0)
						{
							c_crosshair.Set(1.f, 0.f, 0.f);
						}
						else if (!(WO->attackRate + firerate < timer))
						{
							c_crosshair.Set(0.f, 0.5f, 0.5f);
						}
						else
						{
							c_crosshair.Set(0.f, 1.f, 1.f);
						}

						modelStack.PushMatrix();
						modelStack.Translate(Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f, 1.f);

						modelStack.PushMatrix();
						modelStack.Translate(0, 1 + f_curRecoil * 0.5f, 0);
						RenderMeshin2D(meshList[GEO_CROSSHAIR], false, 100.f, 10.f, c_crosshair);
						modelStack.PopMatrix();

						modelStack.PushMatrix();
						modelStack.Rotate(90, 0, 0, 1);
						modelStack.Translate(0, 1 + f_curRecoil * 0.5f, 0);
						RenderMeshin2D(meshList[GEO_CROSSHAIR], false, 100.f, 10.f, c_crosshair);
						modelStack.PopMatrix();

						modelStack.PushMatrix();
						modelStack.Rotate(-90, 0, 0, 1);
						modelStack.Translate(0, 1 + f_curRecoil * 0.5f, 0);
						RenderMeshin2D(meshList[GEO_CROSSHAIR], false, 100.f, 10.f, c_crosshair);
						modelStack.PopMatrix();

						modelStack.PushMatrix();
						modelStack.Rotate(180, 0, 0, 1);
						modelStack.Translate(0, 1 + f_curRecoil * 0.5f, 0);
						RenderMeshin2D(meshList[GEO_CROSSHAIR], false, 100.f, 10.f, c_crosshair);
						modelStack.PopMatrix();

						modelStack.PopMatrix();
					}
				}
				else
				{
					modelStack.PushMatrix();
					modelStack.Translate(Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f, 1.f);
					modelStack.Rotate(-45, 0, 0, 1);
					modelStack.Scale(0.8f, 2.f, 1.f);
					RenderMeshin2D(meshList[GEO_CROSSHAIR], false, 100.f, 10.f, Color(0.8f, 0.8f, 0.8f));
					modelStack.PopMatrix();
				}
			}
			if (f_poweramount > 0)
			{
				modelStack.PushMatrix();
				modelStack.Translate(0, 0, 0.3f);
				modelStack.Scale(1, 10.f, 0.f);
				RenderMeshin2D(meshList[GEO_SCREEN_OVERLAY], false, 35.f, 10.f, Color(0.f, 0.f, 0.2f));
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0, 0, 0.4f);
				modelStack.Scale(1, f_poweramount*0.1f, 0.f);
				if (f_poweramount < 20)
				{
					RenderMeshin2D(meshList[GEO_SCREEN_OVERLAY], false, 100.f, 10.f, Color(0.5f, 0.f, 0.f));
				}
				else
				{
					RenderMeshin2D(meshList[GEO_SCREEN_OVERLAY], false, 100.f, 10.f, Color(0.f, 1.f, 1.f));
				}
				modelStack.PopMatrix();
			}

			glUniform1i(m_parameters[U_LENS_EFFECT], static_cast<GLint>(10));
			modelStack.PushMatrix();
			modelStack.Translate(Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f, 0);
			modelStack.Scale(static_cast<float>(Application::GetWindowWidth()*0.05f), static_cast<float>(Application::GetWindowHeight()*0.05f), 0.1f);
			RenderMeshin2D(meshList[GEO_SCREEN_OVERLAY], false, f_powerTint, 10.f, c_powerColor);
			modelStack.PopMatrix();
			glUniform1i(m_parameters[U_LENS_EFFECT], static_cast<GLint>(0));

			modelStack.PushMatrix();
			modelStack.Translate(Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f, 0.1f);
			modelStack.Scale(static_cast<float>(Application::GetWindowWidth()*0.05f), static_cast<float>(Application::GetWindowHeight()*0.05f), 0.2f);
			RenderMeshin2D(meshList[GEO_SCREEN_OVERLAY], false, f_playerHealthTint, 10.f, Color(0.5f, 0.f, 0.f));
			modelStack.PopMatrix();
		}
		break;
	case mainscene::GS_END:
	case mainscene::GS_PAUSED:
	{		
		modelStack.PushMatrix();
		modelStack.Translate(Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f, 0);
		modelStack.Scale(static_cast<float>(Application::GetWindowWidth()), static_cast<float>(Application::GetWindowHeight()), 0.f);
		RenderMeshin2D(meshList[GEO_SCREEN_OVERLAY], false, 70.f, 10.f, Color(0.f, 0.f, 0.f));
		modelStack.PopMatrix();

		std::stringstream ss;
		ss << "Current Level: " << currentLevel;
		modelStack.PushMatrix();
		modelStack.Translate(Application::GetWindowWidth()*0.022f, Application::GetWindowHeight()*0.05f + 10.f, 0.1f);
		modelStack.Scale(2.5f, 2.5f, 2.5f);
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), UIColor);
		modelStack.PopMatrix();
		break;
	}
	default:
		break;
	}

	RenderButtons();

	if (TESTMODE)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(static_cast<long double>(FPScounter)), Color(0, 1, 1), 2, 1, Application::GetWindowHeight()*0.1f - 1.f);
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


	/*
	RenderMeshin2D(meshList[GEO_POSITION_QUAD], false, 5, Application::GetWindowWidth()*0.095f, Application::GetWindowHeight()*0.09f);
	RenderMeshin2D(meshList[GEO_NORMAL_QUAD], false, 5, Application::GetWindowWidth()*0.095f, Application::GetWindowHeight()*0.075f);
	RenderMeshin2D(meshList[GEO_AMBIENT_QUAD], false, 5, Application::GetWindowWidth()*0.095f, Application::GetWindowHeight()*0.06f);
	RenderMeshin2D(meshList[GEO_DIFFUSE_QUAD], false, 5, Application::GetWindowWidth()*0.095f, Application::GetWindowHeight()*0.045f);
	RenderMeshin2D(meshList[GEO_SPECULAR_QUAD], false, 5, Application::GetWindowWidth()*0.095f, Application::GetWindowHeight()*0.03f);
	RenderMeshin2D(meshList[GEO_EMISSIVE_QUAD], false, 5, Application::GetWindowWidth()*0.095f, Application::GetWindowHeight()*0.015f);//*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	RenderWorldShadow();
	RenderWorldNoShadow();

	if (TESTMODE)
	{
		modelStack.PushMatrix();
		modelStack.Translate(Application::GetWindowWidth()*0.08f, Application::GetWindowHeight()*0.09f, 0);
		modelStack.Scale(5, 5, 5);
		RenderMeshin2D(meshList[GEO_LIGHT_DEPTH_QUAD], false);
		modelStack.PopMatrix();
	}

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
		m_lightDepthProj.SetToOrtho(-150, 150, -150, 150, -200, 400);
	}
	//else
	{
		//m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);
	}

	//m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y + FPC.position.y*0.1, lights[0].position.z, 0, 0, 0, 0, 1, 0);
	m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y, lights[0].position.z, lights[0].position.x + 1, lights[0].position.y - 10, lights[0].position.z + 1, 0, 1, 0);

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

	lights[0].position.x = P_Player.pos.x;
	lights[0].position.z = P_Player.pos.z;
	if (Graphics < GRA_SHIT)
	{
		RenderPassGPass();
		RenderPassGBuffer();
	}

	RenderPassLight();
	RenderPassMain();
}

/******************************************************************************/
/*!
\brief
Renders buttons
*/
/******************************************************************************/
void mainscene::RenderButtons(void)
{
	for (unsigned i = 0; i < v_buttonList.size(); ++i)
	{
		TextButton *S_MB = v_buttonList[i];
		if (S_MB->gamestate == GAMESTATE)
		{
			modelStack.PushMatrix();
			modelStack.Translate(S_MB->pos);
			modelStack.Scale(S_MB->scale);
			RenderTextOnScreen(meshList[GEO_TEXT], S_MB->text, S_MB->color);
			modelStack.PopMatrix();
		}
	}
}

/******************************************************************************/
/*!
\brief
Clears memory upon exit
*/
/******************************************************************************/
void mainscene::Exit(void)
{
	assignSave(true);
	Application::SetCursor(true);
	SE_Engine.Exit();

	if (PlayerSound != NULL)
	{
		delete PlayerSound;
		PlayerSound = NULL;
	}

	for (unsigned j = 0; j < 10; ++j)
	{
		for (unsigned i = 0; i < Dialogues[j].size(); ++i)
		{
			if (Dialogues[j][i] != NULL)
			{
				delete Dialogues[j][i];
				Dialogues[j][i] = NULL;
			}			
		}
	}

	while (v_buttonList.size() > 0)
	{
		TextButton *TB = v_buttonList.back();
		if (TB != NULL)
		{
			delete TB;
			TB = NULL;
		}
		v_buttonList.pop_back();
	}

	while (Doors.size() > 0)
	{
		Doors.pop_back();
	}

	while (Keys.size() > 0)
	{
		Keys.pop_back();
	}

	while (Stopper.size() > 0)
	{
		Stopper.pop_back();
	}

	while (PulseBombs.size() > 0)
	{
		PulseBombs.pop_back();
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

	for (unsigned i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
		{
			delete meshList[i];
		}
		meshList[i] = NULL;
	}

	glDeleteProgram(m_gPassShaderID);
	glDeleteProgram(m_gBufferShaderID);
	glDeleteProgram(m_lightShaderID);

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
	glDeleteProgram(m_gPassShaderID);
}


bool mainscene::CollisionBetween(Vector3 start, Vector3 &end)
{
	std::vector<CollisionBox> Temporary;
	Vector3 direction = (end-start).Normalized(); 
	float length = (end-start).Length(); 
	float line = 0; 

	while (line < length)
	{
		CollisionBox Test;
		Test.Type = CollisionBox::CT_SPHERE; 
		Test.radius = 5; 
		start += direction * 5; 
		Test.Position = start; 
		line += 5; 

		for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it) 
		{ 
			GameObject *go = (GameObject *)*it; 
			if (go->active) 
			{	 
				WorldObject *WO = dynamic_cast<WorldObject*>(go); 
				if(WO != NULL) 
				{ 
					if (CollisionBox::checkCollision(Test, go->collisionMesh))
						return true;
				} 
			} 
		}

	}

	return false;
}

void mainscene::CheckPlayerSound(void)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			AI *ai = dynamic_cast<AI*>(go);
			if (ai != NULL)
			{
				if (PlayerSound->heard(go->pos))
				{
					if (ai->getState() == AI::WALKING)
					{
					ai->setcurrentLookat(P_Player.pos);
					ai->setDestination(P_Player.pos);
					ai->setState(AI::ALERT);
					}
				}
			}
		}
	}
}

void mainscene::checkDoor()
{
	for (unsigned i = 0; i < Doors.size(); ++i)
	{
		if (CollisionBox::checkCollision(P_Player.collisionMesh, Doors[i]))
		{
			if (this->KeyCount > 0)
			{
				KeyCount--;
				Doors.erase(Doors.begin() + i);
				addStatus("Door Opened! Key Used!", 150);
				break;
			}
			else
			{
				SE_Engine.playSound2D(soundList[ST_STATUS], false, false);
				addStatus("Need Keycard!", 150);
				Vector3 pushBack = P_Player.vel;
				pushBack.y = 0;
				P_Player.vel = Vector3(0, 0, 0);
				P_Player.vel -= pushBack * 3;
				break;
			}
		}
	}
}

void mainscene::checkKey()
{
	for (unsigned i = 0; i < Keys.size(); ++i)
	{
		if (CollisionBox::checkCollision(P_Player.collisionMesh, Keys[i]))
		{
			KeyCount++;
			Keys.erase(Keys.begin() + i);
			addStatus("Key Obtained!", 150);
		}
	}
}

void mainscene::addStatus(std::string status, double statusTimer)
{
	this->status = status;
	this->statusTimer = statusTimer;
}

void mainscene::checkStatus()
{
	if (status != "\0")
	{
		statusTimer -= 1.0;
		if (statusTimer <= 0)
		{ 
			status = "\0";
			statusTimer = 0;
		}
	}
}

void mainscene::pushPlayer(float pushForce)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			AI *ai = dynamic_cast<AI*>(go);
			if (ai != NULL)
			{
				CollisionBox Temp = ai->collisionMesh;
				Temp.Position.y = 20;
				if (CollisionBox::checkCollision(P_Player.collisionMesh, Temp))
				{
					P_Player.vel = Vector3(0,0,0);
					P_Player.vel += ai->vel * pushForce;
				}
			}
		}
	}
}

void mainscene::layTrap(Vector3 pos)
{
	if (bombCount > 0)
	{
		float bombRadius = 100;
		CollisionBox Bomb;
		Bomb.Type = CollisionBox::CT_SPHERE;
		Bomb.radius = bombRadius;
		Bomb.Position = pos;
		PulseBombs.push_back(Bomb);
		bombCount--;
	}
	else
		addStatus("PulseBombs Depleted!", 100);
}

void mainscene::activateTrap()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			AI *ai = dynamic_cast<AI*>(go);
			if (ai != NULL)
			{
				for (unsigned i = 0; i < PulseBombs.size(); ++i)
				{
					if (CollisionBox::checkCollision(PulseBombs[i], ai->collisionMesh))
					{
						P_Player.pos.y = 80;
						SE_Engine.playSound3D(soundList[ST_AI_DEATH], ai->pos);
						ai->DropObject();
						ai->active = false;
						generateCharacterParticle(ai, go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)), go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)), go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)), go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)), go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)), go->vel*0.2f + Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(40, 50), Math::RandFloatMinMax(-10, 10)));
					}
					PulseBombs.erase(PulseBombs.begin() + i);
				}
			}
		}
	}
}

void mainscene::alertDeath(Vector3 pos, float alertRadius)
{
	CollisionBox alertBound;
	alertBound.Type = CollisionBox::CT_SPHERE;
	alertBound.radius = alertRadius;
	alertBound.Position = pos;
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			AI *ai = dynamic_cast<AI*>(go);
			if (ai != NULL)
			{
				if (CollisionBox::checkCollision(alertBound, ai->collisionMesh))
				{
					if (ai->getState() == AI::WALKING)
					{
						ai->setcurrentLookat(alertBound.Position);
						ai->setDestination(alertBound.Position);
						ai->setState(AI::ALERT);
					}
				}
			}
		}
	}
}

void mainscene::stopAI(float pushForce)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			AI *ai = dynamic_cast<AI*>(go);
			if (ai != NULL)
			{
				if (ai->getState() == AI::WALKING)
				{
					for (unsigned i = 0; i < Stopper.size(); ++i)
					{
						if (CollisionBox::checkCollision(Stopper[i], ai->collisionMesh))
						{
							Vector3 newPos = ai->pos + ai->Lookat;
							Vector3 dir = (ai->pos - newPos);
							ai->pos -= ai->vel * 1.01f;//Take it out of the new Position 
							ai->Lookat = dir;
							ai->setcurrentLookat(dir);//Goal!
							
							ai->vel = ai->getCurrentLookAt().Normalized() *pushForce;
							//ai->AiLookatRotation(dt);
						}
					}
				}
			}
		}
	}
}