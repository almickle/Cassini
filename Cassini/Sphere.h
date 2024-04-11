#pragma once
#include "Entity.h"

class Sphere :
	public Entity
{
public:
	Sphere(Graphics& gfx, ResourceManager& manager, const string& in_entityID, const string& systemID, const float& in_radius, const XMFLOAT3& color, const XMFLOAT3& in_position);
	Sphere(Graphics& gfx, ResourceManager& manager, const string& in_entityID);
	Sphere(Graphics& gfx, ResourceManager& manager, const XMFLOAT3& color, const float& in_radius);
	Sphere(Graphics& gfx, ResourceManager& manager, const string& in_entityID, const float& in_radius);
public:
	void Update(Graphics& gfx, ResourceManager& manager) override;
public:
	static const string entityID;
public:
	float GetRadius() const;
private:
	float radius = 1.0f;
private:
	vector<D3D11_INPUT_ELEMENT_DESC> ld = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vertex::Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TextureCoordinate", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vertex::Position) + sizeof(Vertex::Normal), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	MeshData mesh = LoadMesh("Models\\icosphere.obj");
};