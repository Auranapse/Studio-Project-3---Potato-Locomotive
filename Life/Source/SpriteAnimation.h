#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include "Mesh.h"
#include "Vertex.h"
#include "GL\glew.h"
#include "MyMath.h"
#include "SpriteAnimation.h"

struct Animation
{
	int startFrame, endFrame;
	float animTime;
	bool repeat;
	bool ended;

	Animation()
	{

	}

	void Set(int startFrame, int endFrame, float time, bool repeat = true)
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->animTime = time;
		this->repeat = repeat;
	}
};

class SpriteAnimation : public Mesh
{
public:
	SpriteAnimation(const std::string &meshName, int row, int col);
	~SpriteAnimation();
	void Update(double dt);
	virtual void Render();

	int m_row;
	int m_col;

	float m_currentTime;
	int m_currentFrame;
	int m_playCount;
	Animation *m_anim;
};

#endif