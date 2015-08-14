/****************************************************************************/
/*!
\file MenuScene.cpp
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Main menu for the openGL framework
!*/
/****************************************************************************/
#include "MenuScene.h"
#include "GL\glew.h"

#include <cstdlib>
#include <ctime>

#include "shader.hpp"
#include "Mtx44.h"
#include "MyMath.h"
#include <math.h>

#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include <sstream>

const unsigned int MenuScene::ui_NUM_LIGHT_PARAMS = MenuScene::E_UNI_LIGHT0_EXPONENT - (MenuScene::E_UNI_LIGHT0_POSITION - 1/*Minus the enum before this*/);
/******************************************************************************/
/*!
\brief
Default Constructor
*/
/******************************************************************************/
MenuScene::MenuScene() : ui_NUM_LIGHTS(1)
{
	P_lightsArray = new Light[ui_NUM_LIGHTS];
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
MenuScene::~MenuScene()
{

}

/******************************************************************************/
/*!
\brief
Assigns file to values
*/
/******************************************************************************/
void MenuScene::assignsave(void)
{
	if (SF_1.Data.size() == 0)
	{
		std::string *outputData;

		outputData = new std::string;
		*outputData = "70";//FOV
		SF_1.Data.push_back(outputData);

		outputData = new std::string;
		*outputData = "87";
		SF_1.Data.push_back(outputData);

		outputData = new std::string;
		*outputData = "83";
		SF_1.Data.push_back(outputData);

		outputData = new std::string;
		*outputData = "65";
		SF_1.Data.push_back(outputData);

		outputData = new std::string;
		*outputData = "68";
		SF_1.Data.push_back(outputData);

		outputData = new std::string;
		*outputData = "16";
		SF_1.Data.push_back(outputData);

		outputData = new std::string;
		*outputData = "17";
		SF_1.Data.push_back(outputData);

		outputData = new std::string;
		*outputData = "32";
		SF_1.Data.push_back(outputData);

		outputData = new std::string;
		*outputData = "69";
		SF_1.Data.push_back(outputData);

		outputData = new std::string;
		*outputData = "70";
		SF_1.Data.push_back(outputData);

		outputData = new std::string;
		*outputData = "1";
		SF_1.Data.push_back(outputData);

		outputData = new std::string;
		*outputData = "2";
		SF_1.Data.push_back(outputData);

		outputData = new std::string;
		*outputData = "81";
		SF_1.Data.push_back(outputData);

		outputData = new std::string;
		*outputData = "82";
		SF_1.Data.push_back(outputData);

		outputData = new std::string;
		*outputData = "100";
		SF_1.Data.push_back(outputData);
	}

	f_fov = static_cast<float>(std::stoi(*SF_1.Data[0]));
	f_mouseSensitivity = static_cast<float>(std::stoi(*SF_1.Data[14]));
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
Saves the file
*/
/******************************************************************************/
void MenuScene::saveGame(void)
{
	*SF_1.Data[0] = std::to_string(static_cast<long double>(f_fov));
	*SF_1.Data[14] = std::to_string(static_cast<long double>(f_mouseSensitivity));

	std::stringstream ss;

	for (unsigned i = 1; i <= 13; ++i)
	{
		ss << us_control[E_CTRL_MOVE_FRONT + i - 1];
		*SF_1.Data[i] = ss.str();
		ss.str("");
	}

	SF_1.saveData();
}

/******************************************************************************/
/*!
\brief
Initialize default variables, create meshes, lighting
*/
/******************************************************************************/
void MenuScene::Init()
{
	f_fov = 0;
	f_mouseSensitivity = 0;

	for (size_t i = 0; i < E_CTRL_TOTAL; ++i)
	{
		us_control[i] = 0;
	}

	SF_1.init("GameData//playerdata.wtf");
	assignsave();

	InitShaders();
	//Starting position of translations and initialize physics

	f_LogoScreenTimer = 0;
	MousePosX = 0.f;
	MousePosY = 0.f;
	v3_MenuCam.SetZero();
	InitMenu();

	MENU_STATE = E_M_SPLASH;
}

/******************************************************************************/
/*!
\brief
Initialize shaders
*/
/******************************************************************************/
void MenuScene::InitShaders()
{
	// Init VBO here
	// Set background color to whatever
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

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
	glGenVertexArrays(1, &u_m_vertexArrayID);
	glBindVertexArray(u_m_vertexArrayID);

	// Initialize Materials
	InitShadersAndLights();

	//Set projection matrix to perspective mode
	Mtx44 projection;
	projection.SetToPerspective(f_fov, static_cast<double>(Application::GetWindowWidth()) / static_cast<double>(Application::GetWindowHeight()), 0.1f, 10000.0f); //FOV, Aspect Ratio, Near plane, Far plane
	projectionStack.LoadMatrix(projection);

	// Init Camera
	camera.Init(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));

	//---------------------------------------------------------------------------------------

	InitMeshList();
}

/******************************************************************************/
/*!
\brief
Initializes the meshes that is in the P_meshArray
*/
/******************************************************************************/
void MenuScene::InitMeshList()
{
	P_meshArray[E_GEO_AXES] = MeshBuilder::GenerateAxes("AXES", 10000, 10000, 10000);

	//Text
	P_meshArray[E_GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	P_meshArray[E_GEO_TEXT]->textureID[0] = LoadTGA("GameData//Image//font//inputm.tga", false, false);

	P_meshArray[E_GEO_BACKGROUND] = MeshBuilder::GenerateQuad("Background", Color(1.0f, 1.0f, 1.0f), static_cast<float>(Application::GetWindowWidth() / 2), static_cast<float>(Application::GetWindowHeight() / 2), 1.0f);
	P_meshArray[E_GEO_BACKGROUND]->textureID[0] = LoadTGA("GameData//Image//UI//Background.tga", true, false);

	P_meshArray[E_GEO_SPLASH] = MeshBuilder::GenerateQuad("PentaFeedStudios", Color(1.0f, 1.0f, 1.0f), static_cast<float>(Application::GetWindowWidth() / 2), static_cast<float>(Application::GetWindowHeight() / 2), 1.0f);
	P_meshArray[E_GEO_SPLASH]->textureID[0] = LoadTGA("GameData//Image//UI//Logo.tga", true);

	P_meshArray[E_GEO_LOADING_BACKGROUND] = MeshBuilder::GenerateQuad("Loading Screen", Color(1.0f, 1.0f, 1.0f), static_cast<float>(Application::GetWindowWidth() / 2), static_cast<float>(Application::GetWindowHeight() / 2), 1.0f);
	P_meshArray[E_GEO_LOADING_BACKGROUND]->textureID[0] = LoadTGA("GameData//Image//UI//Loading.tga", true);
}

/******************************************************************************/
/*!
\brief
Initializes menu
*/
/******************************************************************************/
void MenuScene::InitMenu(void)
{
	UIColor.Set(0.8f, 0.8f, 0.8f);

	v3_Menupos[E_M_SPLASH].Set(-100, -100, 0);
	v3_Menupos[E_M_MAIN].Set(0, 0, 0);
	v3_Menupos[E_M_LOADING] = v3_Menupos[E_M_MAIN];
	v3_Menupos[E_M_OPTIONS].Set(0, 1000, 0);
	v3_Menupos[E_M_OPTIONS_CONTROLS].Set(200, 1000, 0);
	v3_Menupos[E_M_OPTIONS_CONTROLS_SETCONTROL].Set(200, 2000, 0);

	transcomplete = false;

	//MAIN--------------------------------------------------------
	S_BUTTON* S_MB;
	S_MB = new S_BUTTON;
	S_MB->pos.Set(Application::GetWindowWidth()*0.22f, Application::GetWindowHeight()*0.5f, 0.1f);
	S_MB->scale.Set(35, 35, 35);
	S_MB->text = "Play";
	S_MB->gamestate = E_M_MAIN;
	v_buttonList.push_back(S_MB);

	S_MB = new S_BUTTON;
	S_MB->pos.Set(Application::GetWindowWidth()*0.22f - 4.f, Application::GetWindowHeight()*0.5f - 60.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = "Options";
	S_MB->gamestate = E_M_MAIN;
	v_buttonList.push_back(S_MB);

	S_MB = new S_BUTTON;
	S_MB->pos.Set(Application::GetWindowWidth()*0.22f - 4.f, Application::GetWindowHeight()*0.5f - 90.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = "Quit";
	S_MB->gamestate = E_M_MAIN;
	v_buttonList.push_back(S_MB);

	S_MB = new S_BUTTON;
	S_MB->pos.Set(Application::GetWindowWidth()*0.15f, Application::GetWindowHeight()*0.15f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = "Back";
	S_MB->menubypass = true;
	S_MB->gamestate = E_M_OPTIONS;
	v_buttonList.push_back(S_MB);

	//OPTIONS-----------------------------------------------------
	S_MB = new S_BUTTON;
	S_MB->pos.Set(Application::GetWindowWidth()*0.22f - 4.f, Application::GetWindowHeight()*0.5f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = "Controls";
	S_MB->gamestate = E_M_OPTIONS;
	v_buttonList.push_back(S_MB);
}

/******************************************************************************/
/*!
\brief
Gets the button
*/
/******************************************************************************/
S_BUTTON* MenuScene::FetchBUTTON(std::string name)
{
	for (std::vector<S_BUTTON*>::iterator it = v_buttonList.begin(); it != v_buttonList.end(); ++it)
	{
		S_BUTTON *S_MB = (S_BUTTON *)*it;
		if (S_MB->text == name)
		{
			return S_MB;
		}
	}

	return NULL;
}

/******************************************************************************/
/*!
\brief
Update button state
*/
/******************************************************************************/
void MenuScene::UpdateButtons(void)
{
	for (std::vector<S_BUTTON*>::iterator it = v_buttonList.begin(); it != v_buttonList.end(); ++it)
	{
		S_BUTTON *S_MB = (S_BUTTON *)*it;
		if (S_MB->gamestate == MENU_STATE || (S_MB->menubypass && MENU_STATE != E_M_MAIN) && (S_MB->menubypass && MENU_STATE != E_M_SPLASH) && (S_MB->menubypass && MENU_STATE != E_M_LOADING))
		{
			Vector3 offset = v3_Menupos[MENU_STATE];

			if (intersect2D((S_MB->pos + Vector3(S_MB->text.length() * (S_MB->scale.x) - S_MB->scale.x, S_MB->scale.y*0.4f, 0)) + offset, S_MB->pos + Vector3(-S_MB->scale.x*0.5f, -(S_MB->scale.y*0.4f), 0) + offset, Vector3(MousePosX, MousePosY, 0)))
			{
				S_MB->active = true;
				S_MB->color.Set(1, 1, 1);
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
void MenuScene::Update(double dt)	//TODO: Reduce complexity of MenuScene::Update()
{
	//Mouse Section
	double x, y;
	Application::GetMousePos(x, y);
	MousePosX = static_cast<float>(x) / Application::GetWindowWidth() * Application::GetWindowWidth() + v3_MenuCam.x;
	MousePosY = (Application::GetWindowHeight() - static_cast<float>(y)) / Application::GetWindowHeight() * Application::GetWindowHeight() + v3_MenuCam.y;//*/

	static bool bLButtonState = false;

	UpdateButtons();

	if (v3_MenuCam != v3_Menupos[MENU_STATE])
	{
		transcomplete = false;

		Vector3 diff = v3_Menupos[MENU_STATE] - v3_MenuCam;
		v3_MenuCam += diff * static_cast<float>(dt) * 6.f;

		if (diff.x < 0.15f && diff.y < 0.15f && diff.x > -0.15f && diff.y > -0.15f)
		{
			v3_MenuCam = v3_Menupos[MENU_STATE];
		}
	}
	else
	{
		transcomplete = true;
	}

	switch (MENU_STATE)
	{
	case E_M_LOADING:
	{
		e_nextScene = Application::E_SCENE_MAIN2;
		break;
	}
	case E_M_MAIN:
	{
		if (!bLButtonState && Application::IsKeyPressed(VK_LBUTTON))
		{
			bLButtonState = true;
		}
		if (bLButtonState && !Application::IsKeyPressed(VK_LBUTTON))
		{
			bLButtonState = false;

			if (FetchBUTTON("Play")->active)
			{
				MENU_STATE = E_M_LOADING;
			}
			else if (FetchBUTTON("Options")->active)
			{
				PREV_STATE = MENU_STATE;
				MENU_STATE = E_M_OPTIONS;
			}
			else if (FetchBUTTON("Quit")->active)
			{
				e_nextScene = Application::E_SCENE_QUIT;
			}
		}
		break;
	}
	case E_M_OPTIONS:
	{
		if (!bLButtonState && Application::IsKeyPressed(VK_LBUTTON))
		{
			bLButtonState = true;
		}
		if (bLButtonState && !Application::IsKeyPressed(VK_LBUTTON))
		{
			bLButtonState = false;

			if (FetchBUTTON("Controls")->active)
			{
				MENU_STATE = E_M_OPTIONS_CONTROLS;
			}
			else if (FetchBUTTON("Back")->active)
			{
				MENU_STATE = E_M_MAIN;
			}
		}
		break;
	}
	case E_M_OPTIONS_CONTROLS:
	{
		if (!bLButtonState && Application::IsKeyPressed(VK_LBUTTON))
		{
			bLButtonState = true;
		}
		if (bLButtonState && !Application::IsKeyPressed(VK_LBUTTON))
		{
			bLButtonState = false;

			if (FetchBUTTON("Back")->active)
			{
				MENU_STATE = E_M_OPTIONS;
			}
		}
		break;
	}
	case E_M_OPTIONS_CONTROLS_SETCONTROL:
	{
		for (size_t i = VK_LBUTTON; i < VK_OEM_CLEAR; ++i)
		{
			if (((GetAsyncKeyState(i) & 0x8001) != 0) == true)
			{
				if (i == 160 || i == 161)
				{
					i = 16;
				}
				if (i == 162 || i == 163)
				{
					i = 17;
				}

				*us_ControlChange = i;
				UpdateControlSettingLabels(i, i_ControlChange);
				MENU_STATE = E_M_OPTIONS_CONTROLS;
			}
		}
		break;
	}
	case E_M_SPLASH:
	{
		f_LogoScreenTimer += static_cast<float>(dt);

		if (f_LogoScreenTimer > 1.0f)
		{
			MENU_STATE = E_M_MAIN;
		}
		break;
	}
	}
}

/******************************************************************************/
/*!
\brief
Updates control display in options menu
*/
/******************************************************************************/
void MenuScene::UpdateControlSettingLabels(unsigned short key, int button)
{
	char temp_char = static_cast<char>(key);
	std::stringstream ss;
	std::string s;
	ss << temp_char;
	ss >> s;
	if (key == 16)
	{
		//scrButton[button].SetText("SH");
	}
	else if (key == 17)
	{
		//scrButton[button].SetText("CN");
	}
	else if (key == 32)
	{
		//scrButton[button].SetText("SP");
	}
	else if (key == 1)
	{
		//scrButton[button].SetText("LM");
	}
	else if (key == 2)
	{
		//scrButton[button].SetText("RM");
	}
	else
	{
		//scrButton[button].SetText(s);
	}
}

/******************************************************************************/
/*!
\brief
Checks if buttons are pressed

\return FOV value
*/
/******************************************************************************/
void MenuScene::UpdateFOV()
{
	if (Application::IsKeyPressed('K'))
	{
		if (f_fov >= 44.f && f_fov < 110.f)
		{
			f_fov += 1.f;
		}
	}
	if (Application::IsKeyPressed('L'))
	{
		if (f_fov <= 111.f && f_fov > 45.f)
		{
			f_fov -= 1.f;
		}
	}

	Mtx44 proj;
	proj.SetToPerspective(f_fov, 16.0f / 9.0f, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(proj);
}

/******************************************************************************/
/*!
\brief
Initializes all the Shaders & Lights
*/
/******************************************************************************/
void MenuScene::InitShadersAndLights(void)
{
	//Load vertex and fragment shaders
	u_m_programID = LoadShaders("GameData//Shader//comg.vertexshader", "GameData//Shader//comg.fragmentshader");
	glUseProgram(u_m_programID);

	// Get a handle for our "colorTexture" uniform
	u_m_parameters[E_UNI_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(u_m_programID, "colorTextureEnabled");
	u_m_parameters[E_UNI_COLOR_TEXTURE] = glGetUniformLocation(u_m_programID, "colorTexture");

	// Get a handle for our "textColor" uniform
	u_m_parameters[E_UNI_TEXT_ENABLED] = glGetUniformLocation(u_m_programID, "textEnabled");
	u_m_parameters[E_UNI_TEXT_COLOR] = glGetUniformLocation(u_m_programID, "textColor");

	// Get a handle for our "MVP" uniform
	u_m_parameters[E_UNI_MVP] = glGetUniformLocation(u_m_programID, "MVP");
	u_m_parameters[E_UNI_MODELVIEW] = glGetUniformLocation(u_m_programID, "MV");
	u_m_parameters[E_UNI_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(u_m_programID, "MV_inverse_transpose");
	u_m_parameters[E_UNI_MATERIAL_AMBIENT] = glGetUniformLocation(u_m_programID, "material.kAmbient");
	u_m_parameters[E_UNI_MATERIAL_DIFFUSE] = glGetUniformLocation(u_m_programID, "material.kDiffuse");
	u_m_parameters[E_UNI_MATERIAL_SPECULAR] = glGetUniformLocation(u_m_programID, "material.kSpecular");
	u_m_parameters[E_UNI_MATERIAL_SHININESS] = glGetUniformLocation(u_m_programID, "material.kShininess");

	std::string code[ui_NUM_LIGHT_PARAMS] =
	{
		"lights[0].position_cameraspace",
		"lights[0].color",
		"lights[0].power",
		"lights[0].kC",
		"lights[0].kL",
		"lights[0].kQ",
		"lights[0].type",
		"lights[0].spotDirection",
		"lights[0].cosCutoff",
		"lights[0].cosInner",
		"lights[0].exponent"
	};

	for (size_t i = 0; i < ui_NUM_LIGHTS; ++i)		// For every light
	{
		if (i != 0)
		{
			// Loop to update array index of strings
			for (size_t param = 0; param < ui_NUM_LIGHT_PARAMS; ++param)		// For every param for each light
			{
				code[param][7] = static_cast<char>(i + 48) /*convert into char*/;
			}
		}

		u_m_parameters[E_UNI_LIGHT0_POSITION + i * ui_NUM_LIGHT_PARAMS] = glGetUniformLocation(u_m_programID, code[0].c_str());
		u_m_parameters[E_UNI_LIGHT0_COLOR + i * ui_NUM_LIGHT_PARAMS] = glGetUniformLocation(u_m_programID, code[1].c_str());
		u_m_parameters[E_UNI_LIGHT0_POWER + i * ui_NUM_LIGHT_PARAMS] = glGetUniformLocation(u_m_programID, code[2].c_str());
		u_m_parameters[E_UNI_LIGHT0_KC + i * ui_NUM_LIGHT_PARAMS] = glGetUniformLocation(u_m_programID, code[3].c_str());
		u_m_parameters[E_UNI_LIGHT0_KL + i * ui_NUM_LIGHT_PARAMS] = glGetUniformLocation(u_m_programID, code[4].c_str());
		u_m_parameters[E_UNI_LIGHT0_KQ + i * ui_NUM_LIGHT_PARAMS] = glGetUniformLocation(u_m_programID, code[5].c_str());
		u_m_parameters[E_UNI_LIGHT0_TYPE + i * ui_NUM_LIGHT_PARAMS] = glGetUniformLocation(u_m_programID, code[6].c_str());
		u_m_parameters[E_UNI_LIGHT0_SPOTDIRECTION + i * ui_NUM_LIGHT_PARAMS] = glGetUniformLocation(u_m_programID, code[7].c_str());
		u_m_parameters[E_UNI_LIGHT0_COSCUTOFF + i * ui_NUM_LIGHT_PARAMS] = glGetUniformLocation(u_m_programID, code[8].c_str());
		u_m_parameters[E_UNI_LIGHT0_COSINNER + i * ui_NUM_LIGHT_PARAMS] = glGetUniformLocation(u_m_programID, code[9].c_str());
		u_m_parameters[E_UNI_LIGHT0_EXPONENT + i * ui_NUM_LIGHT_PARAMS] = glGetUniformLocation(u_m_programID, code[10].c_str());
	}

	u_m_parameters[E_UNI_LIGHTENABLED] = glGetUniformLocation(u_m_programID, "lightEnabled");
	u_m_parameters[E_UNI_NUMLIGHTS] = glGetUniformLocation(u_m_programID, "numLights");

	glUniform1i(u_m_parameters[E_UNI_NUMLIGHTS], ui_NUM_LIGHTS);

	//Main Lighting
	P_lightsArray[0].type = LIGHT_POINT;
	P_lightsArray[0].position.Set(80, 350, 100);
	P_lightsArray[0].color.Set(1.f, 1.f, 0.98f);
	P_lightsArray[0].power = 1.5f;
	P_lightsArray[0].kC = 1.0f;
	P_lightsArray[0].kL = 0.0005f;
	P_lightsArray[0].kQ = 0.000001f;
	P_lightsArray[0].cosCutoff = cos(Math::DegreeToRadian(50));
	P_lightsArray[0].cosInner = cos(Math::DegreeToRadian(20));
	P_lightsArray[0].exponent = 3.f;
	P_lightsArray[0].spotDirection.Set(0.f, 1.f, 0.f);

	// Make sure you pass uniform parameters after glUseProgram()
	for (size_t i = 0; i < ui_NUM_LIGHTS; ++i)
	{
		glUniform1i(u_m_parameters[E_UNI_LIGHT0_TYPE + ui_NUM_LIGHT_PARAMS * i], P_lightsArray[i].type);
		glUniform3fv(u_m_parameters[E_UNI_LIGHT0_COLOR + ui_NUM_LIGHT_PARAMS * i], 1, &P_lightsArray[i].color.r);
		glUniform1f(u_m_parameters[E_UNI_LIGHT0_POWER + ui_NUM_LIGHT_PARAMS * i], P_lightsArray[i].power);
		glUniform1f(u_m_parameters[E_UNI_LIGHT0_KC + ui_NUM_LIGHT_PARAMS * i], P_lightsArray[i].kC);
		glUniform1f(u_m_parameters[E_UNI_LIGHT0_KL + ui_NUM_LIGHT_PARAMS * i], P_lightsArray[i].kL);
		glUniform1f(u_m_parameters[E_UNI_LIGHT0_KQ + ui_NUM_LIGHT_PARAMS * i], P_lightsArray[i].kQ);
		glUniform1f(u_m_parameters[E_UNI_LIGHT0_COSCUTOFF + ui_NUM_LIGHT_PARAMS * i], P_lightsArray[i].cosCutoff);
		glUniform1f(u_m_parameters[E_UNI_LIGHT0_COSINNER + ui_NUM_LIGHT_PARAMS * i], P_lightsArray[i].cosInner);
		glUniform1f(u_m_parameters[E_UNI_LIGHT0_EXPONENT + ui_NUM_LIGHT_PARAMS * i], P_lightsArray[i].exponent);
	}
}

/******************************************************************************/
/*!
\brief
Renders mesh

\param mesh - pointer to a mesh that will be rendered
\param enableLight - should the mesh rendered be affected by light?
*/
/******************************************************************************/
void MenuScene::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(u_m_parameters[E_UNI_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight)
	{
		glUniform1i(u_m_parameters[E_UNI_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(u_m_parameters[E_UNI_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(u_m_parameters[E_UNI_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(u_m_parameters[E_UNI_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(u_m_parameters[E_UNI_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(u_m_parameters[E_UNI_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(u_m_parameters[E_UNI_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(u_m_parameters[E_UNI_LIGHTENABLED], 0);
	}

	if (mesh->textureID[0] > 0)
	{
		glUniform1i(u_m_parameters[E_UNI_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
		glUniform1i(u_m_parameters[E_UNI_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(u_m_parameters[E_UNI_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once 
	if (mesh->textureID[0] > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

/******************************************************************************/
/*!
\brief
Renders text

\param mesh - Pointer to a mesh with a font texture
\param text - String to be printed
\param color - The colour of the text to be printed
*/
/******************************************************************************/
void MenuScene::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(u_m_parameters[E_UNI_TEXT_ENABLED], 1);
	glUniform3fv(u_m_parameters[E_UNI_TEXT_COLOR], 1, &color.r);
	glUniform1i(u_m_parameters[E_UNI_LIGHTENABLED], 0);
	glUniform1i(u_m_parameters[E_UNI_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(u_m_parameters[E_UNI_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(u_m_parameters[E_UNI_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(u_m_parameters[E_UNI_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

/******************************************************************************/
/*!
\brief
Renders text around the center

\param mesh - Pointer to a mesh with a font texture
\param text - String to be printed
\param color - The colour of the text to be printed
*/
/******************************************************************************/
void MenuScene::RenderTextCenter(Mesh* mesh, std::string text, Color color)
{
	modelStack.PushMatrix();
	modelStack.Translate(-(text.size() / 2.0f), 0.0f, 0.0f);
	RenderText(mesh, text, color);
	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Renders text around the center on the screen

\param mesh - Pointer to a mesh with a font texture
\param text - String to be printed
\param color - The colour of the text to be printed
*/
/******************************************************************************/
void MenuScene::RenderTextCenterOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	x -= ((text.length() - 1.5f) / 2.0f) * size;
	//RenderTextOnScreen(mesh, text, color, size, x, y);
}

/******************************************************************************/
/*!
\brief
Renders text on screen
*/
/******************************************************************************/
void MenuScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

	//Add these code just after glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::GetWindowWidth(), 0, Application::GetWindowHeight(), -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	//modelStack.LoadIdentity(); //Reset modelStack

	glUniform1i(u_m_parameters[E_UNI_TEXT_ENABLED], 1);
	glUniform3fv(u_m_parameters[E_UNI_TEXT_COLOR], 1, &color.r);
	glUniform1i(u_m_parameters[E_UNI_LIGHTENABLED], 0);
	glUniform1i(u_m_parameters[E_UNI_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(u_m_parameters[E_UNI_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(u_m_parameters[E_UNI_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(u_m_parameters[E_UNI_TEXT_ENABLED], 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

/******************************************************************************/
/*!
\brief
Renders a mesh on screen
*/
/******************************************************************************/
void MenuScene::RenderMeshOnScreen(Mesh* mesh, float sizeX, float sizeY, float x, float y)
{
	x /= sizeX;
	y /= sizeY;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(u_m_parameters[E_UNI_LIGHTENABLED], 0);
	glUniform1i(u_m_parameters[E_UNI_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(u_m_parameters[E_UNI_COLOR_TEXTURE], 0);

	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::GetWindowWidth(), 0, Application::GetWindowHeight(), -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizeX, sizeY, sizeX);

	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(u_m_parameters[E_UNI_MVP], 1, GL_FALSE, &MVP.a[0]);

	mesh->Render();

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(u_m_parameters[E_UNI_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

/******************************************************************************/
/*!
\brief
Renders the menu buttons
*/
/******************************************************************************/
void MenuScene::RenderButtons(void)
{
	for (unsigned i = 0; i < v_buttonList.size(); ++i)
	{
		S_BUTTON *S_MB = v_buttonList[i];
		if (S_MB->gamestate == MENU_STATE || (S_MB->menubypass && MENU_STATE != E_M_MAIN) && (S_MB->menubypass && MENU_STATE != E_M_SPLASH) && (S_MB->menubypass && MENU_STATE != E_M_LOADING))
		{
			modelStack.PushMatrix();
			modelStack.Translate(v3_Menupos[MENU_STATE]);
			modelStack.Translate(S_MB->pos);
			modelStack.Scale(S_MB->scale);
			RenderTextOnScreen(P_meshArray[E_GEO_TEXT], S_MB->text, S_MB->color);
			modelStack.PopMatrix();
		}
	}
}

/******************************************************************************/
/*!
\brief
Renders the scene
*/
/******************************************************************************/
void MenuScene::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (size_t i = 0; i < ui_NUM_LIGHTS; ++i)
	{
		if (P_lightsArray[i].type == LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(P_lightsArray[i].position.x, P_lightsArray[i].position.y, P_lightsArray[i].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(u_m_parameters[E_UNI_LIGHT0_POSITION + (i * ui_NUM_LIGHT_PARAMS)], 1, &lightDirection_cameraspace.x);
		}
		else if (P_lightsArray[i].type == LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * P_lightsArray[i].position;
			glUniform3fv(u_m_parameters[E_UNI_LIGHT0_POSITION + (i * ui_NUM_LIGHT_PARAMS)], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * P_lightsArray[i].spotDirection;
			glUniform3fv(u_m_parameters[E_UNI_LIGHT0_SPOTDIRECTION + (i * ui_NUM_LIGHT_PARAMS)], 1, &spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() * P_lightsArray[i].position;
			glUniform3fv(u_m_parameters[E_UNI_LIGHT0_POSITION + (i * ui_NUM_LIGHT_PARAMS)], 1, &lightPosition_cameraspace.x);
		}

		Position lightPosition_cameraspace = viewStack.Top() * P_lightsArray[i].position;
		glUniform3fv(u_m_parameters[E_UNI_LIGHT0_POSITION + (i * ui_NUM_LIGHT_PARAMS)], 1, &lightPosition_cameraspace.x);
	}

	viewStack.LoadIdentity();

	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);

	modelStack.LoadIdentity();

	modelStack.PushMatrix();
	modelStack.Translate(-v3_MenuCam);

	RenderMeshOnScreen(P_meshArray[E_GEO_BACKGROUND], 1.f, 1.f, static_cast<float>(Application::GetWindowWidth() / 2), static_cast<float>(Application::GetWindowHeight() / 2));

	switch (MENU_STATE)
	{
	case E_M_LOADING:
	{
		RenderMeshOnScreen(P_meshArray[E_GEO_LOADING_BACKGROUND], 1.f, 1.f, static_cast<float>(Application::GetWindowWidth() / 2), static_cast<float>(Application::GetWindowHeight() / 2));
		break;
	}
	case E_M_SPLASH:
	{
		RenderMeshOnScreen(P_meshArray[E_GEO_SPLASH], 1.f, 1.f, static_cast<float>(Application::GetWindowWidth() / 2), static_cast<float>(Application::GetWindowHeight() / 2));
		break;
	}
	case E_M_MAIN:
	{

		break;
	}
	case E_M_OPTIONS:
	{

		break;
	}
	case E_M_OPTIONS_CONTROLS:
	{

		break;
	}
	case E_M_OPTIONS_CONTROLS_SETCONTROL:
	{
		RenderMeshOnScreen(P_meshArray[E_GEO_BACKGROUND], 1, 1, Application::GetWindowWidth() / 2, Application::GetWindowHeight() / 2);
		//RenderTextOnScreen(P_meshArray[E_GEO_TEXT], "Press a key", Color(0.f, 0.f, 0.f), 30, Application::GetWindowWidth()/2 - 100, Application::GetWindowHeight()/2);
		break;
	}
	}
	RenderButtons();
	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Clears memory upon exit
*/
/******************************************************************************/
void MenuScene::Exit()
{
	saveGame();
	glDeleteVertexArrays(1, &u_m_vertexArrayID);
	glDeleteProgram(u_m_programID);

	delete[] P_lightsArray;

	while (v_buttonList.size() > 0)
	{
		S_BUTTON *S_MB = v_buttonList.back();
		delete S_MB;
		v_buttonList.pop_back();
	}

	for (unsigned i = 0; i < E_GEO_TOTAL; ++i)
	{
		if (P_meshArray[i] != NULL)
		{
			delete P_meshArray[i];
		}
	}
}
