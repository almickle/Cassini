#include "OutputStructuredBuffer.h"

template<typename T>
OutputStructuredBuffer<T>::OutputStructuredBuffer(Graphics& gfx, const vector<T>& data, UINT in_slot)
	: Buffer(slot),
	numElements(data.size())
{
	INFOMAN(gfx);
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(T) * data.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.StructureByteStride = sizeof(T);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = data.data();

	GFX_THROW_INFO(gfx.GetDevice()->CreateBuffer(&bd, &sd, pBuffer.GetAddressOf()));

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN; // Use DXGI_FORMAT_UNKNOWN for structured buffer
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = data.size();

	GFX_THROW_INFO(gfx.GetDevice()->CreateUnorderedAccessView(pBuffer.Get(), &uavDesc, pUAV.GetAddressOf()));
}

template<typename T>
void OutputStructuredBuffer<T>::Bind(Graphics& gfx) const
{
	INFOMAN(gfx);
	GFX_THROW_INFO_ONLY(gfx.GetContext()->CSSetUnorderedAccessViews(slot, 1u, pUAV.GetAddressOf(), nullptr));
}

template<typename T>
vector<T>OutputStructuredBuffer<T>::ReadFromBuffer(Graphics& gfx) const
{
	INFOMAN(gfx);
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	GFX_THROW_INFO_ONLY(gfx.GetContext()->Map(pBuffer.Get(), 0, D3D11_MAP_READ, 0, &mappedResource));
	vector<T> data(numElements);
	memcpy(data.data(), mappedResource.pData, numElements * sizeof(T));
	gfx.GetContext()->Unmap(pBuffer.Get(), 0);
	return data;
}