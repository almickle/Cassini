#pragma once
#include "Entity.h"
#include "Particle.h"

class ParticleSystem :
	public Entity
{
public:
	ParticleSystem(Graphics& gfx, ResourceManager& manager, const vector<Particle*>& particles);
private:
	static const string entityID;
};

