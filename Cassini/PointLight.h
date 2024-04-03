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
	PhongLightingData GetLightData();
	void UpdateLight(Graphics& gfx);
private:
	XMFLOAT3 ambient = { 0.5f, 0.5f, 0.5f };
	XMFLOAT3 color = { 1.0f, 1.0f, 1.0f };
	float intensity = 1.0f;
	float attConst = 0.1f;
	float attLin = 0.1f;
	float attQuad = 0.1f;
};