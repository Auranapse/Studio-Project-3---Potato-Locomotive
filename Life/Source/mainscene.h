/****************************************************************************/
/*!
\file mainscene.h
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Main scene
!*/
/****************************************************************************/
#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <vector>
#include <iostream>
#include "Scene.h"
#include "CustomCam1.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "LoadTGA.h"
#include "maploader.h"

#include "TextButton.h"
#include "BulletObject.h"
#include "GameObject.h"
#include "WorldObject.h"
#include "ItemObject.h"
#include "WeaponsObject.h"

#include "SpriteAnimation.h"
#include "particle.h"
#include "SaveHandler.h"
#include "DepthFBO.h"
#include "GBuffer.h"
#include "GabrielDLC.h"

#include "CharacterObject.h"
#include "Player.h"
#include "AI.h"
#include "SecurityCam.h"
#include "SoundEngine.h"


#include "SceneManager.h"
#include "SoundDetect.h"
#include "PressurePlate.h"
#include <sstream>

/******************************************************************************/
/*!
		Class mainscene:
\brief	Define geometries, light sources
*/
/******************************************************************************/

class mainscene : public Scene
{
	enum GRAPHICS_SETTINGS
	{
		GRA_MAX,
		GRA_MEDIUM,
		GRA_LOW,
		GRA_SHIT,
	};

	int Graphics;

	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_REDLINE,
		GEO_GREENLINE,
		GEO_BLUELINE,
		GEO_CROSSHAIR,
		GEO_SKYPLANE,
		GEO_WORLD_CUBE,
		GEO_WORLD_QUAD,

		GEO_LIGHT,
		GEO_TEXT,

		//SKYBOX
		E_GEO_LEFT,
		E_GEO_RIGHT,
		E_GEO_TOP,
		E_GEO_FRONT,
		E_GEO_BACK,
		E_GEO_SKYPLANE,


		//OBJ MODELS--------------
		GEO_OBJCAKE,
		GEO_SECURITYCAMERA,
		GEO_KEYCARD,
		GEO_DOOR,
		GEO_EXIT,

		//Weapons
		GEO_M9,
		GEO_MP5K,
		GEO_SPAS12,
		GEO_KATANA,
		GEO_SCALPLE,
		//Objects
		GEO_ITEM_SYRINGE,

		//Other
		GEO_TABLE,
		GEO_KEYCOUNT,

		//------------------------
		//EFFECTS/OTHERS
		GEO_BULLET,
		GEO_STATUS,

		GEO_SCREEN_OVERLAY,

		GEO_LIGHT_DEPTH_QUAD,
		GEO_POSITION_QUAD,
		GEO_NORMAL_QUAD,
		GEO_AMBIENT_QUAD,
		GEO_DIFFUSE_QUAD,
		GEO_SPECULAR_QUAD,
		GEO_EMISSIVE_QUAD,
		GEO_RENDERING_QUAD,
		GEO_RENDERING_SPHERE,


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

		U_TRANSPARENCY,
		U_GLOW,
		U_GLOW_COLOR,
		U_LENS_EFFECT,

		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
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

		U_LIGHTENABLED,
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

		U_LIGHT_DEPTH_MVP_GPASS,
		U_LIGHT_DEPTH_MVP,
		U_SHADOW_MAP,

		U_MVP_GBUFFER,
		U_MODELVIEW_GBUFFER,
		U_MODELVIEW_INVERSE_TRANSPOSE_GBUFFER,
		U_MATERIAL_AMBIENT_GBUFFER,
		U_MATERIAL_DIFFUSE_GBUFFER,
		U_MATERIAL_SPECULAR_GBUFFER,
		U_MATERIAL_EMISSIVE_GBUFFER,
		U_MATERIAL_SHININESS_GBUFFER,
		U_LIGHTENABLED_GBUFFER,
		U_COLOR_TEXTURE_ENABLED_GBUFFER,
		U_COLOR_TEXTURE_ENABLED1_GBUFFER,
		U_COLOR_TEXTURE_GBUFFER,
		U_COLOR_TEXTURE1_GBUFFER,
		U_LIGHT_DEPTH_MVP_GBUFFER,
		U_SHADOW_MAP_GBUFFER,

