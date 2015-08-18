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

#include <irrKlang.h>
#include <ik_ISoundEffectControl.h>
#include <vector>
#include <iostream>
#include "Scene.h"
#include "Camera4.h"
#include "CustomCam1.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "LoadTGA.h"
#include "maploader.h"

#include "BulletInfo.h"
#include "GameObject.h"
#include "WorldObject.h"
#include "ItemObject.h"
#include "WeaponsObject.h"

#include "SpriteAnimation.h"
#include "particle.h"
#include "savefile.h"
#include "DepthFBO.h"
#include "GBuffer.h"
#include "GabrielDLC.h"

#include "CharacterObject.h"
#include "Player.h"
#include "AI.h"
#include "Collision.h"

/******************************************************************************/
/*!
		Class mainscene:
\brief	Define geometries, light sources
*/
/******************************************************************************/

class mainscene : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_SNIPER_CROSSHAIR_1,
		GEO_SNIPER_CROSSHAIR_2,
		GEO_SKYPLANE,
		GEO_FLOOR_TILE,
		GEO_WORLD_CUBE,

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

		//GUNS
		GEO_M9,
		GEO_MP5K,
		GEO_SPAS12,
		//------------------------
		//EFFECTS/OTHERS
		GEO_BULLET,

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
	
	enum SOUND_TYPE
	{
		ST_PANEL,
		ST_STEP,
		ST_STEP_2,
		ST_KILL,
		ST_BUZZER,
		ST_ALERT,
		
		ST_WEAPON_CLICK,
		ST_WEAPON_M9_SHOOT,

		ST_TOTAL,
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

	double d_dt;

	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;

	RENDER_PASS m_renderPass;

	bool enableFOG;
	const bool TESTMODE;
	Vector3 gravity_force;

	irrklang::ISoundSource *soundList[ST_TOTAL];

	Mesh* meshList[NUM_GEOMETRY];
	std::vector<ItemObject*> m_itemList;
	std::vector<GameObject*> m_goList;
	std::vector<Particle*> m_ParList;
	std::vector<BulletInfo*> BIv_BulletList;

	Player P_Player;
	WeaponsObject *B;

	AI ai;

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera4 camera;
	CustomCam1 FPC;
	float f_step;
	float f_mouseSensitivity;

	/******************************************************************************/
	/*!
	unsigned short us_control:
	\brief	To store the virtual keycodes for the controls
	*/
	/******************************************************************************/
	unsigned short us_control[E_CTRL_TOTAL];

	float CamRotationYaw, CamRotationPitch;

	SaveFile SF_1;
	void assignSave(void);

	float f_fov;
	float f_currentfov;
	float FPScounter;
	bool renderAxis;
	bool mouseEnabled;

	bool DisplayInfo;


	bool weaponsEnabled;
	float f_curRecoil;
	float firerate;
	float timer;
	float inputDelay;
	int currentWeapon;

	MapLoader GAME_MAP;
	bool loadLevel(int level);
	Particle* FetchParticle(void);

	void UpdateSound(double &dt);
	void UpdatePlayer(double &dt);
	void UpdateGO(double &dt);
	void generateParticle(Vector3 &Pos, Vector3 &scale, Vector3 &Velocity, int type = Particle::PAR_DEFAULT, float lifetime = 5.f);
	void UpdateParticles(double &dt);
	void UpdateBullets(double &dt);

	void Shoot(Vector3 Pos, Vector3 Dir, float Speed, float Longevity = 10, float dmg = 100);
	void weaponsUpdate(double &dt);

	const int NUM_LIGHT_PARAM;
	Light lights[4];

	void editFOV(float &newFOV);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshin2D(Mesh *mesh, bool enableLight, float size = 1.0f, float x = .0f, float y = .0f, float rotation = 0.f);

	void initWeapons(void);
	void generateRoom1(void);

	void RenderGO(GameObject *go);
	void RenderCharacter(CharacterObject *CO);
	void RenderParticles(void);
	void RenderBullet(void);
	void RenderMesh(Mesh *mesh, bool enableLight, bool enableFog = true, Material *material = NULL);
	void RenderSkybox(void);
	void RenderUI(void);
	bool collide(Vector3 &Position, bool bullet = false);
	bool collideGO(GameObject *go);
	MS modelStack, viewStack, projectionStack;
	irrklang::ISoundEngine* engine;
};

#endif