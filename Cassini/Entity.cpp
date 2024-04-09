#include "Entity.h"
#include "InstanceData.h"
#include "Utility.h"
#include <fstream>
#include <iostream>

using namespace utility;

Entity::Entity(Graphics& gfx, ResourceManager& manager, string in_entityID, string meshPath, string VSPath = "PhongVS.cso", string PSPath = "PhongPS.cso")
	: entityID(in_entityID)
{
	if (!manager.CheckForEntity(entityID)) {
		MeshData meshData = LoadMesh(meshPath);
		manager.RegisterEntity(entityID);
		manager.CreateStaticResources(gfx, entityID, meshData.vertices, meshData.indices, VSPath, PSPath, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		manager.BindStaticResources(gfx, entityID);
	}
	instanceIndex = manager.RegisterInstance(entityID);

	ModelViewProjection buffer = { XMMatrixTranspose(transformation), XMMatrixTranspose(gfx.GetCameraView()), XMMatrixTranspose(gfx.GetProjection()) };
	AddInstanceBuffer(gfx, manager, VERTEX_SHADER_BUFFER, buffer);

	LightBuffer lightBuffer = { gfx.GetLighting(), GetModelColor() };
	AddInstanceBuffer(gfx, manager, PIXEL_SHADER_BUFFER, lightBuffer);
}

Entity::Entity(Graphics& gfx, ResourceManager& manager, string in_entityID, string meshPath)
	: entityID(in_entityID)
{
	if (!manager.CheckForEntity(entityID)) {
		manager.RegisterEntity(entityID);
		MeshData meshData = LoadMesh(meshPath);
		manager.CreateStaticResources(gfx, entityID, meshData.vertices, meshData.indices, "InstanceVS.cso", "InstancePS.cso", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	instanceIndex = manager.RegisterInstance(entityID);
}

void Entity::Draw(Graphics& gfx, ResourceManager& manager)
{
	UINT count = reinterpret_cast<IndexBuffer*>(manager.GetStaticResourceByIndex(entityID, 1))->GetCount();
	gfx.GetContext()->DrawIndexed(count, 0, 0);
};

void Entity::Bind(Graphics& gfx, ResourceManager& manager) {
	manager.BindStaticResources(gfx, entityID);
	manager.BindInstanceResources(gfx, entityID, instanceIndex);
}

template<typename T>
void Entity::AddInstanceBuffer(Graphics& gfx, ResourceManager& manager, UINT type, const T& cbData) {
	manager.CreateConstantBuffer(gfx, entityID, instanceIndex, type, cbData);
}

void Entity::UpdateVSData(Graphics& gfx, ResourceManager& manager, const XMMATRIX& transform)
{
	ModelViewProjection buffer = { XMMatrixTranspose(transform), XMMatrixTranspose(gfx.GetCameraView()), XMMatrixTranspose(gfx.GetProjection()) };
	manager.UpdateConstantData(gfx, entityID, instanceIndex, 0u, buffer);
}

template<typename PSData>
void Entity::UpdatePSData(Graphics& gfx, ResourceManager& manager, const PSData& cbData)
{
	manager.UpdateConstantData(gfx, entityID, instanceIndex, 1u, cbData);
}

void Entity::SetPosition(XMFLOAT3 in_position) {
	position = in_position;
	CalculateTransformation();
}
void Entity::SetModelColor(XMFLOAT3 color) {
	modelColor = color;
}
void Entity::SetOrientation(XMFLOAT3 in_orientation) {
	orientation = in_orientation;
	CalculateTransformation();
}
void Entity::SetScale(XMFLOAT3 in_scale)
{
	scale = in_scale;
	CalculateTransformation();
}

void Entity::CalculateTransformation() {
	transformation = XMMatrixRotationRollPitchYaw(orientation.x, orientation.y, orientation.z) *
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixTranslation(position.x, position.y, position.z);
}

void Entity::OverrideTransform(XMMATRIX transform) {
	transformation = transform;
}

void Entity::Update(Graphics& gfx, ResourceManager& manager, float dt) {
	UpdateModel(dt);
	UpdateVSData(gfx, manager, GetTransformation());
	LightBuffer buffer = { gfx.GetLighting(), GetModelColor() };
	UpdatePSData(gfx, manager, buffer);
};

XMMATRIX Entity::GetTransformation() const {
	return transformation;
}
XMFLOAT3 Entity::GetModelColor() const {
	return modelColor;
}
XMFLOAT3 Entity::GetPosition() const {
	return position;
}

MeshData Entity::LoadMesh(string path)
{
	struct MeshData meshData;

	struct Face
	{
		short vIndex[3]; // Vertex indices
		short nIndex[3]; // Normal indices
		short uvIndex[3]; // UV indices
	};

	struct Position
	{
		float x;
		float y;
		float z;
	};

	struct Normal
	{
		float x;
		float y;
		float z;
	};

	struct TextureCoordinate
	{
		float u;
		float v;
	};

	vector<Position> positions;
	vector<Normal> normals;
	vector<TextureCoordinate> uvs;
	vector<Face> faces;

	ifstream inputFile;
	inputFile.open(path);
	string line;
	while (getline(inputFile, line))
	{
		istringstream iss(line);
		string prefix;
		iss >> prefix;

		if (prefix == "v")
		{
			// Vertex
			Position pos;
			iss >> pos.x >> pos.y >> pos.z;
			positions.push_back(pos);
		}
		else if (prefix == "vt")
		{
			// Texture coordinate
			TextureCoordinate tc;
			iss >> tc.u >> tc.v;
			uvs.push_back(tc);
		}
		else if (prefix == "vn")
		{
			// Normal
			Normal n;
			iss >> n.x >> n.y >> n.z;
			normals.push_back(n);
		}
		else if (prefix == "f")
		{
			// Face
			Face f;
			char slash; // To consume the slash character in the input
			for (int i = 0; i < 3; i++)
			{
				iss >> f.vIndex[i] >> slash >> f.uvIndex[i] >> slash >> f.nIndex[i];
			}
			faces.push_back(f);
		}
	}

	inputFile.close();

	int index = 0;
	for (const Face& face : faces)
	{
		for (int i = 0; i < 3; i++)
		{
			Vertex vertex = {
				{ positions[face.vIndex[i] - 1].x, positions[face.vIndex[i] - 1].y, positions[face.vIndex[i] - 1].z },
				{ normals[face.nIndex[i] - 1].x, normals[face.nIndex[i] - 1].y, normals[face.nIndex[i] - 1].z },
				{ uvs[face.uvIndex[i] - 1].u, uvs[face.uvIndex[i] - 1].v, },
			};
			meshData.vertices.push_back(vertex);
			meshData.indices.push_back(index);
			index++;
		}
	}

	return meshData;
}