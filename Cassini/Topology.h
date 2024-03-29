#pragma once
#include "Graphics.h"

class Topology
{
public:
	Topology() { };
	Topology(D3D11_PRIMITIVE_TOPOLOGY in_type)
	{
		type = in_type;
	};

	void Bind(Graphics& gfx)
	{
		gfx.GetContext()->IASetPrimitiveTopology(type);
	}

private:
	D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};
