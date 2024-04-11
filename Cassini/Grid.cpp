#include "Grid.h"

const string Grid::entityID = "Grid";

Grid::Grid(Graphics& gfx, ResourceManager& manager)
	: Entity(gfx, manager, entityID)
{
	SetScale({ 100.0f, 1.0f, 100.0f });
	if (!manager.IsStaticInitialized(entityID)) {
		manager.CreateVertexShader(gfx, entityID, "PhongVS.cso", ld);
		manager.CreateVertexBuffer(gfx, entityID, mesh.vertices, 0u);
		manager.CreateIndexBuffer(gfx, entityID, mesh.indices);
		manager.CreatePixelShader(gfx, entityID, "GridPS.cso");
		manager.CreateStaticConstantBuffer(gfx, entityID, GetTransform(), VERTEX_SHADER_BUFFER, 1u, RecordType::ENTITY);
	}
}

void Grid::Update(Graphics& gfx, ResourceManager& manager)
{

}