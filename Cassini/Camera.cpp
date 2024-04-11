#include "Camera.h"

const string Camera::entityID = "Camera";

Camera::Camera(Graphics& gfx, ResourceManager& manager)
	: Entity(gfx, manager, entityID)
{
	SetPosition({ 10.0f, 10.0f, 10.0f });
	SetTarget({ 0.0f, 0.0f, 0.0f });
	UpdateCamera(gfx);
	if (!manager.IsStaticInitialized(entityID)) {
		manager.CreateVertexShader(gfx, entityID, "PhongVS.cso", ld);
		manager.CreateVertexBuffer(gfx, entityID, mesh.vertices, 0u);
		manager.CreateIndexBuffer(gfx, entityID, mesh.indices);
		manager.CreatePixelShader(gfx, entityID, "PixelShader.cso");
		cbIndex = manager.CreateDynamicConstantBuffer(gfx, entityID, GetTransform(), VERTEX_SHADER_BUFFER, 1u, RecordType::ENTITY);
	}
}

void Camera::Update(Graphics& gfx, ResourceManager& manager)
{
	manager.UpdateConstantBuffer(gfx, entityID, cbIndex, GetTransform());
}

void Camera::UpdateCamera(Graphics& gfx) {
	XMFLOAT3 pos = { target.x + radius * cos(theta * 3.14f / 180.0f), target.y + height, target.z + radius * sin(theta * 3.14f / 180.0f) };
	SetPosition(pos);
	XMMATRIX transform = XMMatrixInverse(nullptr, XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&upDirection)));
	OverrideTransform(transform);
	UpdateViewMatrix(gfx, transform);
}

void Camera::UpdateViewMatrix(Graphics& gfx, const XMMATRIX& transform) {
	gfx.SetCameraTransform(transform);
}

void Camera::SpawnControlWindow()
{
	ImGui::Begin("Camera");
	ImGui::SliderFloat("Orbit radius", &radius, 1.0f, 500.0f);
	ImGui::SliderFloat("Orbit angle", &theta, 0.0f, 1440.0f);
	ImGui::SliderFloat("View height", &height, -500.0f, 500.0f);
	ImGui::End();
}

void Camera::SetTarget(const XMFLOAT3& in_target) {
	target = in_target;
}