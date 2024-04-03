#pragma once
#include "DataTypes.h"
#include "GraphicsResource.h"

class VertexBuffer : public GraphicsResource
{
public:
	VertexBuffer(Graphics& gfx, const vector<Vertex>& vertices)
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(Vertex) * vertices.size());
		bd.StructureByteStride = sizeof(Vertex);
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
	UINT stride = sizeof(Vertex);
	UINT offset = 0u;
};