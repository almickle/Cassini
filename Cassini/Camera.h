#pragma once
#include "Entity.h"
#include "InstanceData.h"

class Camera :
	public Entity
{

public:
	Camera(Graphics& gfx, ResourceManager& manager) : Entity(gfx, manager, "Models\\cube.txt", "VertexShader.cso", "PixelShader.cso")
	{
		UpdateCamera(gfx, 0.0f);
	};

	void UpdateCamera(Graphics& gfx, float dt) {
		XMFLOAT3 pos = { radius * cos(dt * 3.14f / 180.0f), 4.0f, radius + radius * sin(dt * 3.14f / 180.0f) };
		XMMATRIX transform = XMMatrixInverse(nullptr, XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&upDirection)));
		OverrideTransform(transform);
		UpdateViewMatrix(gfx, transform);
	}
private:
	void UpdateViewMatrix(Graphics& gfx, XMMATRIX transform);
private:
	string meshPath = "cube.txt";
private:
	const float radius = 10.0f;
	const XMFLOAT3 upDirection = { 0.0f, 1.0f, 0.0f };
	const XMFLOAT3 target = { 0.0f, -1.0f, 10.0f };
};
