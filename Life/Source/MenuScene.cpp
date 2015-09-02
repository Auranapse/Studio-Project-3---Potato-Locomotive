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
void MenuScene::assignsave(bool save)
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
Initialize default variables, create meshes, lighting
*/
/******************************************************************************/
void MenuScene::Init()
{
	SE_Engine.Init();
	f_fov = 0.f;
	f_mouseSensitivity = 0.f;
	Graphics = GRA_MAX;

	for (size_t i = 0; i < E_CTRL_TOTAL; ++i)
	{
		us_control[i] = 0;
	}

	SH_1.init("GameData//ImportantData.GoddamnitQuen");
	assignsave(false);

	InitShaders();
	//Starting position of translations and initialize physics

	f_timer = 0.f;
	MousePosX = 0.f;
	MousePosY = 0.f;
	v3_MenuCam.SetZero();
	InitMenu();

	f_fov_target = f_fov;

	MENU_STATE = E_M_SPLASH;

	SoundList[ST_BUTTON_CLICK] = SE_Engine.preloadSound("GameData//Sounds//UI//click.wav");
	SoundList[ST_BUTTON_CLICK_2] = SE_Engine.preloadSound("GameData//Sounds//UI//click2.wav");
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
	glClearColor(1.f, 0.65f, 0.439f, 0.0f);

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
	projection.SetToPerspective(f_fov, static_cast<double>(Application::GetWindowWidth()) / static_cast<double>(Application::GetWindowHeight()), 0.01f, 10000.0f); //FOV, Aspect Ratio, Near plane, Far plane
	projectionStack.LoadMatrix(projection);

	// Init Camera
	camera.Init(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -100.0f), Vector3(0.0f, 1.0f, 0.0f));

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
	for (unsigned i = 0; i < E_GEO_TOTAL; ++i)
	{
		P_meshArray[i] = NULL;
	}

	P_meshArray[E_GEO_AXES] = MeshBuilder::GenerateAxes("AXES", 10000, 10000, 10000);
	P_meshArray[E_GEO_MATRIX] = MeshBuilder::GenerateMatrix("Matrix", Color(0.835f, 0.56f, 0.396f), 10000, 1000, 10);
	//Text
	P_meshArray[E_GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	P_meshArray[E_GEO_TEXT]->textureID[0] = LoadTGA("GameData//Image//font//inputm.tga", false, false);

	P_meshArray[E_GEO_BACKGROUND] = MeshBuilder::GenerateQuad("Background", Color(1.0f, 1.0f, 1.0f), static_cast<float>(Application::GetWindowWidth() / 2), static_cast<float>(Application::GetWindowHeight() / 2), 1.0f);
	P_meshArray[E_GEO_BACKGROUND]->textureID[0] = LoadTGA("GameData//Image//UI//Background.tga", true, false);

	P_meshArray[E_GEO_SPLASH] = MeshBuilder::GenerateQuad("PentaFeedStudios", Color(1.0f, 1.0f, 1.0f), static_cast<float>(Application::GetWindowWidth() / 2), static_cast<float>(Application::GetWindowHeight() / 2), 1.0f);
	P_meshArray[E_GEO_SPLASH]->textureID[0] = LoadTGA("GameData//Image//UI//Logo.tga", true);

	P_meshArray[E_GEO_LOADING_BACKGROUND] = MeshBuilder::GenerateQuad("Loading Screen", Color(1.0f, 1.0f, 1.0f), static_cast<float>(Application::GetWindowWidth() / 2), static_cast<float>(Application::GetWindowHeight() / 2), 1.0f);
	P_meshArray[E_GEO_LOADING_BACKGROUND]->textureID[0] = LoadTGA("GameData//Image//UI//Loading.tga", true);

	P_meshArray[E_GEO_BUTTON_BACK] = MeshBuilder::GenerateQuad("Button Texture", Color(0.f, 0.f, 0.f), 1.f, 1.f, 1.0f);
	P_meshArray[E_GEO_BUTTON_BACK]->textureID[0] = LoadTGA("GameData//Image//UI//Arrow//back.tga", true);

	P_meshArray[E_GEO_BUTTON_LEFT] = MeshBuilder::GenerateQuad("Button Texture", Color(0.f, 0.f, 0.f), 1.f, 1.f, 1.0f);
	P_meshArray[E_GEO_BUTTON_LEFT]->textureID[0] = LoadTGA("GameData//Image//UI//Arrow//left.tga", true);

	P_meshArray[E_GEO_BUTTON_RIGHT] = MeshBuilder::GenerateQuad("Button Texture", Color(0.f, 0.f, 0.f), 1.f, 1.f, 1.0f);
	P_meshArray[E_GEO_BUTTON_RIGHT]->textureID[0] = LoadTGA("GameData//Image//UI//Arrow//right.tga", true);
}

