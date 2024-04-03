#pragma once
#include "Bindables.h"
#include "DataTypes.h"
#include "Graphics.h"
#include "ResourceManager.h"

class Entity
{
public:
	Entity(Graphics& gfx, ResourceManager& manager, string meshPath, string VSPath, string PSPath, D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	template<typename T>
	string AddInstanceBuffer(Graphics& gfx, ResourceManager& manager, UINT type, T cbData);
	void Draw(Graphics& gfx);
	void Bind(Graphics& gfx, ResourceManager& manager);
	string GetResourceID(int index) {
		return resourceIDs[index];
	}
public:
	void MoveX(float x);
	void MoveY(float y);
	void MoveZ(float z);
	void Rotate(float x, float y, float z);

private:
	void LoadMesh(string path);
	void UpdateVSData(Graphics& gfx, ResourceManager& manager, XMMATRIX cbData);
	void UpdatePSData(Graphics& gfx, ResourceManager& manager, PhongLightingData cbData);

private:
	string entityID;
	string instanceID;
	vector<string> resourceIDs;
	vector<Vertex> vertices;
	vector<unsigned short> indices;
	LPWSTR VSPath;
	LPWSTR PSPath;
	XMFLOAT3 modelColor;

public:
	XMMATRIX GetTransformation() {
		return transformation;
	}
	void CalculateTransformation() {
		transformation = XMMatrixRotationRollPitchYaw(orientation.x, orientation.y, orientation.z) *
			XMMatrixScaling(scale.x, scale.y, scale.z) *
			XMMatrixTranslation(position.x, position.y, position.z);
	}
	XMFLOAT3 GetPosition() {
		return position;
	}
	void SetPosition(XMFLOAT3 in_position) {
		position = in_position;
		CalculateTransformation();
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
	void Update(Graphics& gfx, ResourceManager& manager) {
		UpdateVSData(gfx, manager, GetTransformation());
		//UpdatePSData(gfx, manager, gfx.GetLighting());
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