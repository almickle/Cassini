#include "Cylinder.h"

const string Cylinder::entityID = "Cylinder";

Cylinder::Cylinder(Graphics& gfx, ResourceManager& manager, const string& systemID, const string& in_entityID)
	: Entity(gfx, manager, in_entityID, systemID)
{
	if (!manager.IsStaticInitialized(in_entityID, systemID))
	{
		mesh = LoadMesh("Models\\cylinder.obj");
		manager.CreateVertexShader(gfx, in_entityID, systemID, "InstanceVS.cso", ld);
		manager.CreateVertexBuffer(gfx, in_entityID, systemID, mesh.vertices, 0u);
		manager.CreateIndexBuffer(gfx, in_entityID, systemID, mesh.indices);
		manager.CreatePixelShader(gfx, in_entityID, systemID, "InstancePS.cso");
	}
}