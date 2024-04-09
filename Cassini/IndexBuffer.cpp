#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Graphics& gfx, const vector<unsigned short>& indices)
	: count(indices.size())
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
	gfx.GetDevice()->CreateBuffer(&ibd, &isd, &pBuffer);
}

void IndexBuffer::Bind(Graphics& gfx) const
{
	gfx.GetContext()->IASetIndexBuffer(pBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const
{
	return count;
};
