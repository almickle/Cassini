#pragma once
#include "Entity.h"
#include "ResourceManager.h"

class Grid : public Entity
{
public:
	Grid(Graphics& gfx, ResourceManager& manager);
public:
	void Update(Graphics& gfx, ResourceManager& manager) override;
public:
	static const string entityID;
private:
	vector<D3D11_INPUT_ELEMENT_DESC> ld = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vertex::Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TextureCoordinate", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vertex::Position) + sizeof(Vertex::Normal), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	MeshData mesh = LoadMesh("Models\\plane.obj");
};