#pragma once
#include "GameObject.h"
#include "GabrielDLC.h"
#include "Collision.h"
#include "AI.h"
#include "CharacterObject.h"

class SecurityCam : public GameObject
{
public:
	enum CAMERA_STATE
	{
		SPOTTED,
		FOUND,
		NOTFOUND,
	};

	SecurityCam(void);
	~SecurityCam(void);

	void update(const double &dt, Vector3 &playerPos, std::vector<GameObject*> m_goList);
	float getAngle();
	CAMERA_STATE getState();

	double getCameraRange_Angle();
	double getCameraRange();
	Vector3 Lookat;

private:
	CAMERA_STATE c_State;

	static double d_cameraRange_Angle;
	static double d_cameraRange;
};

