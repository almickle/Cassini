#pragma once
#include "Entity.h"
#include "imgui.h"
#include "InstanceData.h"

class ParticleSystem;

class Camera :
	public Entity
{
public:
	Camera(Graphics& gfx, ResourceManager& manager);
public:
	void Update(Graphics& gfx, ResourceManager& manager) override;
public:
	void UpdateCamera(Graphics& gfx);
	void SetTarget(const XMFLOAT3& in_target);
	void SpawnControlWindow(ParticleSystem* system);
public:
	static const string entityID;
private:
	void UpdateViewMatrix(Graphics& gfx, const XMMATRIX& transform);
private:
	float radius = 2000.0f;
	float theta = 270.0f;
	float height = 200.0f;
	XMFLOAT3 upDirection = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 target = { 0.0f, 0.0f, 0.0f };
	int targetIndex = 0;
private:
	UINT cbIndex;
private:
	vector<D3D11_INPUT_ELEMENT_DESC> ld = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vertex::Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TextureCoordinate", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vertex::Position) + sizeof(Vertex::Normal), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	MeshData mesh = LoadMesh("Models\\cube.obj");
};