#pragma once
#include "Entity.h"

class Cylinder :
	public Entity
{
public:
	Cylinder(Graphics& gfx, ResourceManager& manager) :
		Entity(gfx, manager, entityID, "Models\\cylinder.txt", "PhongVS.cso", "PhongPS.cso") {

	}

	static const string entityID;
};

