#include "ParticleSystem.h"

const string ParticleSystem::systemID = "ParticleSystem";

ParticleSystem::ParticleSystem(Graphics& gfx, ResourceManager& manager, UINT count)
	: System(gfx, manager, systemID)
{
	float lower_bound = -600.0f;   // Lower bound of the range
	float upper_bound = 600.0f; // Upper bound of the range
	mt19937 rng{ random_device{}() };
	uniform_real_distribution<float> distribution(lower_bound, upper_bound);
	UINT bondI = 0u;
	for (int i = 0; i < count; i++) {
		float charge = i % 2 == 0 ? 1.0f : -1.0f;
		float radius = 50.0f; // pm
		float bondLength = 200.0f; // pm
		XMVECTOR posaV = { distribution(rng), distribution(rng) + upper_bound, distribution(rng) };
		XMVECTOR offsetV = XMVector3Normalize({ distribution(rng) / upper_bound, distribution(rng) / upper_bound, distribution(rng) / upper_bound }) * bondLength;
		XMVECTOR posbV = posaV + offsetV;
		XMFLOAT3 posa;
		XMFLOAT3 posb;
		XMStoreFloat3(&posa, posaV);
		XMStoreFloat3(&posb, posbV);
		Particle* ptcla = new Particle(gfx, manager, systemID, radius, charge, posa);
		Particle* ptclb = new Particle(gfx, manager, systemID, radius, -charge, posb);
		Particle* pair[2] = { ptcla, ptclb };
		Bond* bond = new Bond(gfx, manager, systemID, pair, bondI, bondI + 1);
		bondI += 2;
		bonds.push_back(bond);
		particles.push_back(ptcla);
		particles.push_back(ptclb);
	}
	for (auto& ptcl : particles)
	{
		if (ptcl->GetCharge() > 0.0f) desc.numPositive++;
		if (ptcl->GetCharge() < 0.0f) desc.numNegative++;
		if (ptcl->GetCharge() == 0.0f) desc.numNeutrals++;
	}
	desc.numParticles = particles.size();
	manager.CreateComputeShader(gfx, systemID, "RungeKuttaCS.cso", SYSTEM);
	vector<XMMATRIX> instanceTransforms(particles.size());
	vector<IntrinsicParticleData> intrinsicData(particles.size());
	vector<ParticleData> inputData(particles.size());
	vector<ParticleData> outputData(particles.size());
	vector<XMFLOAT3> instanceColors(particles.size());
	StaticSimulationData staticData = {
		{ bounds.xMin, bounds.yMin, bounds.zMin },
		{ bounds.xMax, bounds.yMax, bounds.zMax },
		particles.size(), bonds.size() * 2
	};
	DynamicSimulationData dynamicData = { 0.0f };
	vector<BondData> bondBufferData(bonds.size() * 2);
	vector<XMMATRIX> biTransforms(bonds.size());
	vector<XMFLOAT3> biColors(bonds.size());
	for (int i = 0; i < particles.size(); i++)
	{
		const auto& ptcl = particles[i];
		intrinsicData[i] = { ptcl->GetMass(), ptcl->GetCharge(), ptcl->GetRadius() };
		inputData[i] = { ptcl->GetPosition(), ptcl->GetVelocity() };
		outputData[i] = { ptcl->GetPosition(), ptcl->GetVelocity() };
		instanceTransforms[i] = ptcl->GetTransform();
		instanceColors[i] = ptcl->GetModelColor();
	}
	unsigned int bondIndex = 0;
	for (unsigned int i = 0; i < bonds.size(); i++)
	{
		const auto& bond = bonds[i];
		bondBufferData[bondIndex] = {
			bond->GetEnergy(),
			bond->GetLength(),
			bond->GetIndexA(),
			bond->GetIndexB()
		};
		bondBufferData[bondIndex + 1] = {
			bond->GetEnergy(),
			bond->GetLength(),
			bond->GetIndexB(),
			bond->GetIndexA()
		};
		bondIndex += 2;
		biTransforms[i] = bond->GetTransform();
		biColors[i] = bond->GetModelColor();
	}
	// Particle buffers
	intrinsicPtclData = manager.CreateInputStructuredBuffer(gfx, systemID, intrinsicData, COMPUTE_SHADER_BUFFER, 0u, SYSTEM);
	inputPtclData = manager.CreateInputStructuredBuffer(gfx, systemID, inputData, COMPUTE_SHADER_BUFFER, 1u, SYSTEM);
	outputPtclData = manager.CreateOutputStructuredBuffer(gfx, systemID, outputData, 0u, SYSTEM);
	staticBuffer = manager.CreateStaticConstantBuffer(gfx, systemID, staticData, COMPUTE_SHADER_BUFFER, 0u, SYSTEM);
	dynamicBuffer = manager.CreateDynamicConstantBuffer(gfx, systemID, dynamicData, COMPUTE_SHADER_BUFFER, 1u, SYSTEM);
	ptclTransforms = manager.CreateInputStructuredBuffer(gfx, Particle::GetEntityID(), systemID, instanceTransforms, VERTEX_SHADER_BUFFER, 0u);
	ptclColors = manager.CreateInputStructuredBuffer(gfx, Particle::GetEntityID(), systemID, instanceColors, PIXEL_SHADER_BUFFER, 0u);

	// Bond buffers
	bondData = manager.CreateInputStructuredBuffer(gfx, systemID, bondBufferData, COMPUTE_SHADER_BUFFER, 2u, SYSTEM);
	bondTransforms = manager.CreateInputStructuredBuffer(gfx, Bond::GetEntityID(), systemID, biTransforms, VERTEX_SHADER_BUFFER, 0u);
	bondColors = manager.CreateInputStructuredBuffer(gfx, Bond::GetEntityID(), systemID, biColors, PIXEL_SHADER_BUFFER, 0u);
}

