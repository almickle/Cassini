#pragma once
#include "GraphicsResource.h"

class Topology : public GraphicsResource
{
public:
	Topology(D3D11_PRIMITIVE_TOPOLOGY in_type)
	{
		type = in_type;
	};

	void Bind(Graphics& gfx) const override
	{
		gfx.GetContext()->IASetPrimitiveTopology(type);
	}

private:
	D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};
