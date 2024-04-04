#pragma once
#include "DataTypes.h"
#include "GraphicsResource.h"

template<typename V>
class VertexBuffer : public GraphicsResource
{
public:
	VertexBuffer(Graphics& gfx, const vector<V>& vertices)
		: stride(sizeof(V))
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
		gfx.GetDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer);
	};

	void Bind(Graphics& gfx) const override
	{
		gfx.GetContext()->IASetVertexBuffers(
			0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	};

private:
	ComPtr<ID3D11Buffer> pVertexBuffer;
	UINT stride;
	UINT offset = 0u;
};