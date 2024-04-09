#include "VertexBuffer.h"

template<typename V>
VertexBuffer<V>::VertexBuffer(Graphics& gfx, const vector<V>& vertices, UINT in_slot)
	: Buffer(slot),
	stride(sizeof(V))
{
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = UINT(sizeof(V) * vertices.size());
	bd.StructureByteStride = sizeof(V);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();
	gfx.GetDevice()->CreateBuffer(&bd, &sd, &pBuffer);
}

template<typename V>
void VertexBuffer<V>::Bind(Graphics& gfx) const
{
	gfx.GetContext()->IASetVertexBuffers(slot, 1u, pBuffer.GetAddressOf(), &stride, &offset);
}