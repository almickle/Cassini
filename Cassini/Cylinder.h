#pragma once
#include "Entity.h"

class Cylinder :
	public Entity
{
public:
	Cylinder(Graphics& gfx, ResourceManager& manager);
public:
	static const string entityID;
};

