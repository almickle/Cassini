#pragma once
#include "DataTypes.h"
#include "GraphicsResource.h"

class InputLayout : public GraphicsResource
{
public:
	InputLayout(Graphics& gfx, ComPtr<ID3DBlob> pBlob)
	{
		INFOMAN(gfx);
		D3D11_INPUT_ELEMENT_DESC ld[] = {
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vertex::Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TextureCoordinate", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vertex::Position) + sizeof(Vertex::Normal), D3D11_INPUT_PER_VERTEX_DATA, 0 } };

		GFX_THROW_INFO(gfx.GetDevice()->CreateInputLayout(ld, size(ld), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), pInputLayout.GetAddressOf()));
	};

	void Bind(Graphics& gfx) const override
	{
		INFOMAN(gfx);
		GFX_THROW_INFO_ONLY(gfx.GetContext()->IASetInputLayout(pInputLayout.Get()));
	};

private:
	ComPtr<ID3D11InputLayout> pInputLayout;
};
