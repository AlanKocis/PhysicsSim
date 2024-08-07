#ifndef FORCE_GEN_H
#define FORCE_GEN_H
#include <h/Particle.h>
#include <vector>

class ParticleForceGenerator
{
public:
	virtual void updateForce(Particle *particle, float time) = 0;
};
typedef ParticleForceGenerator pfGen;

class ParticleGravity : public ParticleForceGenerator
{
private:
	glm::vec3 G{ 0.0F, -10.0F, 0.0F };
public:
	virtual void updateForce(Particle *particle, float time) override;
};



struct regEntry
{
	Particle *particle;
	pfGen *pfg;
};

class ParticleForceRegistry
{
protected:
	std::vector<struct regEntry> registry;
public:
//	registers the force generator to apply to the particle
	void add(Particle *particle, pfGen *pfg)
	{
		this->registry.push_back({ particle, pfg });
	}
//	removes given pair from the registry
	void remove(Particle *particle, pfGen *pfg);
	void clear();
	void updateForces(float time);
};
#endif