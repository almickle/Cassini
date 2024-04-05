#pragma once
#include "Sphere.h"

class Atom :
	public Sphere
{
public:
	Atom(Graphics& gfx, ResourceManager& manager, const XMFLOAT3& color, const float& in_radius, const float& in_charge, XMFLOAT3 in_pos)
		: Sphere(gfx, manager, color, in_radius)
		, charge(in_charge)
	{
		SetPosition(in_pos);
	}

	void UpdateModel(float dt) override {
		XMFLOAT3 oldPosition = GetPosition();
		XMFLOAT3 newPosition;
		XMStoreFloat3(&newPosition, XMLoadFloat3(&oldPosition) + velocity * dt);
		SetPosition(newPosition);
	}

public:
	const float GetCharge() {
		return charge;
	}

private:
	XMVECTOR velocity = { 0.0f, 0.0f, 0.0f };
private:
	const float mass = 1.0f;
	const float charge;
};