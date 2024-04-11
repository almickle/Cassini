#include "ParticleSystem.h"

const string ParticleSystem::systemID = "ParticleSystem";

ParticleSystem::ParticleSystem(Graphics& gfx, ResourceManager& manager, UINT count)
	: System(gfx, manager, systemID)
{
	float lower_bound = -90.0f;   // Lower bound of the range
	float upper_bound = 90.0f; // Upper bound of the range
	mt19937 rng{ random_device{}() };
	uniform_real_distribution<float> distribution(lower_bound, upper_bound);
	for (int i = 0; i < count; i++) {
		float charge = i % 2 == 0 ? 1.0f : -1.0f;
		float radius = 1.0f;
		Particle* ptcl = new Particle(gfx, manager, systemID, radius, charge, { distribution(rng), distribution(rng) + 100, distribution(rng) });
		particles.push_back(ptcl);
		desc.numParticles++;
		if (charge > 0.0f) desc.numPositive++;
		if (charge < 0.0f) desc.numNegative++;
		if (charge == 0.0f) desc.numNeutrals++;
	}
	manager.CreateComputeShader(gfx, systemID, "ParticleCS.cso", SYSTEM);
	vector<XMMATRIX> instanceTransforms(particles.size());
	vector<IntrinsicParticleData> intrinsicData(particles.size());
	vector<ParticleData> inputData(particles.size());
	vector<ParticleData> outputData(particles.size());
	StaticSimulationData staticData = {
		{ bounds.xMin, bounds.yMin, bounds.zMin },
		{ bounds.xMax, bounds.yMax, bounds.zMax },
		particles.size()
	};
	DynamicSimulationData dynamicData = { 0.0f };
	for (int i = 0; i < particles.size(); i++)
	{
		const auto& ptcl = particles[i];
		intrinsicData[i] = { ptcl->GetMass(), ptcl->GetCharge(), ptcl->GetRadius() };
		inputData[i] = { ptcl->GetPosition(), ptcl->GetVelocity() };
		outputData[i] = { ptcl->GetPosition(), ptcl->GetVelocity() };
		instanceTransforms[i] = ptcl->GetTransform();
	}
	intrinsicPtclData = manager.CreateInputStructuredBuffer(gfx, systemID, intrinsicData, COMPUTE_SHADER_BUFFER, 0u, SYSTEM);
	inputPtclData = manager.CreateInputStructuredBuffer(gfx, systemID, inputData, COMPUTE_SHADER_BUFFER, 1u, SYSTEM);
	outputPtclData = manager.CreateOutputStructuredBuffer(gfx, systemID, outputData, 0u, SYSTEM);
	staticBuffer = manager.CreateStaticConstantBuffer(gfx, systemID, staticData, COMPUTE_SHADER_BUFFER, 0u, SYSTEM);
	dynamicBuffer = manager.CreateDynamicConstantBuffer(gfx, systemID, dynamicData, COMPUTE_SHADER_BUFFER, 1u, SYSTEM);
	ptclTransforms = manager.CreateInputStructuredBuffer(gfx, Particle::GetEntityID(), systemID, instanceTransforms, VERTEX_SHADER_BUFFER, 0u);
}

void ParticleSystem::Update(Graphics& gfx, ResourceManager& manager)
{
	UINT divisions = particles.size() / 1024;
	UINT remainder = particles.size() % 1024;
	UINT threadGroups = divisions + (remainder > 0 ? 1 : 0);
	gfx.Dispatch(threadGroups, 1, 1);
	OutputStructuredBuffer<ParticleData>* outputBuffer = reinterpret_cast<OutputStructuredBuffer<ParticleData>*>(manager.GetSystemBufferByIndex(systemID, outputPtclData));
	vector<ParticleData> data = outputBuffer->ReadFromBuffer(gfx);
	manager.UpdateSystemStructuredBuffer(gfx, systemID, inputPtclData, data);
	for (UINT i = 0; i < data.size(); i++)
	{
		particles[i]->SetPosition(data[i].s);
	}
	vector<XMMATRIX> instanceTransforms(particles.size());
	for (UINT i = 0; i < particles.size(); i++)
	{
		instanceTransforms[i] = particles[i]->GetTransform();
	}
	manager.UpdateStructuredBuffer(gfx, systemID, Particle::GetEntityID(), ptclTransforms, instanceTransforms);
}

void ParticleSystem::SpawnControlWindow()
{
	ImGui::Begin("Particle System");
	ImGui::Text("Particle count: %i", desc.numParticles);
	ImGui::Text("Positive: %i", desc.numPositive);
	ImGui::Text("Negative: %i", desc.numNegative);
	ImGui::Text("Neutral: %i", desc.numNeutrals);
	ImGui::End();
}