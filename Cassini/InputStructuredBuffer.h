#pragma once
#include "Buffer.h"

template<typename T>
class InputStructuredBuffer :
	public Buffer
{
public:
	InputStructuredBuffer(Graphics& gfx, const vector<T>& data, ShaderDataTypes type, UINT slot)
		: Buffer(slot),
		shaderType(type),
		numElements(data.size())
	{
		INFOMAN(gfx);
		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = sizeof(T) * data.size();
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.StructureByteStride = sizeof(T);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = data.data();

		GFX_THROW_INFO(gfx.GetDevice()->CreateBuffer(&bd, &sd, pBuffer.GetAddressOf()));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN; // Use DXGI_FORMAT_UNKNOWN for structured buffer
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = data.size();

		GFX_THROW_INFO(gfx.GetDevice()->CreateShaderResourceView(pBuffer.Get(), &srvDesc, pSRV.GetAddressOf()));
	}
	void Bind(Graphics& gfx) const override
	{
		INFOMAN(gfx);
		switch (shaderType) {
		case VERTEX_SHADER_BUFFER:
			GFX_THROW_INFO_ONLY(gfx.GetContext()->VSSetShaderResources(slot, 1u, pSRV.GetAddressOf()));
			break;
		case COMPUTE_SHADER_BUFFER:
			GFX_THROW_INFO_ONLY(gfx.GetContext()->CSSetShaderResources(slot, 1u, pSRV.GetAddressOf()));
			break;
		case PIXEL_SHADER_BUFFER:
			GFX_THROW_INFO_ONLY(gfx.GetContext()->PSSetShaderResources(slot, 1u, pSRV.GetAddressOf()));
			break;
		default:
			break;
		}
	}
	void WriteToBuffer(Graphics& gfx, const vector<T>& data)
	{
		INFOMAN(gfx);
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		GFX_THROW_INFO_ONLY(gfx.GetContext()->Map(pBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedResource));
		memcpy(mappedResource.pData, data.data(), numElements * sizeof(T));
		gfx.GetContext()->Unmap(pBuffer.Get(), 0);
	}
private:
	ComPtr<ID3D11ShaderResourceView> pSRV;
	const ShaderDataTypes shaderType;
	const UINT numElements;
};

