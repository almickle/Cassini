#pragma once
#include "Bindables.h"
#include "DataTypes.h"
#include "Graphics.h"
#include "ResourceManager.h"

class Entity
{
protected:
	Entity(Graphics& gfx, ResourceManager& manager, const string& entityID);
	Entity(Graphics& gfx, ResourceManager& manager, const string& entityID, const string& systemID);
	virtual ~Entity() {};
public:
	virtual void Update(Graphics& gfx, ResourceManager& manager) = 0;
public:
	XMMATRIX GetTransform() const;
	XMFLOAT3 GetModelColor() const;
	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetRotation() const;
public:
	void SetPosition(const XMFLOAT3& in_position);
	void SetModelColor(const XMFLOAT3& color);
	void SetOrientation(const XMFLOAT3& in_rotation);
	void SetScale(const XMFLOAT3& in_scale);
	void OverrideTransform(const XMMATRIX& transform);
public:
	void CalculateTransformation();
private:
	const string entityID;
private:
	XMFLOAT3 modelColor = { 0.3f, 0.3f, 0.35f };
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
	XMMATRIX transformation =
		XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixTranslation(position.x, position.y, position.z);
protected:
	MeshData LoadMesh(const string& path);
};