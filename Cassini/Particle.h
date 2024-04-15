#pragma once
#include "Sphere.h"

class Particle :
	public Sphere
{
public:
	Particle(Graphics& gfx, ResourceManager& manager);
	Particle(Graphics& gfx, ResourceManager& manager, const string& systemID, const float& in_radius, const float& in_charge, XMFLOAT3 in_pos);
public:
	static const string GetEntityID();
public:
	const float GetCharge() const;
	const float GetMass() const;
public:
	XMFLOAT3 GetVelocity() const;
private:
	const float mass = 1.0f;
	const float charge = 0.0f;
private:
	XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };
private:
	static const string entityID;
};