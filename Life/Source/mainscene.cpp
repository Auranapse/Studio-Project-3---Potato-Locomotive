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


/******************************************************************************/
/*!
mainscene Constructor
*/
/******************************************************************************/
mainscene::mainscene() : TESTMODE(true), NUM_LIGHT_PARAM(11)
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
	SH_1.assign(f_fov, 70.f, 1);
	SH_1.assign(f_mouseSensitivity, 100.f, 2);
	f_mouseSensitivity *= 0.01f;
	SH_1.assign(us_control[E_CTRL_MOVE_FRONT], 'W', 3);
	SH_1.assign(us_control[E_CTRL_MOVE_BACK], 'S', 4);
	SH_1.assign(us_control[E_CTRL_MOVE_LEFT], 'A', 5);
	SH_1.assign(us_control[E_CTRL_MOVE_RIGHT], 'D', 6);
	SH_1.assign(us_control[E_CTRL_MOVE_SPRINT], VK_SHIFT, 7);
	SH_1.assign(us_control[E_CTRL_MOVE_WALK], VK_CONTROL, 8);
	SH_1.assign(us_control[E_CTRL_MOVE_JUMP], VK_SPACE, 9);
	SH_1.assign(us_control[E_CTRL_INTERACT], 'E', 10);
	SH_1.assign(us_control[E_CTRL_THROW], VK_RBUTTON, 11);
	SH_1.assign(us_control[E_CTRL_ATTACK], VK_LBUTTON, 12);
	SH_1.assign(us_control[E_CTRL_AIM], VK_MBUTTON, 13);
	SH_1.assign(us_control[E_CTRL_ABILITY_1], 'V', 14);
	SH_1.assign(us_control[E_CTRL_ABILITY_2], 'B', 15);
	SH_1.assign(Graphics, GRA_MAX, 16);
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
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("Crosshair", Color(0.f, 1.f, 1.f), 0.1f, 0.5f, 1.f);
	meshList[GEO_FLOOR_TILE] = MeshBuilder::GenerateQuad("Room floor", Color(1.f, 1.f, 1.f), 10.f, 10.f, 400.f);
	meshList[GEO_FLOOR_TILE]->textureID[0] = LoadTGA("GameData//Image//floortexture.tga", false);

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

	meshList[GEO_LIGHT] = MeshBuilder::GenerateSphere("THELIGHT", Color(1.0, 1.0, 1.0), 9, 18, 1);

	//Load OBJ Models
	meshList[GEO_OBJCAKE] = MeshBuilder::GenerateOBJ("CAKE", "GameData//OBJ//CAKE.obj");
	meshList[GEO_OBJCAKE]->textureID[0] = LoadTGA("GameData//Image//OBJ//Cake_UV.tga", true);

	meshList[GEO_SECURITYCAMERA] = MeshBuilder::GenerateOBJ("CAKE", "GameData//OBJ//other//SecurityCamera.obj");
	meshList[GEO_SECURITYCAMERA]->textureID[0] = LoadTGA("GameData//Image//OBJ//SecurityCamera_UV.tga", true);

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

	meshList[GEO_ITEM_SYRINGE] = MeshBuilder::GenerateOBJ("Syringe", "GameData//OBJ//Items//Syringe.obj");
	meshList[GEO_ITEM_SYRINGE]->textureID[0] = LoadTGA("GameData//Image//Items//Syringe.tga", true);

	meshList[GEO_M9]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_M9]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_M9]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_M9]->material.kShininess = 10.0f;

	meshList[GEO_SPAS12]->material = meshList[GEO_M9]->material;
	meshList[GEO_MP5K]->material = meshList[GEO_M9]->material;
	meshList[GEO_KATANA]->material = meshList[GEO_M9]->material;
	meshList[GEO_ITEM_SYRINGE]->material = meshList[GEO_M9]->material;

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


	//Lighting-------------------------------------------------------------------------------

	meshList[GEO_FLOOR_TILE]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_FLOOR_TILE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FLOOR_TILE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_FLOOR_TILE]->material.kShininess = 1.0f;

	meshList[GEO_OBJCAKE]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_OBJCAKE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_OBJCAKE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_OBJCAKE]->material.kShininess = 1.0f;

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
	P_Player.Scale.Set(10, 10, 10);

	f_step = 0.f;

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

	soundList[ST_STEP] = SE_Engine.preloadSound("GameData//sounds//other//step1.wav");
	soundList[ST_STEP_2] = SE_Engine.preloadSound("GameData//sounds//other//step2.wav");
	soundList[ST_BUZZER] = SE_Engine.preloadSound("GameData//sounds//other//buzzer.wav");
	soundList[ST_ALERT] = SE_Engine.preloadSound("GameData//sounds//other//alert.wav");

	soundList[ST_WEAPON_M9_SHOOT] = SE_Engine.preloadSound("GameData//sounds//weapons//M9//FIRE.wav");
	soundList[ST_WEAPON_M9_SHOOT]->setDefaultVolume(0.3f);

	soundList[ST_WEAPON_KATANA] = SE_Engine.preloadSound("GameData//sounds//weapons//Katana.mp3");

	soundList[ST_WEAPON_CLICK] = SE_Engine.preloadSound("GameData//sounds//weapons//empty.wav");

	soundList[ST_CAMERA_SPOTTED] = SE_Engine.preloadSound("GameData//sounds//other//EnemySpotted.mp3");
	soundList[ST_CAMERA_FOUND] = SE_Engine.preloadSound("GameData//sounds//other//Alarm.mp3");

	GAMESTATE = GS_PLAY;
	currentLevel = 1;
	loadLevel(currentLevel);

	Shape *sTest = new Sphere(Vector3(0, 0, 0), 100);
	Asset *Test = new Room(meshList[GEO_OBJCAKE], sTest, 100, true, false, 0.6f, 0.55f);
	MainManager.Add(Test);

	Shape *aTest = new Sphere(Vector3(2, 0, 5), 5);
	Asset *Test2 = new Enemy(meshList[GEO_OBJCAKE], aTest, 40, 1, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(10, 0, 0), 1, 20, 0);
	MainManager.Add(Test2);
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

	P_Player.Velocity.SetZero();
	P_Player.DropObject();
	PowerActive = false;
	f_powerTintSet = 0.f;

	while (m_charList.size() > 0)
	{
		CharacterObject *CO = m_charList.back();
		if (CO != NULL)
		{
			delete CO;
			CO = NULL;
		}
		m_charList.pop_back();
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

	while (m_ScamList.size() > 0)
	{
		SecurityCam *SC = m_ScamList.back();
		if (SC != NULL)
		{
			delete SC;
			SC = NULL;
		}
		m_ScamList.pop_back();
	}

	float worldsize = static_cast<float>(GAME_MAP.worldSize);

	std::cout << "Map Size: ";
	std::cout << GAME_MAP.map_width << ", " << GAME_MAP.map_height << "\n";

	for (unsigned y = GAME_MAP.map_height - 1; y > 0; --y)
	{
		for (unsigned x = 0; x < GAME_MAP.map_width; ++x)
		{
			if (GAME_MAP.map_data[y][x] == ".")
			{
				continue;
			}

			if (GAME_MAP.map_data[y][x] == "SPAWN")//Generate spawnpoint
			{
				P_Player.setPosition(Vector3(x*worldsize*2.f, 5.f, y*worldsize*2.f));
			}
			else if (GAME_MAP.map_data[y][x][0] == 'I')
			{
				if (GAME_MAP.map_data[y][x][1] == 'W')
				{
					WeaponsObject *WO;
					if (GAME_MAP.map_data[y][x] == "IW_M9")
					{
						WO = new WeaponsObject(WO_presetList[WO_M9]);
					}
					else if (GAME_MAP.map_data[y][x] == "IW_KATANA")
					{
						WO = new WeaponsObject(WO_presetList[WO_KATANA]);
					}

					if (WO != NULL)
					{
						WO->pos.Set(x*worldsize*2.f, 10.f, y*worldsize*2.f);
						m_goList.push_back(WO);
					}
					continue;
				}
				else if (GAME_MAP.map_data[y][x][1] == 'I')
				{
					ItemObject *IO;
					if (GAME_MAP.map_data[y][x] == "II_SYRINGE")
					{
						IO = new WeaponsObject(WO_presetList[WO_M9]);
					}

					if (IO != NULL)
					{
						IO->pos.Set(x*worldsize*2.f, 10.f, y*worldsize*2.f);
						m_goList.push_back(IO);
					}
					continue;
				}
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

				if (SizeY > worldsize * 2)
				{
					SizeY = worldsize*2.f;
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
				WO->ColBox.Set(SizeX, SizeY, SizeZ);
				WO->dynamicRendering = true;
				WO->mesh = meshList[GEO_WORLD_CUBE];
				m_goList.push_back(WO);
			}
			else if (GAME_MAP.map_data[y][x][0] == 'A')
			{
				AI *ai;
				ai = new AI(AI::WALKING, AI::AI_SCIENTIST);
				ai->Init(Vector3(x*worldsize*2.f, 0, y*worldsize*2.f), Vector3(0, 0, 0), "GameData//Image//player//PlayerSkin.tga");
				ai->Lookat = ai->getPosition() + Vector3(0, 0, 10);
				ai->Scale.Set(10, 10, 10);

				WeaponsObject *WO;
				WO = new WeaponsObject(WO_presetList[WO_M9]);
				ai->HoldObject(WO);
				m_goList.push_back(WO);
				m_charList.push_back(ai);
			}
		}
	}

	//World Floor
	WorldObject *WO;
	WO = new WorldObject();
	WO->pos.Set(GAME_MAP.map_width*0.5f*worldsize, 0.f, (GAME_MAP.map_height*0.5f)*worldsize);
	WO->scale.Set(GAME_MAP.map_width * worldsize * 2.f, 1, GAME_MAP.map_height * worldsize * 2.f);
	WO->ColBox.Set(GAME_MAP.map_width * worldsize * 2.f, 5, GAME_MAP.map_height * worldsize * 2.f);
	WO->active = true;
	WO->enablePhysics = false;
	WO->colEnable = true;
	WO->mesh = meshList[GEO_WORLD_CUBE];
	Floor = WO;
	m_goList.push_back(WO);
	//World Celling
	WO = new WorldObject();
	WO->pos.Set(GAME_MAP.map_width*0.5f*worldsize, worldsize*4.f, (GAME_MAP.map_height*0.5f)*worldsize);
	WO->rotation.x = 90.f;
	WO->scale.Set(GAME_MAP.map_width * worldsize * 2.f, GAME_MAP.map_height * worldsize * 2.f, 1);
	WO->ColBox.Set(GAME_MAP.map_width * worldsize * 2.f, 5, GAME_MAP.map_height * worldsize * 2.f);
	WO->active = true;
	WO->enablePhysics = false;
	WO->colEnable = true;
	WO->mesh = meshList[GEO_WORLD_QUAD];
	Celling = WO;
	m_goList.push_back(WO);

	WO = new WorldObject();
	WO->pos.Set(worldsize * 2.f, worldsize * 2.f, GAME_MAP.map_height * worldsize * 2.f);
	WO->scale.Set(GAME_MAP.map_width * worldsize * 2.f, worldsize * 2.2f, worldsize);
	WO->ColBox.Set(GAME_MAP.map_width * worldsize * 2.f, worldsize * 2.2f, worldsize);
	WO->active = true;
	WO->enablePhysics = false;
	WO->colEnable = true;
	WO->mesh = meshList[GEO_WORLD_CUBE];
	SWALL1 = WO;
	m_goList.push_back(WO);

	WO = new WorldObject();
	WO->pos.Set(worldsize * 2.f, worldsize * 2.f, 0.f);
	WO->scale.Set(GAME_MAP.map_width * worldsize * 2.2f, worldsize * 2.2f, worldsize);
	WO->ColBox.Set(GAME_MAP.map_width * worldsize * 2.2f, worldsize * 2.2f, worldsize);
	WO->active = true;
	WO->enablePhysics = false;
	WO->colEnable = true;
	WO->mesh = meshList[GEO_WORLD_CUBE];
	SWALL2 = WO;
	m_goList.push_back(WO);

	WO = new WorldObject();
	WO->pos.Set(-worldsize * 2.f, worldsize * 2.f, GAME_MAP.map_height * worldsize);
	WO->scale.Set(worldsize, worldsize * 2.2f, GAME_MAP.map_height * worldsize * 2.f);
	WO->ColBox.Set(worldsize, worldsize * 2.2f, GAME_MAP.map_height * worldsize * 2.f);
	WO->active = true;
	WO->enablePhysics = false;
	WO->colEnable = true;
	WO->mesh = meshList[GEO_WORLD_CUBE];
	SWALL3 = WO;
	m_goList.push_back(WO);

	WO = new WorldObject();
	WO->pos.Set(GAME_MAP.map_width * worldsize * 2.f, worldsize * 2.f, GAME_MAP.map_height * worldsize);
	WO->scale.Set(worldsize, worldsize * 2.2f, GAME_MAP.map_height * worldsize * 2.f);
	WO->ColBox.Set(worldsize, worldsize * 2.2f, GAME_MAP.map_height * worldsize * 2.f);
	WO->active = true;
	WO->enablePhysics = false;
	WO->colEnable = true;
	WO->mesh = meshList[GEO_WORLD_CUBE];
	SWALL4 = WO;
	m_goList.push_back(WO);

	lights[0].position.y = worldsize * 4.5f;

	FPC.Init(P_Player.getPosition() + P_Player.CamOffset, P_Player.getPosition() + P_Player.CamOffset + Vector3(0.f, 0.f, -1.f), Vector3(0.f, 1.f, 0.f), f_mouseSensitivity);
	std::cout << "Map Successfully loaded\n";
	return true;
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
			m_ParList[i]->active = true;
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
				BO->active = true;
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
		BO->active = true;
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
	IO_presetList[IO_SYRINGE].ColBox.Set(1, 1, 1);
	IO_presetList[IO_SYRINGE].ColBoxOffset.Set(0, 1, 0);
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
	WO_presetList[WO_M9].CurrentClip = 15;
	WO_presetList[WO_M9].recoilEffect = 30.f;
	WO_presetList[WO_M9].isGun = true;
	WO_presetList[WO_M9].isWeapon = true;
	WO_presetList[WO_M9].enablePhysics = true;
	WO_presetList[WO_M9].colEnable = true;
	WO_presetList[WO_M9].ColBox.Set(3, 3, 3);
	WO_presetList[WO_M9].AttackSound = ST_WEAPON_M9_SHOOT;

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
	WO_presetList[WO_KATANA].ColBox.Set(3, 3, 3);
	WO_presetList[WO_KATANA].AttackSound = ST_WEAPON_KATANA;

	f_curRecoil = 0.f;
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
			{
				P_Player.Velocity.y = 0;
			}
		}

		if (d_dt != d_dt2)
		{
			double tempDT = (d_dt + d_dt2) / 2;
			P_Player.Velocity += gravity_force * static_cast<float>(tempDT);
		}
		else
		{
			P_Player.Velocity += gravity_force * static_cast<float>(dt);
		}


		inAir = true;
	}
	else
	{
		if (P_Player.Velocity.y != 0)
		{
			P_Player.Velocity.y = 0.f;
		}

		if (collide(Vector3(P_Player.getPosition() + Vector3(0.f, 4.f, 0.f))))//This is to prevent floor clipping, or rather, to make it bounce back up if it's clipping
		{
			P_Player.Velocity.y = 50 * static_cast<float>(dt);
		}

		else if (collide(Vector3(P_Player.getPosition() + Vector3(0.f, 2.f, 0.f))))
		{
			P_Player.Velocity.y = 20 * static_cast<float>(dt);
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
		walkSoundDelay *= 0.5f;
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
			SE_Engine.playSound2D(soundList[ST_STEP]);
			f_step = timer;
		}
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_BACK]) && !Application::IsKeyPressed(us_control[E_CTRL_MOVE_FRONT]))
	{
		P_Player.Velocity.x -= LookDir.x;
		P_Player.Velocity.z -= LookDir.z;

		if (walkSoundDelay + f_step < timer && !inAir)
		{
			SE_Engine.playSound2D(soundList[ST_STEP]);
			f_step = timer;
		}
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_LEFT]) && !Application::IsKeyPressed(us_control[E_CTRL_MOVE_RIGHT]))
	{
		P_Player.Velocity -= RightDir;

		if (walkSoundDelay + f_step < timer && !inAir)
		{
			SE_Engine.playSound2D(soundList[ST_STEP]);
			f_step = timer;
		}
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_RIGHT]) && !Application::IsKeyPressed(us_control[E_CTRL_MOVE_LEFT]))
	{
		P_Player.Velocity += RightDir;

		if (walkSoundDelay + f_step < timer && !inAir)
		{
			SE_Engine.playSound2D(soundList[ST_STEP]);
			f_step = timer;
		}
	}

	if (Application::IsKeyPressed(us_control[E_CTRL_MOVE_JUMP]))
	{
		if (inAir == false)
		{
			P_Player.Velocity.y += 120;
			SE_Engine.playSound2D(soundList[ST_STEP]);
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
	if (collide(Vector3(P_Player.getPosition() + Vector3(10.f, 10.f, 0.f))) || collide(Vector3(P_Player.getPosition() + Vector3(10.f, 20.f, 0.f))))
	{
		if (P_Player.Velocity.x > 0)
		{
			P_Player.Velocity.x = 0;
		}
	}

	if (collide(Vector3(P_Player.getPosition() + Vector3(0.f, 10.f, 10.f))) || collide(Vector3(P_Player.getPosition() + Vector3(0.f, 20.f, 10.f))))
	{
		if (P_Player.Velocity.z > 0)
		{
			P_Player.Velocity.z = 0;
		}
	}

	if (collide(Vector3(P_Player.getPosition() + Vector3(-10.f, 10.f, 0.f))) || collide(Vector3(P_Player.getPosition() + Vector3(-10.f, 20.f, 0.f))))
	{
		if (P_Player.Velocity.x < 0)
		{
			P_Player.Velocity.x = 0;
		}
	}

	if (collide(Vector3(P_Player.getPosition() + Vector3(0.f, 10.f, -10.f))) || collide(Vector3(P_Player.getPosition() + Vector3(0.f, 20.f, -10.f))))
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
			for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				ItemObject *IO = (ItemObject *)*it;
				if (IO->active && IO->holdable)
				{
					if (isVisible(FPC.position, FPC.target, 10, IO->pos))
					{
						if (isVisible2(FPC.position, FPC.target, 10, IO->pos))
						{
							if ((IO->pos - P_Player.getPosition()).LengthSquared() < 1000)
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

	if (d_dt2 != d_dt)
	{
		double tempDT = (d_dt + d_dt2) / 2;
		FPC = FPC + (P_Player.Velocity * static_cast<float>(tempDT));
		P_Player.Lookat = FPC.target;
		P_Player.Update(tempDT);
	}
	else
	{
		FPC = FPC + (P_Player.Velocity * static_cast<float>(dt));
		P_Player.Lookat = FPC.target;
		P_Player.Update(dt);
	}

	for (std::vector<CharacterObject*>::iterator it = m_charList.begin(); it != m_charList.end(); it++)
	{
		CharacterObject *CO = (CharacterObject *)*it;
		if (CO->active)
		{
			AI *ai = dynamic_cast<AI*>(CO);
			if (ai != NULL)
			{
				ai->Update(dt, P_Player.getPosition(), m_charList, m_goList);
			}
			else
			{
				if (CO->holding != NULL)
				{
					CO->holding->Update(dt);
				}
			}
		}
	}

	/* Set AI to die
	if (TEST->active)
	{
		TEST->DropObject();
		TEST->active = false;
		generateCharacterParticle(TEST, Vector3(0, 100, 0), Vector3(0, 100, 0), Vector3(0, 100, 0), Vector3(0, 100, 0), Vector3(0, 100, 0), Vector3(0, 100, 0));
	}
	*/
}

/******************************************************************************/
/*!
\brief
Handles player powers
*/
/******************************************************************************/
void mainscene::UpdatePlayerPower(double &dt)
{
	static bool abilityPressed_1 = false;
	if (Application::IsKeyPressed(us_control[E_CTRL_ABILITY_1]) && !abilityPressed_1)
	{
		abilityPressed_1 = true;
		if (PowerActive)
		{
			PowerActive = false;
			f_powerTintSet = 0.f;
			SE_Engine.playSound2D(soundList[ST_SLOWMO_EXIT]);
		}
		else
		{
			PowerActive = true;
			CurrentPower = PT_SLOWMO;
			f_powerTintSet = 25.f;
			c_powerColor.Set(0.1f, 0.f, 0.f);
			SE_Engine.playSound2D(soundList[ST_SLOWMO_ENTER]);
		}
	}
	else if (!Application::IsKeyPressed(us_control[E_CTRL_ABILITY_1]) && abilityPressed_1)
	{
		abilityPressed_1 = false;
	}

	static bool abilityPressed_2 = false;
	if(Application::IsKeyPressed(us_control[E_CTRL_ABILITY_2]) && !abilityPressed_2)
	{
		abilityPressed_2 = true;
		if(!PowerActive)
		{
			for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				GameObject *go = (GameObject *)*it;
				if (go->active)
				{
					WorldObject *WO = dynamic_cast<WorldObject*>(go);
					if(WO != NULL && WO != Celling && WO != Floor && WO != SWALL1 && WO != SWALL2 && WO != SWALL3 && WO != SWALL4)
					{
						WO->Opacity = 10.f;
					}
				}
			}
			f_powerTintSet = 25.f;
			c_powerColor.Set(0.f, 0.f, 0.3f);
			CurrentPower  = PT_SUPERVISION;
			PowerActive = true;
		}
		else
		{
			for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				GameObject *go = (GameObject *)*it;
				if (go->active)
				{
					WorldObject *WO = dynamic_cast<WorldObject*>(go);
					if(WO != NULL && WO != Celling && WO != Floor && WO != SWALL1 && WO != SWALL2 && WO != SWALL3 && WO != SWALL4)
					{
						WO->Opacity = 100.f;
					}
				}
			}
			f_powerTintSet = 0.f;
			PowerActive = false;
		}
	}
	else if(!Application::IsKeyPressed(us_control[E_CTRL_ABILITY_2]) && abilityPressed_2)
	{
		abilityPressed_2 = false;
	}

	if (PowerActive)
	{
		switch (CurrentPower)
		{
		case mainscene::PT_SLOWMO:
			dt *= 0.05;

			if (P_Player.Velocity != 0)
			{
				float temp = P_Player.Velocity.LengthSquared() *0.002f;
				if (temp > 1)
				{
					dt *= temp;
				}

				if (dt > d_dt)
				{
					dt = d_dt;
				}
			}
			break;
		case mainscene::PT_SUPERVISION:
			break;
		default:
			break;
		}
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
			f_powerTint += diff * static_cast<float>(d_dt) * 10.f;
		}
	}
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
			BulletObject *BO = dynamic_cast<BulletObject*>(go);
			if (BO != NULL)
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
				continue;
			}

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
					if (go->gravityEnabled)
					{
						go->vel += gravity_force * static_cast<float>(dt);
					}
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
				go->Update(d_dt);
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
	float CharRotation = CalAnglefromPosition(CO->Lookat, CO->getPosition(), true);
	generateParticle(CO->getPosition() + CO->ModelPos + CO->HeadPos, CO->Scale, HeadVel, Vector3(0, CharRotation, 0), Particle::PAR_MESH, 1.f, CO->Head);

	Mtx44 Rotation;
	Rotation.SetToRotation(CharRotation, 0, 1, 0);
	Vector3 tempArm = CO->ArmPos;
	tempArm = Rotation * tempArm;
	generateParticle(CO->getPosition() + CO->ModelPos + tempArm, CO->Scale, ArmRightVel, Vector3(0, CharRotation, 0), Particle::PAR_MESH, 4.f, CO->Arm_right);
	tempArm = Vector3(-CO->ArmPos.x, CO->ArmPos.y, CO->ArmPos.z);
	tempArm = Rotation * tempArm;
	generateParticle(CO->getPosition() + CO->ModelPos + tempArm, CO->Scale, ArmLeftVel, Vector3(0, CharRotation, 0), Particle::PAR_MESH, 4.f, CO->Arm_left);

	generateParticle(CO->getPosition() + CO->ModelPos + CO->LegPos, CO->Scale, LegLeftVel, Vector3(0, CharRotation, 0), Particle::PAR_MESH, 4.f, CO->Leg_left);

	generateParticle(CO->getPosition() + CO->ModelPos + CO->LegPos, CO->Scale, LegRightVel, Vector3(0, CharRotation, 0), Particle::PAR_MESH, 4.f, CO->Leg_right);

	generateParticle(CO->getPosition() + CO->ModelPos, CO->Scale, BodyVel, Vector3(0, CharRotation, 0), Particle::PAR_MESH, 5.f, CO->Chest);

	for (unsigned i = 0; i < 64; ++i)
	{
		float bloodsize = Math::RandFloatMinMax(0.1f, .8f);
		generateParticle(CO->getPosition() + CO->ModelPos, Vector3(bloodsize, bloodsize, bloodsize), Vector3(Math::RandFloatMinMax(-70, 70), Math::RandFloatMinMax(-5, 70), Math::RandFloatMinMax(-70, 70)) + BodyVel, Vector3(0.f, 0.f, 0.f), Particle::PAR_BLOOD, 4.f);
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
void mainscene::Shoot(const Vector3 &Pos, const Vector3 &Dir, float Speed, float Longevity)
{
	BulletObject *BO;
	BO = FetchBullet();
	BO->pos = Pos;
	BO->vel = Dir * Speed;
	BO->life = Longevity;
	BO->scale.Set(0.5f, 0.5f, 0.5f);
	BO->mesh = meshList[GEO_BULLET];
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
							FPC.rotateCamVertical(static_cast<float>(dt) * WO->recoilEffect);
							Shoot(FPC.position, ShootVector.Normalize(), WO->shootvelocity, 6);
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
		if (intersect(go->pos + go->ColBox + go2->ColBox, go->pos - go->ColBox - go2->ColBox, go2->pos))
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

	SE_Engine.UpdateListenerPosition(FPC.position, (FPC.target - FPC.target), FPC.up);
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
			f_fov += diff * static_cast<float>(d_dt) * 10.f;
		}

		editFOV(f_fov);
	}

	if (Application::IsKeyPressed('1'))
	{
		GAMESTATE = GS_DEATH;
		Application::SetCursor(true);
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

	UpdateButtons();

	static bool isEscPressed = false;
	static bool isLmbPressed = false;

	switch (GAMESTATE)
	{
	case mainscene::GS_PLAY:
		UpdatePlayerPower(dt);
		d_dt2 = dt;
		timer += static_cast<float>(dt);

		UpdatePlayer(dt);
		UpdateGO(dt);
		UpdateParticles(dt);
		FPC.Update(dt);

		for (std::vector<SecurityCam*>::iterator it = m_ScamList.begin(); it != m_ScamList.end(); ++it)
		{
			SecurityCam *SC = (SecurityCam *)*it;
			if (SC->active)
			{
				SC->update(dt, P_Player.getPosition());
			}
		}

		weaponsUpdate(dt);
		UpdateSound(dt);
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
		f_powerTintSet = 100.f;
		c_powerColor.Set(0.5f, 0.f, 0.f);
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
				f_powerTint = 0.f;
				f_powerTintSet = 0.f;
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

		//SCREEN COLOUR
		if (f_powerTint != f_powerTintSet)
		{
			float diff = f_powerTintSet - f_powerTint;

			if (diff < 0.01 && diff > -0.01)
			{
				f_powerTint = f_powerTintSet;
			}
			else
			{
				f_powerTint += diff * static_cast<float>(d_dt) * 10.f;
			}
		}
		break;
	default:
		break;
	}

	MainManager.Update(dt, 1);
	//std::cout<<"Pos: "<<MainManager.SceneAssets[1]->getBound()->getOrigin().x<<std::endl;
	//Living* Whatever = (Living*)MainManager.SceneAssets[1];
	//std::cout<<"Velo: "<<Whatever->getVelo().x<<std::endl<<"Acc: "<<Whatever->getAcc().x<<std::endl<<"Force: "<<Whatever->getForce().x<<std::endl;
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
			RenderMesh(go->mesh, true, true, go->Opacity);
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
	float YRotation = CalAnglefromPosition(CO->Lookat, CO->getPosition(), true);;
	float Pitch;
	if (CO == &P_Player)
	{
		Pitch = -CalAnglefromPosition(CO->Lookat, CO->getPosition() + CO->CamOffset, false);
	}
	else
	{
		Pitch = -CalAnglefromPosition(CO->Lookat, CO->getPosition(), false);
	}


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

	if (CO == &P_Player && m_renderPass != RENDER_PASS_PRE)
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
	RenderMesh(CO->Arm_left, true);
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
	for (std::vector<CharacterObject*>::iterator it = m_charList.begin(); it != m_charList.end(); ++it)
	{
		CharacterObject *CO = (CharacterObject *)*it;
		if (CO->active)
		{
			if (isVisible(FPC.position, FPC.target, f_fov, CO->getPosition()) || (Vector3(FPC.position.x - CO->getPosition().x, 0, FPC.position.z - CO->getPosition().z)).LengthSquared() < 4000)
			{
				RenderCharacter(CO);
			}
		}
	}

	for (std::vector<SecurityCam*>::iterator it = m_ScamList.begin(); it != m_ScamList.end(); ++it)
	{
		SecurityCam *SC = (SecurityCam *)*it;
		if (SC->active)
		{
			modelStack.PushMatrix();
			modelStack.Translate(SC->pos);
			modelStack.Scale(SC->scale);
			RenderMesh(meshList[GEO_SECURITYCAMERA], true);
			modelStack.PopMatrix();
		}
	}

	//Render gameobjects
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if (m_goList.size() < 100)
			{
				RenderGO(go);
			}
			else if (isVisible(FPC.position, FPC.target, f_fov + go->ColBox.x, go->pos) || (Vector3(FPC.position.x - go->pos.x, 0, FPC.position.z - go->pos.z)).LengthSquared() < 4000)//Dynamic rendering
			{
				RenderGO(go);
			}
		}
	}
	
	RenderCharacter(&P_Player);
	RenderParticles();
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
		modelStack.Translate(Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f, 0);
		modelStack.Scale(static_cast<float>(Application::GetWindowWidth()*0.05f), static_cast<float>(Application::GetWindowHeight()*0.05f), 0.f);
		RenderMeshin2D(meshList[GEO_SCREEN_OVERLAY], false, f_powerTint, 10.f, c_powerColor);
		modelStack.PopMatrix();
		break;
	case mainscene::GS_PLAY:
		glUniform1i(m_parameters[U_LENS_EFFECT], static_cast<GLint>(10));
		modelStack.PushMatrix();
		modelStack.Translate(Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f, 0);
		modelStack.Scale(static_cast<float>(Application::GetWindowWidth()*0.05f), static_cast<float>(Application::GetWindowHeight()*0.05f), 0.f);
		RenderMeshin2D(meshList[GEO_SCREEN_OVERLAY], false, f_powerTint, 10.f, c_powerColor);
		modelStack.PopMatrix();
		glUniform1i(m_parameters[U_LENS_EFFECT], static_cast<GLint>(0));
		break;
	case mainscene::GS_END:
	case mainscene::GS_PAUSED:
		modelStack.PushMatrix();
		modelStack.Translate(Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f, 0);
		modelStack.Scale(static_cast<float>(Application::GetWindowWidth()), static_cast<float>(Application::GetWindowHeight()), 0.f);
		RenderMeshin2D(meshList[GEO_SCREEN_OVERLAY], false, 70.f, 10.f, Color(0.f, 0.f, 0.f));
		modelStack.PopMatrix();
		break;
	default:
		break;
	}

	RenderButtons();

	if (P_Player.holding != NULL)
	{
		if (P_Player.holding->isGun)
		{
			WeaponsObject *WO = dynamic_cast<WeaponsObject*>(P_Player.holding);
			if (WO->animState)
			{
				modelStack.PushMatrix();
				modelStack.Translate(Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f, 1.f);

				modelStack.PushMatrix();
				modelStack.Translate(0, 1 + f_curRecoil * 0.5f, 0);
				RenderMeshin2D(meshList[GEO_CROSSHAIR], false);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Rotate(90, 0, 0, 1);
				modelStack.Translate(0, 1 + f_curRecoil * 0.5f, 0);
				RenderMeshin2D(meshList[GEO_CROSSHAIR], false);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Rotate(-90, 0, 0, 1);
				modelStack.Translate(0, 1 + f_curRecoil * 0.5f, 0);
				RenderMeshin2D(meshList[GEO_CROSSHAIR], false);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Rotate(180, 0, 0, 1);
				modelStack.Translate(0, 1 + f_curRecoil * 0.5f, 0);
				RenderMeshin2D(meshList[GEO_CROSSHAIR], false);
				modelStack.PopMatrix();

				modelStack.PopMatrix();
			}
		}
	}

	if (DisplayInfo)
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

	lights[0].position.x = P_Player.getPosition().x;
	lights[0].position.z = P_Player.getPosition().z;
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
	Application::SetCursor(true);
	SE_Engine.Exit();

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

	while (m_charList.size() > 0)
	{
		CharacterObject *CO = m_charList.back();
		if (CO != NULL)
		{
			delete CO;
			CO = NULL;
		}
		m_charList.pop_back();
	}

	while (m_ScamList.size() > 0)
	{
		SecurityCam *SC = m_ScamList.back();
		if (SC != NULL)
		{
			delete SC;
			SC = NULL;
		}
		m_ScamList.pop_back();
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