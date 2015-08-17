#ifndef WEAPONSOBJECT_H
#define WEAPONSOBJECT_H

#include "ItemObject.h"
#include <irrKlang.h>

class WeaponsObject : public ItemObject
{
public:
	WeaponsObject();
	~WeaponsObject();
	
	float shootvelocity;
	float recoilEffect;

	std::string name;
	float attackRate;
	float reloadTime;
	float adsZoom;
	float bulletSpread;
	int numBullet;
	int CurrentClip;
	int ClipSize;
	
	bool animState;

	Vector3 pos1;//Animation for first position
	Vector3 pos2;//Position final frame
	Vector3 Rotation1;//Animation for rotation
	Vector3 Rotation2;//Rotation final frame

	irrklang::ISoundSource* AttackSound;
	irrklang::ISoundSource* reloadSound;

	void toggleAnimation(void);
	void Update(double &dt);
};

#endif