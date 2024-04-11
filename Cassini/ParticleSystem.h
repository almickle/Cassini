#pragma once
#include "Particle.h"
#include "System.h"

struct SystemDescription {
	UINT numParticles = 0u;
	UINT numPositive = 0u;
	UINT numNegative = 0u;
	UINT numNeutrals = 0u;
};

class ParticleSystem : public System
{
public:
	ParticleSystem(Graphics& gfx, ResourceManager& manager, UINT numParticles);
public:
	void Update(Graphics& gfx, ResourceManager& manager) override;
private:
	vector<Particle*> particles;
	SimulationBoundary bounds = { -100.0f, 100.0f, 200.0f, 0.0f, -100.0f, 100.0f };
	SystemDescription desc;
private:
	UINT intrinsicPtclData;
	UINT inputPtclData;
	UINT outputPtclData;
	UINT staticBuffer;
	UINT dynamicBuffer;
	UINT ptclTransforms;
	UINT ptclColors;
public:
	void SpawnControlWindow();
private:
	static const string systemID;
};

