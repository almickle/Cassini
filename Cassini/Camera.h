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
		SetPosition({ 0.0f, 0.0f, 1.0f });
		UpdateCamera(gfx);
	};

	void UpdateCamera(Graphics& gfx) {
		XMFLOAT3 pos = { target.x + radius * cos(theta * 3.14f / 180.0f), target.y + height, target.z + radius * sin(theta * 3.14f / 180.0f) };
		SetPosition(pos);
		XMMATRIX transform = XMMatrixInverse(nullptr, XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&upDirection)));
		OverrideTransform(transform);
		UpdateViewMatrix(gfx, transform);
	}
	void SpawnControlWindow()
	{
		ImGui::Begin("Camera");
		ImGui::SliderFloat("Orbit radius", &radius, 1.0f, 500.0f);
		ImGui::SliderFloat("Orbit angle", &theta, 0.0f, 1440.0f);
		ImGui::SliderFloat("View height", &height, -500.0f, 500.0f);
		ImGui::End();
	}
	void SetTarget(XMFLOAT3 in_target) {
		target = in_target;
	}
private:
	void UpdateViewMatrix(Graphics& gfx, XMMATRIX transform);
private:
	string meshPath = "cube.txt";
private:
	float radius = 20.0f;
	float theta = 270.0f;
	float height = 25.0f;
	const XMFLOAT3 upDirection = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 target = { 0.0f, 0.0f, 0.0f };
};
