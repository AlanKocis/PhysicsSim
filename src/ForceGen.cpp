#include <h/ForceGen.h>

void ParticleForceRegistry::updateForces(float time)
{
	for (struct regEntry &pair : this->registry)
	{
		pair.pfg->updateForce(pair.particle, time);
	}
}

void ParticleGravity::updateForce(Particle *particle, float time)
{
	if (particle->hasInfiniteMass())
		return;
	glm::vec3 scale = particle->getMass() * this->G;
	particle->addForce(scale);
}
