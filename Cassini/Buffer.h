#pragma once
#include "DataTypes.h"
#include "Graphics.h"

class Buffer
{
protected:
	~Buffer() {};
	Buffer() {};
	Buffer(UINT in_slot) : slot(in_slot) {};
public:
	virtual void Bind(Graphics& gfx) const = 0;
protected:
	ComPtr<ID3D11Buffer> pBuffer;
	UINT slot = 0u;
	UINT stride = 0u;
	UINT offset = 0u;
};

