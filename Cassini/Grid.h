#pragma once
#include "Entity.h"

class Grid : public Entity
{
	//Entity(Graphics& gfx, ResourceManager& manager, string meshPath, string VSPath, string PSPath, D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

public:
	Grid(Graphics& gfx, ResourceManager& manager) : Entity(gfx, manager, "VertexShader.cso", "PixelShader.cso") {

	}
};