/******************************************************************************/
/*!
\brief
Initializes menu
*/
/******************************************************************************/
void MenuScene::InitMenu(void)
{
	UIColor.Set(0.21f, 0.21f, 0.21f);
	UIColorPressed.Set(0.3f, 0.3f, 0.3);

	v3_Menupos[E_M_SPLASH].Set(0, -2000, 0);
	v3_Menupos[E_M_MAIN].Set(0, 0, 0);
	v3_Menupos[E_M_LOADING] = v3_Menupos[E_M_MAIN];
	v3_Menupos[E_M_OPTIONS].Set(0, 2000, 0);
	v3_Menupos[E_M_OPTIONS_CONTROLS].Set(4000, 2000, 0);
	v3_Menupos[E_M_OPTIONS_CONTROLS_SETCONTROL].Set(4000, 4000, 0);

	transcomplete = false;

	//MAIN--------------------------------------------------------
	TextButton* S_MB;
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.22f, Application::GetWindowHeight()*0.5f, 0.1f);
	S_MB->scale.Set(35, 35, 35);
	S_MB->text = "Play";
	S_MB->gamestate = E_M_MAIN;
	v_textButtonList.push_back(S_MB);

	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.22f - 4.f, Application::GetWindowHeight()*0.5f - 60.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = "Options";
	S_MB->gamestate = E_M_MAIN;
	v_textButtonList.push_back(S_MB);

	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.22f - 4.f, Application::GetWindowHeight()*0.5f - 90.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = "Quit";
	S_MB->gamestate = E_M_MAIN;
	v_textButtonList.push_back(S_MB);

	//OPTIONS-----------------------------------------------------
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.22f, Application::GetWindowHeight()*0.6f - 140.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = "Controls";
	S_MB->gamestate = E_M_OPTIONS;
	v_textButtonList.push_back(S_MB);

	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.22f, Application::GetWindowHeight()*0.6f - 180.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = "Toggle Fullscreen";
	S_MB->gamestate = E_M_OPTIONS;
	v_textButtonList.push_back(S_MB);

	Button *m_B;
	m_B = new Button;
	m_B->Position.Set(Application::GetWindowWidth()*0.4f, Application::GetWindowHeight()*0.6f, 0.1f);
	m_B->Scale.Set(20, 20, 20);
	m_B->mesh = P_meshArray[E_GEO_BUTTON_LEFT];
	m_B->ID = BI_FOV_DECREASE;
	m_B->labeltype = Button::LT_NONE;
	m_B->gamestate = E_M_OPTIONS;
	v_buttonList.push_back(m_B);

	m_B = new Button;
	m_B->Position.Set(Application::GetWindowWidth()*0.4f + 60.f, Application::GetWindowHeight()*0.6f, 0.1f);
	m_B->Scale.Set(20, 20, 20);
	m_B->mesh = P_meshArray[E_GEO_BUTTON_RIGHT];
	m_B->ID = BI_FOV_INCREASE;
	m_B->labeltype = Button::LT_NONE;
	m_B->gamestate = E_M_OPTIONS;
	v_buttonList.push_back(m_B);

	m_B = new Button;
	m_B->Position.Set(Application::GetWindowWidth()*0.4f, Application::GetWindowHeight()*0.6f - 40.f, 0.1f);
	m_B->Scale.Set(20, 20, 20);
	m_B->mesh = P_meshArray[E_GEO_BUTTON_LEFT];
	m_B->ID = BI_SENSITIVITY_DECREASE;
	m_B->labeltype = Button::LT_NONE;
	m_B->gamestate = E_M_OPTIONS;
	v_buttonList.push_back(m_B);

	m_B = new Button;
	m_B->Position.Set(Application::GetWindowWidth()*0.4f + 60.f, Application::GetWindowHeight()*0.6f - 40.f, 0.1f);
	m_B->Scale.Set(20, 20, 20);
	m_B->mesh = P_meshArray[E_GEO_BUTTON_RIGHT];
	m_B->ID = BI_SENSITIVITY_INCREASE;
	m_B->labeltype = Button::LT_NONE;
	m_B->gamestate = E_M_OPTIONS;
	v_buttonList.push_back(m_B);

	m_B = new Button;
	m_B->Position.Set(Application::GetWindowWidth()*0.4f, Application::GetWindowHeight()*0.6f - 80.f, 0.1f);
	m_B->Scale.Set(20, 20, 20);
	m_B->mesh = P_meshArray[E_GEO_BUTTON_LEFT];
	m_B->ID = BI_GRAPHICS_DECREASE;
	m_B->labeltype = Button::LT_NONE;
	m_B->gamestate = E_M_OPTIONS;
	v_buttonList.push_back(m_B);

	m_B = new Button;
	m_B->Position.Set(Application::GetWindowWidth()*0.4f + 60.f, Application::GetWindowHeight()*0.6f - 80.f, 0.1f);
	m_B->Scale.Set(20, 20, 20);
	m_B->mesh = P_meshArray[E_GEO_BUTTON_RIGHT];
	m_B->ID = BI_GRAPHICS_INCREASE;
	m_B->labeltype = Button::LT_NONE;
	m_B->gamestate = E_M_OPTIONS;
	v_buttonList.push_back(m_B);


	//BACK BUTTONS------------------------------------------------
	m_B = new Button;
	m_B->Position.Set(Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f, 0.1f);
	m_B->Scale.Set(25, 25, 25);
	m_B->mesh = P_meshArray[E_GEO_BUTTON_LEFT];
	m_B->ID = BI_BACK;
	m_B->labeltype = Button::LT_NONE;
	m_B->gamestate = E_M_OPTIONS;
	v_buttonList.push_back(m_B);

	m_B = new Button;
	m_B->Position.Set(Application::GetWindowWidth()*0.05f, Application::GetWindowHeight()*0.05f, 0.1f);
	m_B->Scale.Set(25, 25, 25);
	m_B->mesh = P_meshArray[E_GEO_BUTTON_LEFT];
	m_B->ID = BI_BACK;
	m_B->labeltype = Button::LT_NONE;
	m_B->gamestate = E_M_OPTIONS_CONTROLS;
	v_buttonList.push_back(m_B);

	//Control changer---------------------------------------------
	us_controlCB[E_CTRL_MOVE_FRONT].text = "Forward";
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.1f, Application::GetWindowHeight()*0.8f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = us_controlCB[E_CTRL_MOVE_FRONT].text;
	S_MB->gamestate = E_M_OPTIONS_CONTROLS;
	us_controlCB[E_CTRL_MOVE_FRONT].button = S_MB;
	v_textButtonList.push_back(S_MB);

	us_controlCB[E_CTRL_MOVE_BACK].text = "Backward";
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.1f, Application::GetWindowHeight()*0.8f - 40.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = us_controlCB[E_CTRL_MOVE_BACK].text;
	S_MB->gamestate = E_M_OPTIONS_CONTROLS;
	us_controlCB[E_CTRL_MOVE_BACK].button = S_MB;
	v_textButtonList.push_back(S_MB);

	us_controlCB[E_CTRL_MOVE_LEFT].text = "Left";
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.1f, Application::GetWindowHeight()*0.8f - 80.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = us_controlCB[E_CTRL_MOVE_LEFT].text;
	S_MB->gamestate = E_M_OPTIONS_CONTROLS;
	us_controlCB[E_CTRL_MOVE_LEFT].button = S_MB;
	v_textButtonList.push_back(S_MB);

	us_controlCB[E_CTRL_MOVE_RIGHT].text = "Right";
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.1f, Application::GetWindowHeight()*0.8f - 120.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = us_controlCB[E_CTRL_MOVE_RIGHT].text;
	S_MB->gamestate = E_M_OPTIONS_CONTROLS;
	us_controlCB[E_CTRL_MOVE_RIGHT].button = S_MB;
	v_textButtonList.push_back(S_MB);

	us_controlCB[E_CTRL_MOVE_SPRINT].text = "Sprint";
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.1f, Application::GetWindowHeight()*0.8f - 160.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = us_controlCB[E_CTRL_MOVE_SPRINT].text;
	S_MB->gamestate = E_M_OPTIONS_CONTROLS;
	us_controlCB[E_CTRL_MOVE_SPRINT].button = S_MB;
	v_textButtonList.push_back(S_MB);

	us_controlCB[E_CTRL_MOVE_WALK].text = "Walk";
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.1f, Application::GetWindowHeight()*0.8f - 200.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = us_controlCB[E_CTRL_MOVE_WALK].text;
	S_MB->gamestate = E_M_OPTIONS_CONTROLS;
	us_controlCB[E_CTRL_MOVE_WALK].button = S_MB;
	v_textButtonList.push_back(S_MB);

	us_controlCB[E_CTRL_MOVE_JUMP].text = "Jump";
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.1f, Application::GetWindowHeight()*0.8f - 240.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = us_controlCB[E_CTRL_MOVE_JUMP].text;
	S_MB->gamestate = E_M_OPTIONS_CONTROLS;
	us_controlCB[E_CTRL_MOVE_JUMP].button = S_MB;
	v_textButtonList.push_back(S_MB);

	us_controlCB[E_CTRL_INTERACT].text = "Interact";
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.1f, Application::GetWindowHeight()*0.8f - 320.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = us_controlCB[E_CTRL_INTERACT].text;
	S_MB->gamestate = E_M_OPTIONS_CONTROLS;
	us_controlCB[E_CTRL_INTERACT].button = S_MB;
	v_textButtonList.push_back(S_MB);

	us_controlCB[E_CTRL_ATTACK].text = "Attack";
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.1f, Application::GetWindowHeight()*0.8f - 360.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = us_controlCB[E_CTRL_ATTACK].text;
	S_MB->gamestate = E_M_OPTIONS_CONTROLS;
	us_controlCB[E_CTRL_ATTACK].button = S_MB;
	v_textButtonList.push_back(S_MB);

	us_controlCB[E_CTRL_THROW].text = "Throw";
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.1f, Application::GetWindowHeight()*0.8f - 400.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = us_controlCB[E_CTRL_THROW].text;
	S_MB->gamestate = E_M_OPTIONS_CONTROLS;
	us_controlCB[E_CTRL_THROW].button = S_MB;
	v_textButtonList.push_back(S_MB);

	us_controlCB[E_CTRL_AIM].text = "Aim";
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.1f, Application::GetWindowHeight()*0.8f - 440.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = us_controlCB[E_CTRL_AIM].text;
	S_MB->gamestate = E_M_OPTIONS_CONTROLS;
	us_controlCB[E_CTRL_AIM].button = S_MB;
	v_textButtonList.push_back(S_MB);

	us_controlCB[E_CTRL_ABILITY_1].text = "Ability 1";
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.6f, Application::GetWindowHeight()*0.8f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = us_controlCB[E_CTRL_ABILITY_1].text;
	S_MB->gamestate = E_M_OPTIONS_CONTROLS;
	us_controlCB[E_CTRL_ABILITY_1].button = S_MB;
	v_textButtonList.push_back(S_MB);

	us_controlCB[E_CTRL_ABILITY_2].text = "Ability 2";
	S_MB = new TextButton;
	S_MB->pos.Set(Application::GetWindowWidth()*0.6f, Application::GetWindowHeight()*0.8f - 40.f, 0.1f);
	S_MB->scale.Set(25, 25, 25);
	S_MB->text = us_controlCB[E_CTRL_ABILITY_2].text;
	S_MB->gamestate = E_M_OPTIONS_CONTROLS;
	us_controlCB[E_CTRL_ABILITY_2].button = S_MB;
	v_textButtonList.push_back(S_MB);

	for (unsigned i = 0; i < E_CTRL_TOTAL; ++i)
	{
		us_controlCB[i].Control = us_control[i];
		UpdateControlSettingLabels(us_control[i], i);
	}
}

