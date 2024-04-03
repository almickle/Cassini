#pragma once
#include "Graphics.h"

class GraphicsResource
{
public:
	virtual ~GraphicsResource() {};
	virtual void Bind(Graphics& gfx) const = 0;
};

class InstanceResource {
public:
	virtual ~InstanceResource() {};
	virtual void Bind(Graphics& gfx, UINT slot = 0u) = 0;
public:
	UINT shaderType;
};

