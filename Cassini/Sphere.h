#pragma once
#include "Entity.h"

class Sphere :
	public Entity
{
public:
	Sphere(Graphics& gfx, ResourceManager& manager, const XMFLOAT3& color, const float& in_radius) :
		Entity(gfx, manager, "Models\\icosphere.txt", "PhongVS.cso", "PhongPS.cso"),
		radius(in_radius)
	{
		SetModelColor(color);
		SetPosition({ 5.0f, 5.0f, 5.0f });
		SetScale({ in_radius, in_radius, in_radius });
	}

private:
	float radius;
};

