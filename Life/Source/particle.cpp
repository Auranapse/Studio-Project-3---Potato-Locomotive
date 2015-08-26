#include "particle.h"

Particle::Particle(void)
{
	Pos.SetZero();
	Vel.SetZero();
	Scale.Set(1.f, 1.f, 1.f);
	active = false;
	ParticleType = PAR_SPARKS;
	lifetime = 1.f;
}

Particle::~Particle(void)
{

}