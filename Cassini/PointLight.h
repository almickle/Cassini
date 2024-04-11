#pragma once
#include "Entity.h"

class PointLight :
	public Entity
{
public:
	PointLight(Graphics& gfx, ResourceManager& manager);
public:
	PhongLightingData ComputeLightData(Graphics& gfx);
	void UpdateLight(Graphics& gfx);
	void SpawnControlWindow();
public:
	void Update(Graphics& gfx, ResourceManager& manager) override;
public:
	static const string entityID;
private:
	XMFLOAT3 ambient = { 0.5f, 0.5f, 0.5f };
	XMFLOAT3 color = { 1.0f, 1.0f, 1.0f };
	float intensity = 1.0f;
	float attConst = 0.1f;
	float attLin = 0.1f;
	float attQuad = 0.1f;
private:
	float radius = 15.0f;
	float theta = 70.0f;
	float height = 4.0f;
private:
	UINT cbIndex;
private:
	vector<D3D11_INPUT_ELEMENT_DESC> ld = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vertex::Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TextureCoordinate", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vertex::Position) + sizeof(Vertex::Normal), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	MeshData mesh = LoadMesh("Models\\icosphere.obj");
};