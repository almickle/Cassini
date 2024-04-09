#pragma once
#include "Shader.h"

class PixelShader : public Shader
{
public:
	PixelShader(Graphics& gfx, string path);
	void Bind(Graphics& gfx) const override;
private:
	ComPtr<ID3D11PixelShader> pPixelShader;
};