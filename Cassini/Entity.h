#pragma once
#include "Bindables.h"
#include "DataTypes.h"
#include "Graphics.h"
#include "ResourceManager.h"

class Entity
{
public:
	Entity(Graphics& gfx, ResourceManager& manager, string meshPath, string VSPath, string PSPath);
	Entity(Graphics& gfx, ResourceManager& manager, const XMVECTOR& color, string meshPath, string VSPath, string PSPath);
	virtual ~Entity() {};

	void Bind(Graphics& gfx, ResourceManager& manager);
	virtual void Update(Graphics& gfx, ResourceManager& manager) {
		UpdateVSData(gfx, manager, GetTransformation());
		LightBuffer buffer = { gfx.GetLighting(), GetModelColor() };
		UpdatePSData(gfx, manager, buffer);
	};
	void Draw(Graphics& gfx);
	template<typename T>
	string AddInstanceBuffer(Graphics& gfx, ResourceManager& manager, UINT type, const T& cbData);
protected:
	void LoadMesh(string path);
	void UpdateVSData(Graphics& gfx, ResourceManager& manager, const XMMATRIX& cbData);
	template<typename PSData>
	void UpdatePSData(Graphics& gfx, ResourceManager& manager, const PSData& cbData);
private:
	string entityID;
	string instanceID;
	vector<string> resourceIDs;
	vector<Vertex> vertices;
	vector<unsigned short> indices;
	LPWSTR VSPath;
	LPWSTR PSPath;
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
	string GetResourceID(int index) {
		return resourceIDs[index];
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