#pragma once
#include "Cylinder.h"
#include "Particle.h"

class Bond :
	public Cylinder
{
public:
	Bond(Graphics& gfx, ResourceManager& manager, const string& systemID, Particle* bondingPair[2], UINT indexA, UINT indexB);
public:
	static const string GetEntityID();
public:
	void Update(Graphics& gfx, ResourceManager& manager) override;
public:
	Particle* GetParticle(UINT index) const;
	XMMATRIX GetUpdatedTransform() const;
	XMFLOAT3 GetStretch() const;
	XMFLOAT3 GetSpin() const;
	XMFLOAT3 GetMove() const;
	float GetEnergy() const;
	float GetLength() const;
	UINT GetIndexA() const;
	UINT GetIndexB() const;
private:
	Particle* pair[2];
	UINT indices[2];
	float energy = 1.0f;
	float length = 3.0f;
private:
	static const string entityID;
};
