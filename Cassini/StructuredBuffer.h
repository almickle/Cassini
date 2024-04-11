//#pragma once
//#include "GraphicsResource.h"
//
//template<typename Data>
//class StructuredBuffer :
//	public GraphicsResource
//{
//public:
//	StructuredBuffer(Graphics& gfx, const vector<Data>& data, UINT in_slot, bool in_input = false)
//		: slot(in_slot), initialSize(data.size()), input(in_input), numElements(data.size()), size(sizeof(Data)* data.size())
//	{
//		INFOMAN(gfx);
//
//		if (input) {
//			D3D11_BUFFER_DESC bufferDesc = {};
//			bufferDesc.ByteWidth = sizeof(Data) * data.size();
//			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//			bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//			bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
//			bufferDesc.StructureByteStride = sizeof(Data);
//
//			D3D11_SUBRESOURCE_DATA sd = {};
//			sd.pSysMem = data.data();
//
//			GFX_THROW_INFO(gfx.GetDevice()->CreateBuffer(&bufferDesc, &sd, pBuffer.GetAddressOf()));
//
//			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
//			srvDesc.Format = DXGI_FORMAT_UNKNOWN; // Use DXGI_FORMAT_UNKNOWN for structured buffer
//			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
//			srvDesc.Buffer.FirstElement = 0;
//			srvDesc.Buffer.NumElements = data.size();
//
//			GFX_THROW_INFO(gfx.GetDevice()->CreateShaderResourceView(pBuffer.Get(), &srvDesc, pSRV.GetAddressOf()));
//		}
//		else {
//			D3D11_BUFFER_DESC bufferDesc = {};
//			bufferDesc.ByteWidth = sizeof(Data) * data.size();
//			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
//			bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
//			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
//			bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
//			bufferDesc.StructureByteStride = sizeof(Data);
//
//			D3D11_SUBRESOURCE_DATA sd = {};
//			sd.pSysMem = data.data();
//
//			GFX_THROW_INFO(gfx.GetDevice()->CreateBuffer(&bufferDesc, &sd, pBuffer.GetAddressOf()));
//
//			// Create the unordered access view (UAV) to write to the buffer
//			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
//			uavDesc.Format = DXGI_FORMAT_UNKNOWN; // Use DXGI_FORMAT_UNKNOWN for structured buffer
//			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
//			uavDesc.Buffer.FirstElement = 0;
//			uavDesc.Buffer.NumElements = data.size();
//			uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
//
//			GFX_THROW_INFO(gfx.GetDevice()->CreateUnorderedAccessView(pBuffer.Get(), &uavDesc, pUAV.GetAddressOf()));
//		}
//	}
//
//	void Bind(Graphics& gfx) const override
//	{
//		INFOMAN(gfx);
//		if (input) {
//			GFX_THROW_INFO_ONLY(gfx.GetContext()->CSSetShaderResources(slot, 1u, pSRV.GetAddressOf()));
//		}
//		else {
//			GFX_THROW_INFO_ONLY(gfx.GetContext()->CSSetUnorderedAccessViews(slot, 1u, pUAV.GetAddressOf(), nullptr));
//		}
//	};
//
//	vector<Data> ReadFromBuffer(Graphics& gfx) {
//		if (input) {
//			throw("Attempting to read from input buffer");
//		}
//		INFOMAN(gfx);
//		D3D11_MAPPED_SUBRESOURCE mappedResource;
//		GFX_THROW_INFO_ONLY(gfx.GetContext()->Map(pBuffer.Get(), 0, D3D11_MAP_READ, 0, &mappedResource));
//
//		// Create a vector to hold the data
//		vector<Data> data(numElements);
//
//		// Copy the data from the mapped resource to the vector
//		memcpy(data.data(), mappedResource.pData, size);
//
//		gfx.GetContext()->Unmap(pBuffer.Get(), 0);
//
//		return data;
//	}
//
//	void WriteToBuffer(Graphics& gfx, const vector<Data>& data) {
//		if (!input) {
//			throw("Attempting to write to an output buffer");
//		}
//		INFOMAN(gfx);
//		D3D11_MAPPED_SUBRESOURCE mappedResource;
//		GFX_THROW_INFO_ONLY(gfx.GetContext()->Map(pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
//
//		// Copy the data from the mapped resource to the vector
//		memcpy(mappedResource.pData, data.data(), size);
//
//		gfx.GetContext()->Unmap(pBuffer.Get(), 0);
//	}
//
//private:
//	ComPtr<ID3D11Buffer> pBuffer;
//	ComPtr<ID3D11UnorderedAccessView> pUAV;
//	ComPtr<ID3D11ShaderResourceView> pSRV;
//	unsigned int size;
//	unsigned int numElements;
//	UINT slot;
//	bool input;
//	const UINT initialSize;
//};
//
