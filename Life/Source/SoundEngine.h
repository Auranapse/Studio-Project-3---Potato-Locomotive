/****************************************************************************/
/*!
\file SoundEngine.h
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
File to handle all sounds effects
!*/
/****************************************************************************/
#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include "Vector3.h"
#include <irrKlang.h>
#include <ik_ISoundEffectControl.h>
#pragma comment (lib, "irrKlang.lib")
#include <vector>

using namespace irrklang;

class SoundEngine
{
public:
	SoundEngine();
	~SoundEngine();

	ISoundSource* preloadSound(const char* dir);


	bool isSoundPlaying(ISoundSource* sound);
	void effectDistortion(bool enable, float gain = -30.f);
	void playSound2D(ISoundSource* sound, bool loop = false, bool effects = true, bool distortionOverwrite = false);
	void playSound3D(ISoundSource* sound, Vector3 &pos, bool loop = false, bool effects = true, bool distortionOverwrite = false);
	void stopAllSounds(void);

	void Init(void);
	void Update(void);
	void UpdateListenerPosition(Vector3 &position, Vector3 &direction, Vector3 &up);
	void Exit(void);
private:
	bool distortionEnabled;
	float f_distortionEffect;

	std::vector<ISound*> v_soundList;
	ISoundEngine* engine;
};

#endif