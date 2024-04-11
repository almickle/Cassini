#include "Sphere.h"

const string Sphere::entityID = "Sphere";

Sphere::Sphere(Graphics& gfx, ResourceManager& manager, const string& in_entityID, const string& systemID, const float& in_radius, const XMFLOAT3& color, const XMFLOAT3& in_position)
	: Entity(gfx, manager, in_entityID, systemID)
{
	SetPosition(in_position);
	SetModelColor(color);
	SetScale({ in_radius, in_radius, in_radius });
	if (!manager.IsStaticInitialized(in_entityID, systemID))
	{
		manager.CreateVertexShader(gfx, in_entityID, systemID, "InstanceVS.cso", ld);
		manager.CreateVertexBuffer(gfx, in_entityID, systemID, mesh.vertices, 0u);
		manager.CreateIndexBuffer(gfx, in_entityID, systemID, mesh.indices);
		manager.CreatePixelShader(gfx, in_entityID, systemID, "PhongPS.cso");
		manager.CreateStaticConstantBuffer(gfx, in_entityID, systemID, ModelColor{ GetModelColor() }, PIXEL_SHADER_BUFFER, 1u, ENTITY);
	}
}

Sphere::Sphere(Graphics& gfx, ResourceManager& manager, const string& in_entityID)
	: Entity(gfx, manager, in_entityID)
{
	if (!manager.IsStaticInitialized(in_entityID))
	{
		manager.CreateVertexShader(gfx, in_entityID, "PhongVS.cso", ld);
		manager.CreateVertexBuffer(gfx, in_entityID, mesh.vertices, 0u);
		manager.CreateIndexBuffer(gfx, in_entityID, mesh.indices);
		manager.CreatePixelShader(gfx, in_entityID, "PhongPS.cso");
		manager.CreateStaticConstantBuffer(gfx, in_entityID, ModelColor{ GetModelColor() }, PIXEL_SHADER_BUFFER, 1u, RecordType::ENTITY);
	}
}

Sphere::Sphere(Graphics& gfx, ResourceManager& manager, const XMFLOAT3& color, const float& in_radius)
	:Entity(gfx, manager, entityID),
	radius(in_radius)
{
	SetModelColor(color);
	SetScale({ in_radius, in_radius, in_radius });
}

Sphere::Sphere(Graphics& gfx, ResourceManager& manager, const string& in_entityID, const float& in_radius)
	: Entity(gfx, manager, in_entityID),
	radius(in_radius)
{
	SetScale({ in_radius, in_radius, in_radius });
}

void Sphere::Update(Graphics& gfx, ResourceManager& manager)
{

}

float Sphere::GetRadius() const {
	return radius;
}