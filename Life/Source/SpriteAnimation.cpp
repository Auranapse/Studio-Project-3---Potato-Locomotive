#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation(const std::string &meshName, int row, int col) : Mesh(meshName), m_row(row), m_col(col), m_currentTime(0), m_currentFrame(0), m_playCount(0)
{
	m_anim = NULL;
}

SpriteAnimation::~SpriteAnimation()
{
	if(m_anim)
	{
		delete m_anim;
	}
}

void SpriteAnimation::Update(double dt)
{
	if(m_anim)
	{
		m_currentTime += static_cast<float>(dt);
		int numFrame = Math::Max(1, m_anim->endFrame - m_anim->startFrame + 1);
		float frameTime = m_anim->animTime / numFrame;
		m_currentFrame = Math::Min(m_anim->endFrame, (m_anim->startFrame + static_cast<int>(m_currentTime/frameTime)));
		
		if(m_anim->repeat == true)
		{
			if(m_currentFrame >= m_anim->endFrame)
			{
				++m_playCount;
				m_currentTime = 0;
				m_currentFrame = m_anim->startFrame;
			}
		}
	}
}

void SpriteAnimation::Render(void)
{
	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	//Render call setup
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));

	
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}