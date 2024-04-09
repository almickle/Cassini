#pragma once
#include "Bindables.h"
#include "DataTypes.h"
#include "Graphics.h"
#include "ResourceManager.h"

class Entity
{
public:
	Entity(Graphics& gfx, ResourceManager& manager, string entityID, string meshPath, string VSPath, string PSPath);
	Entity(Graphics& gfx, ResourceManager& manager, string entityID, string meshPath);
	virtual ~Entity() {};
public:
	virtual void UpdateModel(float dt) {};
	virtual void Update(Graphics& gfx, ResourceManager& manager, float dt);
public:
	void Bind(Graphics& gfx, ResourceManager& manager);
	void Draw(Graphics& gfx, ResourceManager& manager);
public:
	template<typename T>
	void AddInstanceBuffer(Graphics& gfx, ResourceManager& manager, UINT type, const T& cbData);
protected:
	MeshData LoadMesh(string path);
	void UpdateVSData(Graphics& gfx, ResourceManager& manager, const XMMATRIX& cbData);
	template<typename PSData>
	void UpdatePSData(Graphics& gfx, ResourceManager& manager, const PSData& cbData);
public:
	XMMATRIX GetTransformation() const;
	XMFLOAT3 GetModelColor() const;
	XMFLOAT3 GetPosition() const;
public:
	void SetPosition(XMFLOAT3 in_position);
	void SetModelColor(XMFLOAT3 color);
	void SetOrientation(XMFLOAT3 in_orientation);
	void SetScale(XMFLOAT3 in_scale);
public:
	void CalculateTransformation();
	void OverrideTransform(XMMATRIX transform);
private:
	const string entityID;
	UINT instanceIndex;
private:
	XMFLOAT3 modelColor = { 0.3f, 0.3f, 0.35f };
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 orientation = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
	XMMATRIX transformation =
		XMMatrixRotationRollPitchYaw(orientation.x, orientation.y, orientation.z) *
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixTranslation(position.x, position.y, position.z);
};