		U_MVP_LIGHTPASS,
		U_SCREEN_SIZE_LIGHTPASS,
		U_POSITION_MAP_LIGHTPASS,
		U_NORMAL_MAP_LIGHTPASS,
		U_AMBIENT_MAP_LIGHTPASS,
		U_DIFFUSE_MAP_LIGHTPASS,
		U_SPECULAR_MAP_LIGHTPASS,
		U_EMISSIVE_MAP_LIGHTPASS,
		U_LIGHT_TYPE_LIGHTPASS,
		U_LIGHT_POSITION_LIGHTPASS,
		U_LIGHT_COLOR_LIGHTPASS,
		U_LIGHT_POWER_LIGHTPASS,
		U_LIGHT_RADIUS_LIGHTPASS,

		U_TOTAL,
	};

	enum RENDER_PASS
	{
		RENDER_PASS_PRE,
		RENDER_PASS_GBUFFER,
		RENDER_PASS_LIGHT,
		RENDER_PASS_MAIN,
	};


	/******************************************************************************/
	/*!
	enum E_CTRL_TYPE:
	\brief	Lists the types of controls
	*/
	/******************************************************************************/

	enum E_CTRL_TYPE
	{
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
		E_CTRL_THROW,
		E_CTRL_ATTACK,
		E_CTRL_AIM,
		E_CTRL_ABILITY_1,
		E_CTRL_ABILITY_2,

		E_CTRL_TOTAL
	};

	enum SOUND_TYPE
	{
		ST_STEP,
		ST_JUMP,
		ST_LAND,
		ST_ALERT,
		ST_BACKGROUND,

		ST_SLOWMO_ENTER,
		ST_SLOWMO_EXIT,

		ST_HEARTBEAT,
		ST_BREATHING,
		ST_DEATH,

		ST_WALL_POWER_ENTER,
		ST_WALL_POWER_EXIT,

		ST_OBJ_BREAK,
		ST_WEAPON_CLICK,
		ST_WEAPON_M9_SHOOT,
		ST_WEAPON_KATANA,

		ST_AI_DEATH,
		ST_AI_ALERT,

		ST_CAMERA_SPOTTED,
		ST_CAMERA_FOUND,

		ST_STATUS,

		ST_TOTAL,
	};

	enum POWER_TYPE
	{
		PT_SLOWMO,
		PT_SUPERVISION,
	};

	enum ITEMOBJECT_PRESETS
	{
		IO_SYRINGE,
		IO_TOTAL,
	};

	enum WEAPONOBJECT_PRESETS
	{
		WO_KATANA,
		WO_M9,
		WO_SCALPLE,
		WO_TOTAL,
	};

	enum GS_STATE
	{
		GS_PLAY,
		GS_PAUSED,
		GS_DEATH,
		GS_END,
	};

public:
	mainscene();
	~mainscene();

	virtual void Init();
	virtual void InitShaders();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderPassGPass();
	void RenderPassMain();
	void RenderPassGBuffer();
	void RenderPassLight();

	void RenderWorldShadow();
	void RenderWorldNoShadow();

