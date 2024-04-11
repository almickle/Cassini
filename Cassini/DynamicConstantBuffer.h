#pragma once
#include "Buffer.h"
#include "InstanceData.h"

template<typename T>
class DynamicConstantBuffer : public Buffer {
public:
	DynamicConstantBuffer(Graphics& gfx, const T& data, ShaderDataTypes type, UINT slot)
		: Buffer(slot),
		shaderType(type)
	{
		INFOMAN(gfx);
		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.ByteWidth = sizeof(T);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = &data;

		GFX_THROW_INFO(gfx.GetDevice()->CreateBuffer(&cbDesc, &sd, pBuffer.GetAddressOf()));
	}
	void Bind(Graphics& gfx) const override
	{
		INFOMAN(gfx);
		switch (shaderType) {
		case VERTEX_SHADER_BUFFER:
			GFX_THROW_INFO_ONLY(gfx.GetContext()->VSSetConstantBuffers(slot, 1u, pBuffer.GetAddressOf()));
			break;
		case COMPUTE_SHADER_BUFFER:
			GFX_THROW_INFO_ONLY(gfx.GetContext()->CSSetConstantBuffers(slot, 1u, pBuffer.GetAddressOf()));
			break;
		case PIXEL_SHADER_BUFFER:
			GFX_THROW_INFO_ONLY(gfx.GetContext()->PSSetConstantBuffers(slot, 1u, pBuffer.GetAddressOf()));
			break;
		default:
			break;
		}
	}
	void Update(Graphics& gfx, const T& data)
	{
		INFOMAN(gfx);
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		GFX_THROW_INFO(gfx.GetContext()->Map(pBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedResource));
		memcpy(mappedResource.pData, &data, sizeof(T));
		GFX_THROW_INFO_ONLY(gfx.GetContext()->Unmap(pBuffer.Get(), 0u));
	}
private:
	const ShaderDataTypes shaderType;
};