#pragma once
#include "Entity.h"

class Grid : public Entity
{
public:
	Grid(Graphics& gfx, ResourceManager& manager) : Entity(gfx, manager, "Models\\plane.txt", "PhongVS.cso", "GridPS.cso")
	{
		LightBuffer buffer = { gfx.GetLighting(), GetModelColor() };
		UpdatePSData(gfx, manager, buffer);
		SetScale({ 100.0f, 1.0f, 100.0f });
	}
};

