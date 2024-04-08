#pragma once
#include "Entity.h"

class Model :
	public Entity
{
public:
	Model(Graphics& gfx, ResourceManager& manager) : Entity(gfx, manager, entityID, "Models\\icosphere.txt", "PhongVS.cso", "PhongPS.cso")
	{
		SetPosition({ 0.0f, 1.0f, 10.0f });
	};

	static const string entityID;

private:
	string meshPath = "teapot.txt";
};