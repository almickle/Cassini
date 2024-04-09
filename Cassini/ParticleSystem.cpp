#include "ParticleSystem.h"

const string ParticleSystem::entityID = "ParticleSystem";

ParticleSystem::ParticleSystem(Graphics& gfx, ResourceManager& manager, const vector<Particle*>& particles) : Entity(gfx, manager, entityID, "Models\\icosphere.txt")
{
	vector<XMMATRIX> positions;
	for (int i = 0; i < particles.size(); i++) {
		positions.push_back(particles[i]->GetTransformation());
	}
	XMMATRIX viewProjMatrix = gfx.GetCameraView() * gfx.GetProjection();
	manager.CreateStaticConstantBuffer(gfx, entityID, 0u, viewProjMatrix);
	manager.CreateInstanceBuffer(gfx, entityID, positions);
}