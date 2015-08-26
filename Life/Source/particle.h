#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector3.h"
#include "Mesh.h"

class Particle
{
public:
	enum Type
	{
		PAR_SPARKS,
		PAR_BLOOD,
		PAR_MESH,
	};

	int ParticleType;
	Material material;
	Vector3 Pos;
	Vector3 Rotation;
	Vector3 Vel;
	Vector3 Scale;
	float lifetime;
	bool active;
	Mesh *mesh;

	Particle(void);
	~Particle(void);
};

#endif