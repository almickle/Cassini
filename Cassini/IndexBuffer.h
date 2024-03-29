#pragma once
#include "Graphics.h"

class IndexBuffer
{
public:
	IndexBuffer(Graphics& gfx, vector<unsigned short>& indices)
		: count((UINT)indices.size())
	{
		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = UINT(count * sizeof(unsigned short));
		ibd.StructureByteStride = sizeof(unsigned short);
		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices.data();
		gfx.GetDevice()->CreateBuffer(&ibd, &isd, &pIndexBuffer);
	}

	void Bind(Graphics& gfx)
	{
		gfx.GetContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}

	UINT GetCount() { return count; };

private:
	UINT count;
	ComPtr<ID3D11Buffer> pIndexBuffer;
};