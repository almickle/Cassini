#pragma once
#include "GraphicsResource.h"

template<typename T>
class StaticConstantBuffer :
	public GraphicsResource
{
public:
	StaticConstantBuffer(Graphics& gfx, UINT in_slot, T& cbData)
		:slot(in_slot)
	{
		INFOMAN(gfx);
		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.ByteWidth = sizeof(T); // Size of the constant buffer structure
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = &cbData;

		GFX_THROW_INFO(gfx.GetDevice()->CreateBuffer(&cbDesc, &sd, pConstBuffer.GetAddressOf()));
	}

	void Bind(Graphics& gfx) const override
	{
		INFOMAN(gfx);
		GFX_THROW_INFO_ONLY(gfx.GetContext()->CSSetConstantBuffers(slot, 1u, pConstBuffer.GetAddressOf()));
	}

	void Update(Graphics& gfx, const T& cbData)
	{
		INFOMAN(gfx);
		// Map the constant buffer
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		GFX_THROW_INFO(gfx.GetContext()->Map(pConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		// Copy data to the constant buffer
		memcpy(mappedResource.pData, &cbData, sizeof(T));

		// Unmap the constant buffer
		GFX_THROW_INFO_ONLY(gfx.GetContext()->Unmap(pConstBuffer.Get(), 0));
	}

private:
	ComPtr<ID3D11Buffer> pConstBuffer;
	UINT slot;
};

