#pragma once
#include "Sphere.h"

class Particle :
	public Sphere
{
public:
	Particle(Graphics& gfx, ResourceManager& manager, const float& in_radius, const float& in_charge, XMFLOAT3 in_pos, XMFLOAT3 in_vel = { 0.0f, 0.0f, 0.0f })
		: Sphere(gfx, manager, in_radius),
		charge(in_charge),
		velocity(in_vel)
	{
		XMFLOAT3 color = in_charge > 0.0f ? XMFLOAT3(0.0f, 0.0f, 1.0f) : XMFLOAT3(1.0f, 0.0f, 0.0f);
		if (in_charge == 0.0f) {
			color = { 1.0f, 1.0f, 0.0f };
		}
		SetModelColor(color);
		SetPosition(in_pos);
	}

	void UpdateModel(float dt) override {
		//XMFLOAT3 oldPosition = GetPosition();
		//XMFLOAT3 newPosition;
		//XMStoreFloat3(&newPosition, XMLoadFloat3(&oldPosition) + XMLoadFloat3(&velocity) * dt);
		//SetPosition(newPosition);
	}

public:
	const float GetCharge() {
		return charge;
	}

	const float GetMass() {
		return mass;
	}
	XMFLOAT3 GetVelocity() {
		return velocity;
	}

private:
	XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };
private:
	const float mass = 1.0f;
	const float charge;
};