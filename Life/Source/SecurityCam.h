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

	void setRotationAngle(float f_rotationAngle);
	void update(const double &dt, Vector3 &playerPos, std::vector<GameObject*> m_goList);
	CAMERA_STATE getState();

	float getCameraFOV();
	float getCameraRange();
	Vector3 Lookat;
	float offsetY;
private:
	CAMERA_STATE c_State;

	bool rotationState;
	float f_rotationLimiter;
	float f_rotationAngle;
	static float f_cameraFOV;
	static float f_cameraRange;
};

