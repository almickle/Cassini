#pragma once
#include "Buffer.h"

template<typename T>
class StaticConstantBuffer :
	public Buffer
{
public:
	StaticConstantBuffer(Graphics& gfx, const T& cbData, ShaderDataTypes type, UINT slot)
		: Buffer(slot),
		shaderType(type)
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
private:
	const ShaderDataTypes shaderType;
};

