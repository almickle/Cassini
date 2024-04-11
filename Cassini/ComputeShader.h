#pragma once
#include "Shader.h"

class ComputeShader : public Shader
{
public:
	ComputeShader(Graphics& gfx, string path);
	void Bind(Graphics& gfx) const override;
private:
	ComPtr<ID3D11ComputeShader> pComputeShader;
};