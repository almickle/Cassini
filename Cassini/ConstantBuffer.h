#pragma once
#include "GraphicsResource.h"
#include "InstanceData.h"

// Template class for specific constant buffer types
template<typename T>
class ConstantBuffer : public InstanceResource {
public:
	ConstantBuffer(Graphics& gfx, UINT type, const T& cbData)
	{
		INFOMAN(gfx);
		shaderType = type;
		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.ByteWidth = sizeof(T); // Size of the constant buffer structure
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = &cbData;

		GFX_THROW_INFO(gfx.GetDevice()->CreateBuffer(&cbDesc, &sd, pConstBuffer.GetAddressOf()));
	}

	void Bind(Graphics& gfx, UINT slot = 0u) override
	{
		INFOMAN(gfx);
		switch (shaderType) {
		case 0:
			GFX_THROW_INFO_ONLY(gfx.GetContext()->VSSetConstantBuffers(slot, 1u, pConstBuffer.GetAddressOf()));
			break;
		case 1:
			GFX_THROW_INFO_ONLY(gfx.GetContext()->CSSetConstantBuffers(slot, 1u, pConstBuffer.GetAddressOf()));
			break;
		case 2:
			GFX_THROW_INFO_ONLY(gfx.GetContext()->PSSetConstantBuffers(slot, 1u, pConstBuffer.GetAddressOf()));
			break;
		default:
			break;
		}
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
};