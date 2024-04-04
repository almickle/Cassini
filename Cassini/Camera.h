#pragma once
#include "Entity.h"
#include "imgui.h"
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
		XMFLOAT3 pos = { radius * cos(theta * 3.14f / 180.0f), height, target.z + radius * sin(theta * 3.14f / 180.0f) };

		XMMATRIX transform = XMMatrixInverse(nullptr, XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&upDirection)));
		OverrideTransform(transform);
		UpdateViewMatrix(gfx, transform);
	}
	void SpawnControlWindow()
	{
		ImGui::Begin("Camera");
		ImGui::SliderFloat("Orbit radius", &radius, 1.0f, 30.0f);
		ImGui::SliderFloat("Orbit angle", &theta, 0.0f, 360.0f);
		ImGui::SliderFloat("View height", &height, -10.0f, 10.0f);
		ImGui::End();
	}
private:
	void UpdateViewMatrix(Graphics& gfx, XMMATRIX transform);
private:
	string meshPath = "cube.txt";
private:
	float radius = 15.0f;
	float theta = 10.0f;
	float height = 4.0f;
	const XMFLOAT3 upDirection = { 0.0f, 1.0f, 0.0f };
	const XMFLOAT3 target = { 0.0f, -1.0f, 10.0f };
};
