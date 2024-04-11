#include "Particle.h"

const string Particle::entityID = "Particle";
const string Particle::GetEntityID() { return entityID; }

Particle::Particle(Graphics& gfx, ResourceManager& manager)
	: Sphere(gfx, manager, entityID)
{

}

Particle::Particle(Graphics& gfx, ResourceManager& manager, const string& systemID, const float& in_radius, const float& in_charge, XMFLOAT3 in_pos)
	: Sphere(gfx, manager, entityID, systemID, in_radius, in_charge == 0.0f ? XMFLOAT3{ 1.0f, 1.0f, 0.0f } : in_charge > 0.0f ? XMFLOAT3{ 0.0f, 0.0f, 1.0f } : XMFLOAT3{ 1.0f, 0.0f, 0.0f }, in_pos),
	charge(in_charge)
{
	XMFLOAT3 color = in_charge == 0.0f ? XMFLOAT3{ 1.0f, 1.0f, 0.0f } : in_charge > 0.0f ? XMFLOAT3{ 0.0f, 0.0f, 1.0f } : XMFLOAT3{ 1.0f, 0.0f, 0.0f };
}

const float Particle::GetCharge() const {
	return charge;
}

const float Particle::GetMass() const {
	return mass;
}
XMFLOAT3 Particle::GetVelocity() const {
	return velocity;
}