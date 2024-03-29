#pragma once
#include "Graphics.h"

class Rasterizer
{
public:
	Rasterizer(Graphics& gfx) {
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = D3D11_FILL_SOLID; // Specify the fill mode (solid, wireframe, etc.)
		rasterizerDesc.CullMode = D3D11_CULL_BACK;  // Specify back-face culling
		rasterizerDesc.FrontCounterClockwise = false; // Set to true for counter-clockwise (CCW) winding order
		rasterizerDesc.DepthClipEnable = true; // Enable depth clipping
		gfx.GetDevice()->CreateRasterizerState(&rasterizerDesc, pRasterizer.GetAddressOf());
	}

	void Bind(Graphics& gfx) {
		gfx.GetContext()->RSSetState(pRasterizer.Get());
	}

private:
	ComPtr<ID3D11RasterizerState> pRasterizer;

};

