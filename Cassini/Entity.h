#pragma once
#include "Bindables.h"
#include "DataTypes.h"
#include "Graphics.h"
#include "ResourceManager.h"

class Entity
{
public:
	Entity(Graphics& gfx, ResourceManager& manager, string entityID, string meshPath, string VSPath, string PSPath);
	virtual ~Entity() {};

	void Bind(Graphics& gfx, ResourceManager& manager);
	virtual void UpdateModel(float dt) {};
	virtual void Update(Graphics& gfx, ResourceManager& manager, float dt) {
		UpdateModel(dt);
		UpdateVSData(gfx, manager, GetTransformation());
		LightBuffer buffer = { gfx.GetLighting(), GetModelColor() };
		UpdatePSData(gfx, manager, buffer);
	};
	void Draw(Graphics& gfx, ResourceManager& manager);
	template<typename T>
	void AddInstanceBuffer(Graphics& gfx, ResourceManager& manager, UINT type, const T& cbData);
protected:
	MeshData LoadMesh(string path);
	void UpdateVSData(Graphics& gfx, ResourceManager& manager, const XMMATRIX& cbData);
	template<typename PSData>
	void UpdatePSData(Graphics& gfx, ResourceManager& manager, const PSData& cbData);
	void UpdatePSData(Graphics& gfx, ResourceManager& manager, const XMFLOAT3& cbData);
private:
	const string entityID;
private:
	UINT instanceIndex;
	XMFLOAT3 modelColor = { 0.3f, 0.3f, 0.35f };
public:
	XMMATRIX GetTransformation() {
		return transformation;
	}
	XMFLOAT3 GetModelColor() {
		return modelColor;
	}
	XMFLOAT3 GetPosition() {
		return position;
	}
public:
	void SetPosition(XMFLOAT3 in_position) {
		position = in_position;
		CalculateTransformation();
	}
	void SetModelColor(XMFLOAT3 color) {
		modelColor = color;
	}
	void SetOrientation(XMFLOAT3 in_orientation) {
		orientation = in_orientation;
		CalculateTransformation();
	}
	void SetScale(XMFLOAT3 in_scale)
	{
		scale = in_scale;
		CalculateTransformation();
	}
public:
	void CalculateTransformation() {
		transformation = XMMatrixRotationRollPitchYaw(orientation.x, orientation.y, orientation.z) *
			XMMatrixScaling(scale.x, scale.y, scale.z) *
			XMMatrixTranslation(position.x, position.y, position.z);
	}
	void OverrideTransform(XMMATRIX transform) {
		transformation = transform;
	}
private:
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 orientation = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
	XMMATRIX transformation =
		XMMatrixRotationRollPitchYaw(orientation.x, orientation.y, orientation.z) *
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixTranslation(position.x, position.y, position.z);
};