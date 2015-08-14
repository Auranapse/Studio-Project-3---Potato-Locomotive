/****************************************************************************/
/*!
\file mainscene2D.h
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Portal scene of the Sentry Turret, Companion Cube and Storage Cubes/
!*/
/****************************************************************************/
#ifndef MAINSCENE2D_H
#define MAINSCENE2D_H

#include <irrKlang.h>
#include <vector>
#include <iostream>
#include "Scene.h"
#include "Camera4.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "LoadTGA.h"
#include "BulletInfo.h"
#include "GameObject.h"
#include "GameObjectButton.h"
#include "GameObjectTrigger.h"
#include "SpriteAnimation.h"
#include "savefileHandler.h"
#include "maploader.h"
#include "portal.h"
#include "GabrielDLC.h"
#include "Anim4.h"

/******************************************************************************/
/*!
		Class mainscene2D:
\brief	Define geometries, light sources
*/
/******************************************************************************/

class mainscene2D : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_BACKGROUND,

		GEO_CHAR_PLAYER,
		GEO_CHAR_PLAYER_LIMB,

		GEO_PORTAL_GUN_BLUE,
		GEO_PORTAL_BLUE,
		GEO_PORTAL_ORANGE,
		GEO_PORTAL_BULLET_BLUE,
		GEO_PORTAL_BULLET_ORANGE,

		GEO_DOOR_EXIT,
		GEO_BLOCK_1_BLACK_PANEL,
		GEO_BLOCK_2_WHITE_PANEL,

		GEO_T_BUTTON,
		GEO_T_GRILL,
		GEO_T_LASER,


		GEO_TEXT,

		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,

		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_TYPE,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_LIGHT3_POSITION,
		U_LIGHT3_COLOR,
		U_LIGHT3_POWER,
		U_LIGHT3_KC,
		U_LIGHT3_KL,
		U_LIGHT3_KQ,
		U_LIGHT3_TYPE,
		U_LIGHT3_SPOTDIRECTION,
		U_LIGHT3_COSCUTOFF,
		U_LIGHT3_COSINNER,
		U_LIGHT3_EXPONENT,

		U_NUMLIGHTS,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE_ENABLED1,
		U_COLOR_TEXTURE,
		U_COLOR_TEXTURE1,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLED,

		U_TOTAL,
	};

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

		E_CTRL_TOTAL
	};

	struct Transformation
	{
		float rotateA;
		float translateX;
		float translateY;
		float translateZ;
		bool Camera;
	};

	struct terrain
	{
		bool ColEnable;
		Vector3 position;
		Vector3 scale;
		std::vector<unsigned char> uc_heightmap;
		Mesh* mesh;
	};

	enum SOUND_TYPE
	{
		ST_PANEL,
		ST_STEP,
		ST_JUMP,
		ST_LAND,
		ST_BUZZER,
		ST_PORTAL_SHOOT_BLUE,
		ST_PORTAL_SHOOT_ORANGE,
		ST_PORTAL_OPEN,
		ST_PORTAL_RESET,
		ST_PORTAL_INVALID_SURFACE,
		ST_PORTAL_ENTER_PORTAL,

		ST_TOTAL,
	};

public:
	mainscene2D();
	~mainscene2D();

	virtual void Init();
	virtual void InitShaders();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	unsigned m_vertexArrayID;
	const bool TESTMODE;
	Vector3 gravity_force;

	irrklang::ISoundSource *soundList[ST_TOTAL];

	Mesh* meshList[NUM_GEOMETRY];
	std::vector<GameObjectTrigger*> m_goTriggerList;
	std::vector<GameObjectButton*> m_goButtonList;
	std::vector<GameObject*> m_goList;
	std::vector <BulletInfo*> BIv_BulletList;

	Anim4 A4_Player;
	GameObject *GOp_Player;
	GameObject *GOp_DoorExit;

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float X_camLimit;
	Vector3 mainCamera;

	float f_mouseSensitivity;
	float mousePosX, mousePosY;

	float tileSize;
	MapLoader GAME_MAP;

	Portal PORTAL_GUN;
	GameObject *PB_B, *PB_O;

	//******************************************************************************/
	/*!
	unsigned short us_control:
	\brief	To store the virtual keycodes for the controls
	*/
	/******************************************************************************/
	unsigned short us_control[E_CTRL_TOTAL];

	SaveFile SF_1;
	void assignSave(void);

	int currentLevel;
	bool loadLevel(int level);
	GameObjectTrigger* FetchGOTrigger(void);
	GameObjectButton* FetchGOButton(void);
	GameObject* FetchGO(void);

	float f_fov;
	float f_currentfov;
	float FPScounter;

	bool renderAxis;
	bool mouseEnabled;

	bool DisplayInfo;

	float timer;
	float inputDelay;

	void UpdateSound(double dt);

	void UpdateCamera(double dt);
	void UpdateCharacter(double dt);
	void UpdatePortalGun(double dt);
	void resetPortalGun(void);
	void UpdateGOTriggers(double dt);
	void UpdateGOButton(double dt);
	void UpdateGO(double dt);
	void Shoot(Vector3 Pos, Vector3 Dir, float Speed, float Longevity = 10, float dmg = 100);

	Light lights[4];

	void editFOV(float newFOV);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshin2D(Mesh *mesh, bool enableLight, float size = 1.0f, float x = .0f, float y = .0f, float rotation = 0.f);

	void RenderPortalEffect(GameObject *go);
	void RenderGOTriggers(void);
	void RenderGOButtons(void);
	void RenderGO(GameObject *go);
	void RenderBendy(void);
	void RenderBullet(void);
	void RenderMesh(Mesh *mesh, bool enableLight);
	bool collide2D(Vector3 &Position);
	bool GOcollide2D(GameObject *go1, GameObject *go2);
	GameObject *GOcollide2D(Vector3 &Position);
	GameObject *GOcollide2D(GameObject *go2);
	MS modelStack, viewStack, projectionStack;

	irrklang::ISoundEngine* engine;
};

#endif