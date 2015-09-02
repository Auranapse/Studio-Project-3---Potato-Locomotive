/****************************************************************************/
/*!
\file SoundEngine.cpp
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
File to handle all sounds effects
!*/
/****************************************************************************/
#include "SoundEngine.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
SoundEngine::SoundEngine()
{

}

/******************************************************************************/
/*!
\brief
Default destructor
*/
/******************************************************************************/
SoundEngine::~SoundEngine()
{
	
}

/******************************************************************************/
/*!
\brief
Initialise the sound engine
*/
/******************************************************************************/
void SoundEngine::Init(void)
{
	engine = createIrrKlangDevice(ESOD_AUTO_DETECT, ESEO_MULTI_THREADED | ESEO_LOAD_PLUGINS | ESEO_USE_3D_BUFFERS);
	f_distortionEffect = -30.f;
	distortionEnabled = false;
}

/******************************************************************************/
/*!
\brief
Preloads a sound
\param dir
directory of the sound
\return
returns a ISoundSource pointer to the preloaded sound
*/
/******************************************************************************/
ISoundSource* SoundEngine::preloadSound(const char* dir)
{
	ISoundSource *returnsource;
	returnsource = engine->addSoundSourceFromFile(dir, ESM_AUTO_DETECT, true);
	return returnsource;
}

/******************************************************************************/
/*!
\brief
Checks if a sound is currently playing in runtime
\param sound
the pointer to the sound source
\return
returns true if it is still playing
*/
/******************************************************************************/
bool SoundEngine::isSoundPlaying(ISoundSource *sound)
{
	return engine->isCurrentlyPlaying(sound);
}

/******************************************************************************/
/*!
\brief
Enables or disables distortion effect
\param enable
if true will enable effect
\param gain
the gain of the effect
*/
/******************************************************************************/
void SoundEngine::effectDistortion(bool enable, float gain)
{
	distortionEnabled = enable;
	f_distortionEffect = gain;
}

/******************************************************************************/
/*!
\brief
Plays a sound in 2D
\param sound
the sound source to play
\param loop
if true, will loop the sound
*/
/******************************************************************************/
void SoundEngine::playSound2D(ISoundSource* sound, bool loop, bool effects, bool distortionOverwrite)
{
	ISound *snd = engine->play2D(sound, loop, false, false, effects);
	if (distortionEnabled || distortionOverwrite)
	{
		if (snd)
		{
			ISoundEffectControl *fx = snd->getSoundEffectControl();
			fx->enableDistortionSoundEffect(f_distortionEffect);
		}
	}

	if (loop)
	{
		v_soundList.push_back(snd);
	}
}

/******************************************************************************/
/*!
\brief
Plays a sound in 3D
\param sound
the sound source to play
\param loop
if true, will loop the sound
*/
/******************************************************************************/
void SoundEngine::playSound3D(ISoundSource* sound, Vector3 &pos, bool loop, bool effects, bool distortionOverwrite)
{
	ISound *snd = engine->play3D(sound, vec3df(pos.x, pos.y, pos.z), loop, false, false, effects);

	if (distortionEnabled || distortionOverwrite)
	{
		if (snd)
		{
			ISoundEffectControl *fx = snd->getSoundEffectControl();
			fx->enableDistortionSoundEffect(f_distortionEffect);
		}
	}

	if (loop)
	{
		v_soundList.push_back(snd);
	}
}

/******************************************************************************/
/*!
\brief
Stops playing all sounds
*/
/******************************************************************************/
void SoundEngine::stopAllSounds(void)
{
	engine->stopAllSounds();
}

/******************************************************************************/
/*!
\brief
Updates engine effects
*/
/******************************************************************************/
void SoundEngine::Update(void)
{
	for (unsigned i = 0; i < v_soundList.size(); ++i)
	{
		if (v_soundList[i] != NULL)
		{
			ISoundEffectControl *fx = v_soundList[i]->getSoundEffectControl();
			if (distortionEnabled)
			{
				if (!fx->isDistortionSoundEffectEnabled())
				{
					fx->enableDistortionSoundEffect(f_distortionEffect);
				}
			}
			else
			{
				if (fx->isDistortionSoundEffectEnabled())
				{
					fx->disableDistortionSoundEffect();
				}
			}
		}
	}
}

/******************************************************************************/
/*!
\brief
Updates the position and direction of the "ears"
\param position
position of the "ear"
\param direction
the directional vector of where the "ear" is looking
\param up
the up vector of the "ear"
*/
/******************************************************************************/
void SoundEngine::UpdateListenerPosition(Vector3 &position, Vector3 &direction, Vector3 &up)
{
	engine->setListenerPosition(vec3df(position.x, position.y, position.z), vec3df(-(direction.x), direction.y, -(direction.z)), vec3df(0, 0, 0), vec3df(up.x, up.y, up.z));
}

/******************************************************************************/
/*!
\brief
delete the sound engine
*/
/******************************************************************************/
void SoundEngine::Exit(void)
{
	engine->drop();
}