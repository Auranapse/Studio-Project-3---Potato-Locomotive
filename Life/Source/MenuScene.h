/****************************************************************************/
/*!
\file MenuScene.cpp
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Main menu for the openGL framework
!*/
/****************************************************************************/
#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include "Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "LoadTGA.h"
#include "AzimuthAltitudeCamera.h"
#include "savefileHandler.h"
#include "Button.h"
#include "GabrielDLC.h"
#include <string>
#include <iostream>
#include <fstream>

/******************************************************************************/
/*!
Class Assignment3:
\brief	Define geometries, light sources
*/
/******************************************************************************/
class MenuScene : public Scene
{

	/******************************************************************************/
	/*!
	enum E_CTRL_TYPE:
	\brief	Lists the types of controls
	*/
	/******************************************************************************/

	enum E_CTRL_TYPE
	{
		// Settings Controls
		E_CTRL_TOGGLE_CULL,
		E_CTRL_TOGGLE_WIREFRAME,
		E_CTRL_TOGGLE_AXES,
		E_CTRL_TOGGLE_INFO,

		// Camera Controls
		E_CTRL_CHAR_MODE,
		E_CTRL_NOCLIP_MODE,

		// Player Controls
		// -- Move
		E_CTRL_MOVE_FRONT,
		E_CTRL_MOVE_BACK,
		E_CTRL_MOVE_LEFT,
		E_CTRL_MOVE_RIGHT,
		E_CTRL_MOVE_SPRINT,
		E_CTRL_MOVE_WALK,
		E_CTRL_MOVE_JUMP,

		// -- Interactions
		E_CTRL_INTERACT,
		E_CTRL_KICK,
		E_CTRL_ATTACK,
		E_CTRL_AIM,
		E_CTRL_NEXT_ITEM,
		E_CTRL_RELOAD,

		// -- Look
		E_CTRL_LOOK_UP,
		E_CTRL_LOOK_DOWN,
		E_CTRL_LOOK_LEFT,
		E_CTRL_LOOK_RIGHT,

		// Debug
		E_CTRL_SHOW_DEBUG,
		E_CTRL_GRAV_RESET,
		E_CTRL_GRAV_RAISE,
		E_CTRL_GRAV_DROP,

		E_CTRL_TOTAL
	};

	/******************************************************************************/
	/*!
	enum E_GEO_TYPE:
	\brief	Lists the types of Meshes
	*/
	/******************************************************************************/
	enum E_GEO_TYPE
	{
		E_GEO_AXES,
		// Text
		E_GEO_TEXT,
		// UI---------------------
		E_GEO_UI_GRID,
		E_GEO_UI_BUTTON,
		E_GEO_SPLASH,
		E_GEO_BACKGROUND,
		E_GEO_LOADING_BACKGROUND,
		// -----------------------
		E_GEO_TOTAL,
	};
	/******************************************************************************/
	/*!
	enum E_TEX_TYPE:
	\brief	Lists the types of reusable texture resources
	*/
	/******************************************************************************/
	enum E_TEX_TYPE
	{
		E_TEX_BUTTON,
		E_TEX_TOTAL
	};
	/******************************************************************************/
	/*!
	enum E_UNI_TYPE:
	\brief	List the types of uniform parameters
	*/
	/******************************************************************************/
	enum E_UNI_TYPE
	{
		E_UNI_MVP = 0,
		E_UNI_MODELVIEW,
		E_UNI_MODELVIEW_INVERSE_TRANSPOSE,
		E_UNI_MATERIAL_AMBIENT,
		E_UNI_MATERIAL_DIFFUSE,
		E_UNI_MATERIAL_SPECULAR,
		E_UNI_MATERIAL_SHININESS,
		E_UNI_LIGHTENABLED,
		// Light[0]---------------------------
		E_UNI_LIGHT0_POSITION,
		E_UNI_LIGHT0_COLOR,
		E_UNI_LIGHT0_POWER,
		E_UNI_LIGHT0_KC,
		E_UNI_LIGHT0_KL,
		E_UNI_LIGHT0_KQ,
		E_UNI_LIGHT0_TYPE,
		E_UNI_LIGHT0_SPOTDIRECTION,
		E_UNI_LIGHT0_COSCUTOFF,
		E_UNI_LIGHT0_COSINNER,
		E_UNI_LIGHT0_EXPONENT,

		E_UNI_NUMLIGHTS,
		// Texture----------------------------
		E_UNI_COLOR_TEXTURE_ENABLED,
		E_UNI_COLOR_TEXTURE,
		// Text-------------------------------
		E_UNI_TEXT_ENABLED,
		E_UNI_TEXT_COLOR,
		//------------------------------------
		E_UNI_TOTAL,
	};

	/******************************************************************************/
	/*!
	enum E_MENU_STATE:
	\brief	Lists the various states of the Menu
	*/
	/******************************************************************************/
	enum E_MENU_STATE
	{
		E_M_LOADING,
		E_M_SPLASH,
		E_M_MAIN,
		E_M_OPTIONS,
		E_M_OPTIONS_CONTROLS,
		E_M_OPTIONS_CONTROLS_SETCONTROL,
		E_M_TOTAL,
	};

public:
	MenuScene();
	~MenuScene();

