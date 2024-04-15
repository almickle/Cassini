#pragma once
#include "Bond.h"
#include "Particle.h"
#include "System.h"

struct SystemDescription {
	UINT numParticles = 0u;
	UINT numPositive = 0u;
	UINT numNegative = 0u;
	UINT numNeutrals = 0u;
	UINT threadGroups = 0u;
};

class ParticleSystem : public System
{
public:
	ParticleSystem(Graphics& gfx, ResourceManager& manager, UINT numParticles);
public:
	void Update(Graphics& gfx, ResourceManager& manager, float dt) override;
	void RenderSnapshot(Graphics& gfx, ResourceManager& manager, vector<ParticleData>);
public:
	Particle* GetParticle(UINT index) const;
	Bond* GetBond(UINT index) const;
	vector<UINT> GetResources() const;
	vector<ParticleData> TakeSnapShot(Graphics& gfx, ResourceManager& manager) const;
	XMFLOAT3 GetCentroid() const
	{
		XMFLOAT3 centroid = { 0.0f, 0.0f, 0.0f };
		for (auto& ptcl : particles) {
			XMFLOAT3 pos = ptcl->GetPosition();
			XMStoreFloat3(&centroid, (XMLoadFloat3(&centroid) + XMLoadFloat3(&pos)));
		}
		XMStoreFloat3(&centroid, (XMLoadFloat3(&centroid) / particles.size()));
		return centroid;
	}
private:
	vector<Particle*> particles;
	vector<Bond*> bonds;
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
	UINT bondData;
	UINT bondTransforms;
	UINT bondColors;
private:
	float kineticEnergy;
	float potentialEnergy;
	float totalEnergy;
public:
	void SpawnControlWindow();
private:
	static const string systemID;
};

