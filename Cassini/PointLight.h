#pragma once
#include "Entity.h"

class PointLight :
	public Entity
{

public:
	PointLight(Graphics& gfx, ResourceManager& manager) : Entity(gfx, manager, "Models\\icosphere.txt", "VertexShader.cso", "PixelShader.cso")
	{
		SetPosition({ 5.0f, 2.0f, 7.0f });
		SetScale({ 0.5f, 0.5f, 0.5f });
		UpdateLight(gfx);
	};
	PhongLightingData ComputeLightData(Graphics& gfx);
	void UpdateLight(Graphics& gfx);
	void SpawnControlWindow()
	{
		ImGui::Begin("Light");
		ImGui::SliderFloat("Orbit radius", &radius, 1.0f, 30.0f);
		ImGui::SliderFloat("Orbit angle", &theta, 0.0f, 360.0f);
		ImGui::SliderFloat("Height", &height, -10.0f, 10.0f);
		ImGui::End();
	}
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
};