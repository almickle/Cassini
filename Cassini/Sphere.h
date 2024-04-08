#pragma once
#include "Entity.h"

class Sphere :
	public Entity
{
public:
	Sphere(Graphics& gfx, ResourceManager& manager, const XMFLOAT3& color, const float& in_radius) :
		Entity(gfx, manager, entityID, "Models\\icosphere.txt", "PhongVS.cso", "PhongPS.cso"),
		radius(in_radius)
	{
		SetModelColor(color);
		SetScale({ in_radius, in_radius, in_radius });
	}

	Sphere(Graphics& gfx, ResourceManager& manager, const float& in_radius) :
		Entity(gfx, manager, entityID, "Models\\icosphere.txt", "PhongVS.cso", "PhongPS.cso"),
		radius(in_radius)
	{
		SetScale({ in_radius, in_radius, in_radius });
	}

	float GetRadius() {
		return radius;
	}

	static const string entityID;

private:
	float radius;
};