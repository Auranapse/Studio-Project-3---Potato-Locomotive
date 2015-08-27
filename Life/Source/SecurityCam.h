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

	void update(const double &dt, Vector3 &playerPos, std::vector<CharacterObject*> charList);
	float getAngle();
	CAMERA_STATE getState();
private:
	float angle;
	Vector3 Lookat;
	CAMERA_STATE c_State;
};

