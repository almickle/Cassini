#include "StaticConstantBuffer.h"

template<typename T>
StaticConstantBuffer<T>::StaticConstantBuffer(Graphics& gfx, const T& cbData, UINT in_slot)
	: Buffer(slot)
{
	INFOMAN(gfx);
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth = sizeof(T);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.CPUAccessFlags = 0u;

	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = &cbData;

	GFX_THROW_INFO(gfx.GetDevice()->CreateBuffer(&cbDesc, &sd, pBuffer.GetAddressOf()));
}

template<typename T>
void StaticConstantBuffer<T>::Bind(Graphics& gfx) const
{
	INFOMAN(gfx);
	GFX_THROW_INFO_ONLY(gfx.GetContext()->CSSetConstantBuffers(slot, 1u, pBuffer.GetAddressOf()));
}