private:
	unsigned m_vertexArrayID;
	unsigned m_gPassShaderID;
	DepthFBO m_lightDepthFBO;
	unsigned m_gBufferShaderID;
	GBuffer m_gBuffer;

	unsigned m_lightShaderID;

	double d_dt, d_dt2;
	GS_STATE GAMESTATE;

	void InitMenus(void);
	Color UIColor, UIColorPressed;
	std::vector<TextButton*> v_buttonList;
	TextButton *FetchBUTTON(std::string name);
	void UpdateButtons(void);
	void RenderButtons(void);

	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;

	RENDER_PASS m_renderPass;

	bool enableFOG;
	const bool TESTMODE;
	Vector3 gravity_force;

	irrklang::ISoundSource *soundList[ST_TOTAL];

	Mesh* meshList[NUM_GEOMETRY];
	std::vector<GameObject*> m_goList;
	std::vector<Particle*> m_ParList;

	void UpdateGO(double &dt);
	void UpdateBO(BulletObject *BO, double &dt);
	void UpdateCO(CharacterObject *CO, double &dt);

	WorldObject *Floor, *Celling, *SWALL1, *SWALL2, *SWALL3, *SWALL4;
	WorldObject *WO_END;

	ItemObject IO_presetList[IO_TOTAL];
	WeaponsObject WO_presetList[WO_TOTAL];

	Player P_Player;
	SoundDetect *PlayerSound;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	CustomCam1 FPC;
	bool b_step;
	float f_mouseSensitivity;
	float mousePosX, mousePosY;

	/******************************************************************************/
	/*!
	unsigned short us_control:
	\brief	To store the virtual keycodes for the controls
	*/
	/******************************************************************************/
	unsigned short us_control[E_CTRL_TOTAL];

	SaveHandler SH_1;
	void assignSave(bool save = false);

	float f_defaultfov;
	float f_fov;
	float f_targetfov;
	float FPScounter;
	bool renderAxis;
	bool mouseEnabled;

	bool DisplayInfo;

	float f_curRecoil;
	float firerate;
	float timer;
	float inputDelay;
	int currentWeapon;

	POWER_TYPE CurrentPower;
	bool PowerActive;
	float f_powerTint;
	float f_powerTintSet;
	Color c_powerColor;
	float f_poweramount;
	float f_playerHealthTint;

	MapLoader GAME_MAP;
	bool loadLevel(int level);
	void loadLevel_GenerateOBJ(std::string object, Vector3 &Position, Vector3 &rotation = Vector3(0, 0, 0));

	int currentLevel;
	Particle* FetchParticle(void);
	BulletObject* FetchBullet(void);

	void UpdateSound(double &dt);
	void UpdatePlayer(double &dt);
	void UpdatePlayerPower(double &dt);
	void generateParticle(Vector3 &Pos, Vector3 &scale, Vector3 &Velocity, Vector3 &Rotation = Vector3(0.f, 0.f, 0.f), int type = Particle::PAR_MESH, float lifetime = 5.f, Mesh *mesh = NULL);
	void generateCharacterParticle(CharacterObject *CO, Vector3 &HeadVel, Vector3 &ArmLeftVel, Vector3 &ArmRightVel, Vector3 &LegLeftVel, Vector3 &LegRightVel, Vector3 &BodyVel);
	void UpdateParticles(double &dt);

	void Shoot(const Vector3 &Pos, const Vector3 &Dir, float Speed, float Longevity = 10, bool melee = false);
	void weaponsUpdate(double &dt);

	const int NUM_LIGHT_PARAM;
	Light lights[4];

	void editFOV(float &newFOV);

	void initWeapons(void);

	void RenderGO(GameObject *go);
	void RenderCharacter(CharacterObject *CO);
	void RenderParticles(void);
	void RenderMesh(Mesh *mesh, bool enableLight, bool enableFog = true, float visibility = 100.f, float glow = 0.f, Color &glowColor = Color(1, 0, 0), Material *material = NULL);
	void RenderMeshin2D(Mesh *mesh, bool enableLight, float visibility = 100.f, float glow = 0.f, Color &glowColor = Color(1, 0, 0));
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size = 1.f, float x = 0.f, float y = 0.f);
	void RenderSkybox(void);
	void RenderUI(void);
	void RenderAIDebugging(CharacterObject * CO);
	void RenderSCDebugging(SecurityCam * SC);
	bool collide(Vector3 &Position);
	bool collideGO(GameObject *go, GameObject *go2);
	MS modelStack, viewStack, projectionStack;
	SoundEngine SE_Engine;


	//Ray Collision
	bool CollisionBetween(Vector3, Vector3&);
	void checkDoor();
	void checkKey();
	void checkStatus();
	void pushPlayer(float pushForce = 30);
	void CheckPlayerSound(void);
	void addStatus(std::string, double);
	void alertDeath(Vector3 pos, float alertRadius = 100.f);
	void layTrap(Vector3 pos);
	void activateTrap();
	void stopAI(float pushForce);
	std::vector<PressurePlate*> Dialogues[10];
	std::vector<CollisionBox> Keys;
	std::vector<CollisionBox> Doors;
	std::vector<CollisionBox> PulseBombs;
	std::vector<CollisionBox> Stopper;
	std::string status;
	double statusTimer;
	int KeyCount;
	int bombCount;
	float KeyRotate;
	float DoorRotate;

};

#endif