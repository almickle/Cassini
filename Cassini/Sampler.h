#pragma once
#include "GraphicsResource.h"

class Sampler : public GraphicsResource
{
public:
	Sampler(Graphics& gfx) {
		INFOMAN(gfx);

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		GFX_THROW_INFO(gfx.GetDevice()->CreateSamplerState(&samplerDesc, &pSampler));
	}

	void Bind(Graphics& gfx, UINT slot) const override
	{
		gfx.GetContext()->VSSetSamplers(0, 1, pSampler.GetAddressOf());
	}

private:
	ComPtr<ID3D11SamplerState> pSampler;
};
