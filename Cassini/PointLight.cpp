#include "PointLight.h"

const string PointLight::entityID = "PointLight";

PointLight::PointLight(Graphics& gfx, ResourceManager& manager)
	: Entity(gfx, manager, entityID)
{
	SetPosition({ 5.0f, 2.0f, 7.0f });
	SetModelColor({ 1.0f, 1.0f, 0.5f });
	SetScale({ 0.5f, 0.5f, 0.5f });
	UpdateLight(gfx);
	manager.CreateVertexShader(gfx, entityID, "PhongVS.cso", ld);
	manager.CreateVertexBuffer(gfx, entityID, mesh.vertices, 0u);
	manager.CreateIndexBuffer(gfx, entityID, mesh.indices);
	manager.CreatePixelShader(gfx, entityID, "ColorPS.cso");
	cbIndex = manager.CreateDynamicConstantBuffer(gfx, entityID, GetTransform(), VERTEX_SHADER_BUFFER, 1u, RecordType::ENTITY);
	manager.CreateStaticConstantBuffer(gfx, entityID, ModelColor{ GetModelColor() }, PIXEL_SHADER_BUFFER, 1u, RecordType::ENTITY);
};

void PointLight::Update(Graphics& gfx, ResourceManager& manager)
{
	manager.UpdateConstantBuffer(gfx, entityID, cbIndex, GetTransform());
}


PhongLightingData PointLight::ComputeLightData(Graphics& gfx) {
	XMFLOAT3 in_pos = GetPosition();
	XMFLOAT3 out_pos;
	XMVECTOR transformed = XMVector3Transform(XMLoadFloat3(&in_pos), XMMatrixTranspose(GetTransform()));
	transformed = XMVector3Transform(XMLoadFloat3(&in_pos), XMMatrixTranspose(gfx.GetCameraView()));
	XMStoreFloat3(&out_pos, transformed);
	return {
		out_pos,
		color,
		ambient,
	};
}

void PointLight::UpdateLight(Graphics& gfx) {
	SetPosition({ radius * cos((theta + 180.0f) * 3.14f / 180.0f), height, 10.0f + radius * sin((theta + 180.0f) * 3.14f / 180.0f) });
	gfx.SetLighting(ComputeLightData(gfx));
}

void PointLight::SpawnControlWindow()
{
	ImGui::Begin("Light");
	ImGui::SliderFloat("Orbit radius", &radius, 1.0f, 30.0f);
	ImGui::SliderFloat("Orbit angle", &theta, 0.0f, 360.0f);
	ImGui::SliderFloat("Height", &height, -10.0f, 10.0f);
	ImGui::End();
}