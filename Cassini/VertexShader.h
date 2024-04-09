#pragma once
#include "Shader.h"

class VertexShader : public Shader
{
public:
	VertexShader(Graphics& gfx, const string& path, const vector<D3D11_INPUT_ELEMENT_DESC>& layoutDesc);
	void Bind(Graphics& gfx) const override;
	void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY type);
private:
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3D11InputLayout> pInputLayout;
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};