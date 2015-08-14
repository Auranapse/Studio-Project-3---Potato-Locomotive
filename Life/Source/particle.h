#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector3.h"
#include "Mesh.h"

class Particle
{
public:
	enum Type
	{
		PAR_DEFAULT,
		PAR_GEL,
		PAR_TOTAL,
	};

	int ParticleType;
	Material material;
	Vector3 Pos;
	Vector3 Vel;
	Vector3 Scale;
	float lifetime;
	bool active;

	Particle(void);
	~Particle(void);
};

#endif