	virtual void Init();
	virtual void InitShaders();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	// Static Constants
	/******************************************************************************/
	/*!
	static const unsigned int ui_NUM_LIGHT_PARAMS:
	\brief	Stores the number of uniform light parameters
	*/
	/******************************************************************************/
	static const unsigned int ui_NUM_LIGHT_PARAMS;
	/******************************************************************************/
	/*!
	unsigned u_m_vertexArrayID:
	\brief	Vertex array ID
	*/
	/******************************************************************************/
	unsigned u_m_vertexArrayID;

	/******************************************************************************/
	/*!
			Mesh* P_meshArray[E_GEO_TOTAL]:
	\brief	Stores pointers to meshes
	*/
	/******************************************************************************/
	Mesh* P_meshArray[E_GEO_TOTAL];

	// GLFW/GLEW
	/******************************************************************************/
	/*!
			unsigned u_m_programID:
	\brief	Program ID
	*/
	/******************************************************************************/
	unsigned u_m_programID;

	/******************************************************************************/
	/*!
			unsigned u_m_parameters[E_UNI_TOTAL]:
	\brief	An array to store all the uniform parameters specified in the UNIFORM_TYPE enumeration
	*/
	/******************************************************************************/
	unsigned u_m_parameters[E_UNI_TOTAL];

	// Cameras
	/******************************************************************************/
	/*!
			FirstPersonCamera noClipCamera:
	\brief
	*/
	/******************************************************************************/
	Camera camera;

	// Reusable Resources
	/******************************************************************************/
	/*!
			unsigned u_texResourceArray[E_TEX_TOTAL]:
	\brief	Stores reusable textures
	*/
	/******************************************************************************/
	unsigned u_texResourceArray[E_TEX_TOTAL];

	// Controls
	/******************************************************************************/
	/*!
			float f_mouseSensitivity:
	\brief	Stores and controls mouse sensitivity
	*/
	/******************************************************************************/
	float f_mouseSensitivity;

	/******************************************************************************/
	/*!
			float f_fov:
	\brief	Controls the field of view of the game
	*/
	/******************************************************************************/
	float f_fov;

	//Lighting
	/******************************************************************************/
	/*!
			const unsigned ui_NUM_LIGHTS:
	\brief	Controls the number of lights rendered in the scene
	*/
	/******************************************************************************/
	const unsigned ui_NUM_LIGHTS;

	/******************************************************************************/
	/*!
			Light* P_lights:
	\brief	Pointers to an array of lights
	*/
	/******************************************************************************/
	Light* P_lightsArray;

	// UI
	/******************************************************************************/
	/*!
	float f_LogoScreenTimer:
	\brief	timer for splash screen
	*/
	/******************************************************************************/
	float f_LogoScreenTimer;

	/******************************************************************************/
	/*!
	int i_ControlChange:
	\brief	a temp int to store the control to change
	*/
	/******************************************************************************/
	int i_ControlChange;

	/******************************************************************************/
	/*!
	unsigned short *us_ControlChange:
	\brief	pointer to point to the control to change
	*/
	/******************************************************************************/
	unsigned short *us_ControlChange;

	/******************************************************************************/
	/*!
	unsigned short us_control:
	\brief	To store the virtual keycodes for the controls
	*/
	/******************************************************************************/
	unsigned short us_control[E_CTRL_TOTAL];

	/******************************************************************************/
	/*!
	int MENU_STATE:
	\brief	Controls the menu state
	*/
	/******************************************************************************/
	E_MENU_STATE MENU_STATE;
	E_MENU_STATE PREV_STATE;

	Color UIColor;
	std::vector<S_BUTTON*> v_buttonList;

	S_BUTTON* FetchBUTTON(std::string name);
	void UpdateButtons(void);
	void RenderButtons(void);

	bool transcomplete;
	Vector3 v3_Menupos[E_M_TOTAL];
	Vector3 v3_MenuCam;

	// Init Functions
	void InitMeshList(void);
	void InitTextureResource(void);
	void InitShadersAndLights(void);
	void InitMenu(void);

	void assignsave(void);
	void saveGame(void);
	SaveFile SF_1;

	// Update/Control Functions
	float MousePosX, MousePosY;
	void UpdateFOV();
	void UpdateControlSettingLabels(unsigned short key, int button);

	// Render 
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextCenter(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color);
	void RenderTextCenterOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderMeshOnScreen(Mesh* mesh, float sizeX, float sizeY, float x, float y);

	/******************************************************************************/
	/*!
			MS modelStack:
	\brief	A stack of transformation matrices with reference to the models
	*/
	/******************************************************************************/
	MS modelStack;

	/******************************************************************************/
	/*!
			MS viewStack:
	\brief	A stack of transformation matrices with reference to the viewer
	*/
	/******************************************************************************/
	MS viewStack;

	/******************************************************************************/
	/*!
			MS projectionStack:
	\brief	A stack of transformation matrices with reference to the projection of the view
	*/
	/******************************************************************************/
	MS projectionStack;
};

#endif
