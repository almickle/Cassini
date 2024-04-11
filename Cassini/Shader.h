#pragma once
#include "Graphics.h"
#include "Utility.h"

class Shader
{
protected:
	~Shader() {};
public:
	virtual void Bind(Graphics& gfx) const = 0;
public:
	ComPtr<ID3DBlob> GetBlob() { return pBlob; }
protected:
	ComPtr<ID3DBlob> pBlob;
};

