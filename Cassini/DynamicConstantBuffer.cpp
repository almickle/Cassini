#include "DynamicConstantBuffer.h"

template<typename T>
DynamicConstantBuffer<T>::DynamicConstantBuffer(Graphics& gfx, const T& cbData, ShaderDataTypes type, UINT in_slot)
	: Buffer(slot),
	shaderType(type),
{
	INFOMAN(gfx);
D3D11_BUFFER_DESC cbDesc = {};
cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
cbDesc.ByteWidth = sizeof(T);
cbDesc.Usage = D3D11_USAGE_DYNAMIC;
cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

D3D11_SUBRESOURCE_DATA sd = {};
sd.pSysMem = &cbData;

GFX_THROW_INFO(gfx.GetDevice()->CreateBuffer(&cbDesc, &sd, pBuffer.GetAddressOf()));
}

template<typename T>
void DynamicConstantBuffer<T>::Bind(Graphics& gfx) const
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

template<typename T>
void DynamicConstantBuffer<T>::Update(Graphics& gfx, const T& cbData)
{
	INFOMAN(gfx);
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	GFX_THROW_INFO(gfx.GetContext()->Map(pBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedResource));
	memcpy(mappedResource.pData, &cbData, sizeof(T));
	GFX_THROW_INFO_ONLY(gfx.GetContext()->Unmap(pBuffer.Get(), 0u));
}