vector<UINT> ParticleSystem::GetResources() const
{
	vector<UINT> resources = { outputPtclData };
	return resources;
}

vector<ParticleData> ParticleSystem::TakeSnapShot(Graphics& gfx, ResourceManager& manager) const
{
	OutputStructuredBuffer<ParticleData>* outputBuffer = reinterpret_cast<OutputStructuredBuffer<ParticleData>*>(manager.GetSystemBufferByIndex(systemID, outputPtclData));
	vector<ParticleData> data = outputBuffer->ReadFromBuffer(gfx);
	return data;
}

void ParticleSystem::RenderSnapshot(Graphics& gfx, ResourceManager& manager, vector<ParticleData> data)
{
	UINT divisions = particles.size() / 1024;
	UINT remainder = particles.size() % 1024;
	desc.threadGroups = divisions + (remainder > 0 ? 1 : 0);
	gfx.Dispatch(desc.threadGroups, 1, 1);
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
	for (UINT i = 0; i < bonds.size(); i++)
	{
		auto& bond = bonds[i];
		bond->OverrideTransform(bond->GetUpdatedTransform());
	}
	vector<XMMATRIX> biTransforms(bonds.size());
	for (UINT i = 0; i < bonds.size(); i++)
	{
		biTransforms[i] = bonds[i]->GetTransform();
	}
	manager.UpdateStructuredBuffer(gfx, systemID, Particle::GetEntityID(), ptclTransforms, instanceTransforms);
	manager.UpdateStructuredBuffer(gfx, systemID, Bond::GetEntityID(), bondTransforms, biTransforms);
	manager.UpdateSystemConstantBuffer(gfx, systemID, dynamicBuffer, 0.0f);
}

void ParticleSystem::Update(Graphics& gfx, ResourceManager& manager, float dt)
{
	UINT divisions = particles.size() / 1024;
	UINT remainder = particles.size() % 1024;
	desc.threadGroups = divisions + (remainder > 0 ? 1 : 0);
	gfx.Dispatch(desc.threadGroups, 1, 1);
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
	for (UINT i = 0; i < bonds.size(); i++)
	{
		auto& bond = bonds[i];
		bond->OverrideTransform(bond->GetUpdatedTransform());
	}
	vector<XMMATRIX> biTransforms(bonds.size());
	for (UINT i = 0; i < bonds.size(); i++)
	{
		biTransforms[i] = bonds[i]->GetTransform();
	}
	manager.UpdateStructuredBuffer(gfx, systemID, Particle::GetEntityID(), ptclTransforms, instanceTransforms);
	manager.UpdateStructuredBuffer(gfx, systemID, Bond::GetEntityID(), bondTransforms, biTransforms);
	manager.UpdateSystemConstantBuffer(gfx, systemID, dynamicBuffer, dt);
}

Particle* ParticleSystem::GetParticle(UINT index) const
{
	return particles[index];
}
Bond* ParticleSystem::GetBond(UINT index) const
{
	if (index < bonds.size() - 1)
		return bonds[index];
	else
		return bonds[0];
}

void ParticleSystem::SpawnControlWindow()
{
	ImGui::Begin("Particle System");
	ImGui::Text("Particle count: %i", desc.numParticles);
	ImGui::Text("Positive: %i", desc.numPositive);
	ImGui::Text("Negative: %i", desc.numNegative);
	ImGui::Text("Neutral: %i", desc.numNeutrals);
	ImGui::Text("Thread groups: %i", desc.threadGroups);
	ImGui::Text("Thread count: %i", desc.threadGroups * 1024);
	ImGui::End();
}