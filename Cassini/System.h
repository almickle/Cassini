#pragma once
#include "Graphics.h"
#include "ResourceManager.h"

class System
{
protected:
	System(Graphics& gfx, ResourceManager& manager, const string& systemID);
	virtual ~System() {};
public:
	virtual void Update(Graphics& gfx, ResourceManager& manager) = 0;
private:
	const string systemID;
};

