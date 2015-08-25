#ifndef WEAPONSOBJECT_H
#define WEAPONSOBJECT_H

#include "ItemObject.h"
#include <irrKlang.h>

class WeaponsObject : public ItemObject
{
private:
	bool animComplete;
	float timer;
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
	float AnimSpeed;
	int numBullet;
	int CurrentClip;
	
	bool animState;

	Vector3 pos2;//Position final frame
	Vector3 Rotation2;//Rotation final frame

	int AttackSound;

	void toggleAnimation(void);
	virtual void Update(double &dt);
	bool isAnimationComplete(void);
	void isAnimationComplete(bool isitreally);
};

#endif