/******************************************************************************/
/*!
\brief
Gets the button
*/
/******************************************************************************/
TextButton* MenuScene::FetchTB(std::string name)
{
	for (std::vector<TextButton*>::iterator it = v_textButtonList.begin(); it != v_textButtonList.end(); ++it)
	{
		TextButton *S_MB = (TextButton *)*it;
		if (S_MB != NULL)
		{
			if (S_MB->text == name && S_MB->gamestate == MENU_STATE)
			{
				return S_MB;
			}
		}
	}

	return NULL;
}

/******************************************************************************/
/*!
\brief
Gets the button
*/
/******************************************************************************/
Button* MenuScene::FetchBUTTON(int ID)
{
	for (std::vector<Button*>::iterator it = v_buttonList.begin(); it != v_buttonList.end(); ++it)
	{
		Button *m_B = (Button *)*it;
		if (m_B != NULL)
		{
			if (m_B->gamestate == MENU_STATE && m_B->ID == ID)
			{
				return m_B;
			}
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
void MenuScene::UpdateTextButtons(void)
{
	for (std::vector<TextButton*>::iterator it = v_textButtonList.begin(); it != v_textButtonList.end(); ++it)
	{
		TextButton *S_MB = (TextButton *)*it;
		if (S_MB->gamestate == MENU_STATE)
		{
			Vector3 offset = v3_Menupos[MENU_STATE];

			if (intersect2D((S_MB->pos + Vector3(S_MB->text.length() * (S_MB->scale.x) - S_MB->scale.x, S_MB->scale.y*0.4f, 0)) + offset, S_MB->pos + Vector3(-S_MB->scale.x*0.5f, -(S_MB->scale.y*0.4f), 0) + offset, Vector3(MousePosX, MousePosY, 0)))
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
Update button state
*/
/******************************************************************************/
void MenuScene::UpdateButtons(void)
{
	for (std::vector<Button*>::iterator it = v_buttonList.begin(); it != v_buttonList.end(); ++it)
	{
		Button *m_B = (Button *)*it;

		Vector3 offset = v3_Menupos[MENU_STATE];

		if (intersect2D(m_B->Position + offset + m_B->Scale, m_B->Position + offset - m_B->Scale, Vector3(MousePosX, MousePosY, 0)))
		{
			m_B->active = true;
			m_B->color = UIColorPressed;
		}
		else
		{
			m_B->active = false;
			m_B->color = UIColor;
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
	f_timer += static_cast<float>(dt);
	//Mouse Section
	double x, y;
	Application::GetMousePos(x, y);
	MousePosX = static_cast<float>(x) / Application::GetWindowWidth() * Application::GetWindowWidth() + v3_MenuCam.x;
	MousePosY = (Application::GetWindowHeight() - static_cast<float>(y)) / Application::GetWindowHeight() * Application::GetWindowHeight() + v3_MenuCam.y;//*/

	static bool bLButtonState = false;

	UpdateTextButtons();
	UpdateButtons();

	if (v3_MenuCam != v3_Menupos[MENU_STATE])
	{
		transcomplete = false;

		Vector3 diff = v3_Menupos[MENU_STATE] - v3_MenuCam;
		v3_MenuCam += diff * static_cast<float>(dt) * 10.f;

		if (diff.x < 0.15f && diff.y < 0.15f && diff.x > -0.15f && diff.y > -0.15f)
		{
			v3_MenuCam = v3_Menupos[MENU_STATE];
		}
	}
	else
	{
		transcomplete = true;
	}

	if (f_fov != f_fov_target)
	{
		float diff = f_fov_target - f_fov;

		if (diff < 0.01 && diff > -0.01)
		{
			f_fov = f_fov_target;
		}
		else
		{
			f_fov += diff * static_cast<float>(dt) * 10.f;
		}

		editFOV(f_fov);
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

			if (FetchTB("Play")->active)
			{
				PREV_STATE = MENU_STATE;
				MENU_STATE = E_M_LOADING;
			}
			else if (FetchTB("Options")->active)
			{
				SE_Engine.playSound2D(SoundList[ST_BUTTON_CLICK]);
				PREV_STATE = MENU_STATE;
				MENU_STATE = E_M_OPTIONS;
			}
			else if (FetchTB("Quit")->active)
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

			if (FetchTB("Controls")->active)
			{
				SE_Engine.playSound2D(SoundList[ST_BUTTON_CLICK]);
				PREV_STATE = MENU_STATE;
				MENU_STATE = E_M_OPTIONS_CONTROLS;
			}
			else if (FetchTB("Toggle Fullscreen")->active)
			{
				SE_Engine.playSound2D(SoundList[ST_BUTTON_CLICK]);
				Application::fullscreentoggle();
			}
			else if (FetchBUTTON(BI_BACK)->active)
			{
				SE_Engine.playSound2D(SoundList[ST_BUTTON_CLICK]);
				PREV_STATE = MENU_STATE;
				MENU_STATE = E_M_MAIN;
			}
			else if (FetchBUTTON(BI_FOV_INCREASE)->active)
			{
				if (f_fov_target < 120)
				{
					f_fov_target += 5;
				}
			}
			else if (FetchBUTTON(BI_FOV_DECREASE)->active)
			{
				if (f_fov_target > 45)
				{
					f_fov_target -= 5;
				}
			}
			else if (FetchBUTTON(BI_SENSITIVITY_INCREASE)->active)
			{
				if (f_mouseSensitivity < 50)
				{
					f_mouseSensitivity += 0.1f;
				}
			}
			else if (FetchBUTTON(BI_SENSITIVITY_DECREASE)->active)
			{
				if (f_mouseSensitivity > 0.1)
				{
					f_mouseSensitivity -= 0.1f;
				}
			}
			else if (FetchBUTTON(BI_GRAPHICS_INCREASE)->active)
			{
				if (Graphics > GRA_MAX)
				{
					--Graphics;
				}
			}
			else if (FetchBUTTON(BI_GRAPHICS_DECREASE)->active)
			{
				if (Graphics < GRA_SHIT)
				{
					++Graphics;
				}
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

			if (FetchBUTTON(BI_BACK)->active)
			{
				SE_Engine.playSound2D(SoundList[ST_BUTTON_CLICK]);
				PREV_STATE = MENU_STATE;
				MENU_STATE = E_M_OPTIONS;
			}

			for (unsigned i = 0; i < E_CTRL_TOTAL; ++i)
			{
				if (FetchTB(us_controlCB[i].text) != NULL)
				{
					if (FetchTB(us_controlCB[i].text)->active)
					{
						SE_Engine.playSound2D(SoundList[ST_BUTTON_CLICK]);
						us_ControlChange = &us_control[i];
						i_ControlChange = i;
						PREV_STATE = MENU_STATE;
						MENU_STATE = E_M_OPTIONS_CONTROLS_SETCONTROL;
					}
				}
			}
		}
		break;
	}
	case E_M_OPTIONS_CONTROLS_SETCONTROL:
	{
		for (size_t i = 1; i < VK_OEM_CLEAR; ++i)
		{
			if ((GetAsyncKeyState(i) & 0x8000))
			{
				if (i == 12)
				{
					continue;
				}

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
				SE_Engine.playSound2D(SoundList[ST_BUTTON_CLICK_2]);
				break;
			}
		}
		break;
	}
	case E_M_SPLASH:
	{
		if (f_timer > 0.5f)
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
		us_controlCB[button].CONTROLTEXT = "SHIFT";
	}
	else if (key == 17)
	{
		us_controlCB[button].CONTROLTEXT = "CTRL";
	}
	else if (key == 164)
	{
		us_controlCB[button].CONTROLTEXT = "LALT";
	}
	else if (key == 32)
	{
		us_controlCB[button].CONTROLTEXT = "SPACE";
	}
	else if (key == 1)
	{
		us_controlCB[button].CONTROLTEXT = "LMB";
	}
	else if (key == 2)
	{
		us_controlCB[button].CONTROLTEXT = "RMB";
	}
	else if (key == 4)
	{
		us_controlCB[button].CONTROLTEXT = "MMB";
	}
	else if (key == 5)
	{
		us_controlCB[button].CONTROLTEXT = "BUTTON 4";
	}
	else if (key == 6)
	{
		us_controlCB[button].CONTROLTEXT = "BUTTON 5";
	}
	else if (key == 9)
	{
		us_controlCB[button].CONTROLTEXT = "TAB";
	}
	else if (key == 96)
	{
		us_controlCB[button].CONTROLTEXT = "NUM 0";
	}
	else if (key == 97)
	{
		us_controlCB[button].CONTROLTEXT = "NUM 1";
	}
	else if (key == 98)
	{
		us_controlCB[button].CONTROLTEXT = "NUM 2";
	}
	else if (key == 99)
	{
		us_controlCB[button].CONTROLTEXT = "NUM 3";
	}
	else if (key == 100)
	{
		us_controlCB[button].CONTROLTEXT = "NUM 4";
	}
	else if (key == 101)
	{
		us_controlCB[button].CONTROLTEXT = "NUM 5";
	}
	else if (key == 102)
	{
		us_controlCB[button].CONTROLTEXT = "NUM 6";
	}
	else if (key == 103)
	{
		us_controlCB[button].CONTROLTEXT = "NUM 7";
	}
	else if (key == 104)
	{
		us_controlCB[button].CONTROLTEXT = "NUM 8";
	}
	else if (key == 105)
	{
		us_controlCB[button].CONTROLTEXT = "NUM 9";
	}
	else
	{
		us_controlCB[button].CONTROLTEXT = s;
	}
}

/******************************************************************************/
/*!
\brief
edits FOV
\param newFOV
new fov to change to
*/
/******************************************************************************/
void MenuScene::editFOV(float &newFOV)
{
	Mtx44 proj;
	proj.SetToPerspective(newFOV, static_cast<double>(Application::GetWindowWidth()) / static_cast<double>(Application::GetWindowHeight()), 0.1f, 10000.0f);
	projectionStack.LoadMatrix(proj);
}

/******************************************************************************/
/*!
\brief
Update FOV
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
	proj.SetToPerspective(f_fov, Application::GetWindowWidth() / Application::GetWindowHeight(), 0.1f, 10000.0f);
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

	u_m_parameters[U_UNI_GLOW] = glGetUniformLocation(u_m_programID, "glow");
	u_m_parameters[U_UNI_GLOW_COLOR] = glGetUniformLocation(u_m_programID, "glowColor");

	glUniform1i(u_m_parameters[U_UNI_GLOW], 0);

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
void MenuScene::RenderMesh(Mesh *mesh, bool enableLight, float Glow, Color GlowColor)
{
	glUniform1i(u_m_parameters[U_UNI_GLOW], static_cast<GLint>(Glow));
	glUniform3fv(u_m_parameters[U_UNI_GLOW_COLOR], 1, &GlowColor.r);

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
	modelStack.PushMatrix();
	x -= ((text.length() - 1.5f) / 2.0f) * size;
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	RenderTextOnScreen(mesh, text, color);
	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
\brief
Renders text on screen
*/
/******************************************************************************/
void MenuScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color)
{
	glUniform1i(u_m_parameters[U_UNI_GLOW], static_cast<GLint>(0));
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
void MenuScene::RenderMeshOnScreen(Mesh* mesh, float Glow, Color GlowColor)
{
	glUniform1i(u_m_parameters[U_UNI_GLOW], static_cast<GLint>(Glow));
	glUniform3fv(u_m_parameters[U_UNI_GLOW_COLOR], 1, &GlowColor.r);


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

	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(u_m_parameters[E_UNI_MVP], 1, GL_FALSE, &MVP.a[0]);

	mesh->Render();

	projectionStack.PopMatrix();
	viewStack.PopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(u_m_parameters[E_UNI_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
	glUniform1i(u_m_parameters[U_UNI_GLOW], 0);
}

/******************************************************************************/
/*!
\brief
Renders the menu buttons
*/
/******************************************************************************/
void MenuScene::RenderTextButtons(void)
{
	for (unsigned i = 0; i < v_textButtonList.size(); ++i)
	{
		TextButton *S_MB = v_textButtonList[i];
		if ((S_MB->gamestate == MENU_STATE || S_MB->gamestate == PREV_STATE) && S_MB->gamestate != E_M_LOADING)
		{
			modelStack.PushMatrix();
			modelStack.Translate(v3_Menupos[S_MB->gamestate]);
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
Renders the menu buttons
*/
/******************************************************************************/
void MenuScene::RenderButtons(void)
{
	for (unsigned i = 0; i < v_buttonList.size(); ++i)
	{
		Button *m_B = v_buttonList[i];
		if ((m_B->gamestate == MENU_STATE || m_B->gamestate == PREV_STATE) && m_B->gamestate != E_M_LOADING)
		{
			modelStack.PushMatrix();
			modelStack.Translate(v3_Menupos[m_B->gamestate]);
			modelStack.Translate(m_B->Position);
			modelStack.Scale(m_B->Scale);
			RenderMeshOnScreen(m_B->mesh, 10.f, m_B->color);

			if (m_B->labeltype == Button::LT_BUTTON)
			{
				RenderTextCenterOnScreen(P_meshArray[E_GEO_TEXT], m_B->label, m_B->color);
			}

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
		v3_MenuCam.x, v3_MenuCam.y, v3_MenuCam.z,
		v3_MenuCam.x, v3_MenuCam.y, -100.f,
		camera.up.x, camera.up.y, camera.up.z
		);

	modelStack.LoadIdentity();

	modelStack.PushMatrix();
	modelStack.Translate(-2500, -2500, -10000);
	modelStack.Rotate(f_timer, 1, 1, 1);
	RenderMesh(P_meshArray[E_GEO_MATRIX], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-v3_MenuCam);

	RenderTextButtons();
	RenderButtons();

	switch (MENU_STATE)
	{
	case E_M_LOADING:
	{
		modelStack.PushMatrix();
		modelStack.LoadIdentity();
		modelStack.Translate(static_cast<float>(Application::GetWindowWidth() * 0.5f), static_cast<float>(Application::GetWindowHeight() * 0.5f), 0);
		RenderMeshOnScreen(P_meshArray[E_GEO_LOADING_BACKGROUND]);
		modelStack.PopMatrix();
		break;
	}
	case E_M_SPLASH:
	{
		modelStack.PushMatrix();
		modelStack.LoadIdentity();
		modelStack.Translate(static_cast<float>(Application::GetWindowWidth() * 0.5f), static_cast<float>(Application::GetWindowHeight() * 0.5f), 0);
		RenderMeshOnScreen(P_meshArray[E_GEO_SPLASH]);
		modelStack.PopMatrix();
		break;
	}
	case E_M_MAIN:
	{

		break;
	}
	case E_M_OPTIONS:
	{
		modelStack.PushMatrix();
		modelStack.Translate(v3_Menupos[MENU_STATE]);
		//FOV
		modelStack.PushMatrix();
		modelStack.Translate(FetchBUTTON(BI_FOV_DECREASE)->Position);
		modelStack.Translate(-100, 0, 0);
		modelStack.Scale(25, 25, 25);
		RenderTextOnScreen(P_meshArray[E_GEO_TEXT], "FOV", UIColor);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(FetchBUTTON(BI_FOV_INCREASE)->Position);
		modelStack.Translate(50, 0, 0);
		modelStack.Scale(25, 25, 25);

		std::stringstream ss;
		ss << f_fov_target;
		RenderTextOnScreen(P_meshArray[E_GEO_TEXT], ss.str(), UIColor);
		modelStack.PopMatrix();
		//Mouse
		modelStack.PushMatrix();
		modelStack.Translate(FetchBUTTON(BI_SENSITIVITY_DECREASE)->Position);
		modelStack.Translate(-150, 0, 0);
		modelStack.Scale(25, 25, 25);
		RenderTextOnScreen(P_meshArray[E_GEO_TEXT], "Mouse", UIColor);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(FetchBUTTON(BI_SENSITIVITY_INCREASE)->Position);
		modelStack.Translate(50, 0, 0);
		modelStack.Scale(25, 25, 25);

		ss.str("");
		ss << (f_mouseSensitivity*100.f);
		RenderTextOnScreen(P_meshArray[E_GEO_TEXT], ss.str(), UIColor);
		modelStack.PopMatrix();
		//Graphics
		modelStack.PushMatrix();
		modelStack.Translate(FetchBUTTON(BI_GRAPHICS_DECREASE)->Position);
		modelStack.Translate(-225, 0, 0);
		modelStack.Scale(25, 25, 25);
		RenderTextOnScreen(P_meshArray[E_GEO_TEXT], "Graphics", UIColor);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(FetchBUTTON(BI_GRAPHICS_INCREASE)->Position);
		modelStack.Translate(50, 0, 0);
		modelStack.Scale(25, 25, 25);

		ss.str("");
		if (Graphics == GRA_MAX)
		{
			ss << "Max";
		}
		else if (Graphics == GRA_MEDIUM)
		{
			ss << "Medium";
		}
		else if (Graphics == GRA_LOW)
		{
			ss << "Low";
		}
		else
		{
			ss << "Shit";
		}

		RenderTextOnScreen(P_meshArray[E_GEO_TEXT], ss.str(), UIColor);
		modelStack.PopMatrix();

		modelStack.PopMatrix();
		break;
	}
	case E_M_OPTIONS_CONTROLS:
	{
		for (unsigned i = 0; i < E_CTRL_TOTAL; ++i)
		{
			if (us_controlCB[i].button != NULL)
			{
				modelStack.PushMatrix();
				modelStack.Translate(v3_Menupos[MENU_STATE]);
				modelStack.Translate(us_controlCB[i].button->pos);
				modelStack.Translate(300, 0, 0);
				modelStack.Scale(us_controlCB[i].button->scale);
				RenderTextOnScreen(P_meshArray[E_GEO_TEXT], us_controlCB[i].CONTROLTEXT, UIColorPressed);
				modelStack.PopMatrix();
			}
		}

		break;
	}
	case E_M_OPTIONS_CONTROLS_SETCONTROL:
	{
		modelStack.PushMatrix();
		modelStack.Translate(v3_Menupos[MENU_STATE]);
		RenderTextCenterOnScreen(P_meshArray[E_GEO_TEXT], "Enter a key", UIColor, 40.f, Application::GetWindowWidth() * 0.5f, Application::GetWindowHeight() * 0.5f);
		modelStack.PopMatrix();
		break;
	}
	}

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
	assignsave(true);

	SE_Engine.Exit();

	delete[] P_lightsArray;

	while (v_textButtonList.size() > 0)
	{
		TextButton *S_MB = v_textButtonList.back();
		if (S_MB != NULL)
		{
			delete S_MB;
		}
		S_MB = NULL;
		v_textButtonList.pop_back();
	}

	while (v_buttonList.size() > 0)
	{
		Button *m_B = v_buttonList.back();
		if (m_B != NULL)
		{
			delete m_B;
		}
		m_B = NULL;
		v_buttonList.pop_back();
	}

	for (unsigned i = 0; i < E_GEO_TOTAL; ++i)
	{
		if (P_meshArray[i] != NULL)
		{
			delete P_meshArray[i];
		}
		P_meshArray[i] = NULL;
	}

	glDeleteVertexArrays(1, &u_m_vertexArrayID);
	glDeleteProgram(u_m_programID);
}