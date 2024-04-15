#include "Entity.h"
#include "InstanceData.h"
#include "Utility.h"
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <fstream>
#include <iostream>

using namespace utility;

Entity::Entity(Graphics& gfx, ResourceManager& manager, const string& in_entityID)
	: entityID(in_entityID)
{
	if (!manager.CheckForEntity(entityID))
		manager.RegisterEntity(entityID);

	manager.RegisterInstance(entityID, this);
}

Entity::Entity(Graphics& gfx, ResourceManager& manager, const string& in_entityID, const string& systemID)
	: entityID(in_entityID)
{
	if (!manager.CheckForEntityInSystem(systemID, entityID))
		manager.RegisterEntityToSystem(systemID, entityID);

	manager.RegisterEntityInstanceToSystem(systemID, entityID, this);
}


void Entity::SetPosition(const XMFLOAT3& in_position) {
	position = in_position;
	CalculateTransformation();
}
void Entity::SetModelColor(const XMFLOAT3& color) {
	modelColor = color;
}
void Entity::SetOrientation(const XMFLOAT3& in_rotation) {
	rotation = in_rotation;
	CalculateTransformation();
}
void Entity::SetScale(const XMFLOAT3& in_scale)
{
	scale = in_scale;
	CalculateTransformation();
}

void Entity::CalculateTransformation() {
	transformation = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixTranslation(position.x, position.y, position.z);
}

void Entity::OverrideTransform(const XMMATRIX& transform) {
	transformation = transform;
}

XMMATRIX Entity::GetTransform() const {
	return XMMatrixTranspose(transformation);
}
XMFLOAT3 Entity::GetModelColor() const {
	return modelColor;
}
XMFLOAT3 Entity::GetPosition() const {
	return position;
}
XMFLOAT3 Entity::GetRotation() const
{
	return rotation;
}

MeshData Entity::LoadMesh(const string& path)
{
	struct MeshData meshData;
	Assimp::Importer importer;

	const auto model = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	const auto mesh = model->mMeshes[0];

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		const auto& vertex = mesh->mVertices[i];
		const auto& normal = mesh->mNormals[i];
		const auto& textCoord = mesh->mTextureCoords[0][i];
		meshData.vertices.push_back({
			{ vertex.x, vertex.y, vertex.z },
			{ normal.x, normal.y, normal.z },
			{ textCoord.x, textCoord.y }
			}
		);
	}
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		const auto& face = mesh->mFaces[i];
		meshData.indices.push_back(face.mIndices[0]);
		meshData.indices.push_back(face.mIndices[1]);
		meshData.indices.push_back(face.mIndices[2]);
	}

	//struct Face
	//{
	//	short vIndex[3]; // Vertex indices
	//	short nIndex[3]; // Normal indices
	//	short uvIndex[3]; // UV indices
	//};
	//struct Position
	//{
	//	float x;
	//	float y;
	//	float z;
	//};
	//struct Normal
	//{
	//	float x;
	//	float y;
	//	float z;
	//};
	//struct TextureCoordinate
	//{
	//	float u;
	//	float v;
	//};
	//vector<Position> positions;
	//vector<Normal> normals;
	//vector<TextureCoordinate> uvs;
	//vector<Face> faces;
	//ifstream inputFile;
	//inputFile.open(path);
	//string line;
	//while (getline(inputFile, line))
	//{
	//	istringstream iss(line);
	//	string prefix;
	//	iss >> prefix;
	//	if (prefix == "v")
	//	{
	//		// Vertex
	//		Position pos;
	//		iss >> pos.x >> pos.y >> pos.z;
	//		positions.push_back(pos);
	//	}
	//	else if (prefix == "vt")
	//	{
	//		// Texture coordinate
	//		TextureCoordinate tc;
	//		iss >> tc.u >> tc.v;
	//		uvs.push_back(tc);
	//	}
	//	else if (prefix == "vn")
	//	{
	//		// Normal
	//		Normal n;
	//		iss >> n.x >> n.y >> n.z;
	//		normals.push_back(n);
	//	}
	//	else if (prefix == "f")
	//	{
	//		// Face
	//		Face f;
	//		char slash; // To consume the slash character in the input
	//		for (int i = 0; i < 3; i++)
	//		{
	//			iss >> f.vIndex[i] >> slash >> f.uvIndex[i] >> slash >> f.nIndex[i];
	//		}
	//		faces.push_back(f);
	//	}
	//}
	//inputFile.close();
	//int index = 0;
	//for (const Face& face : faces)
	//{
	//	for (int i = 0; i < 3; i++)
	//	{
	//		Vertex vertex = {
	//			{ positions[face.vIndex[i] - 1].x, positions[face.vIndex[i] - 1].y, positions[face.vIndex[i] - 1].z },
	//			{ normals[face.nIndex[i] - 1].x, normals[face.nIndex[i] - 1].y, normals[face.nIndex[i] - 1].z },
	//			{ uvs[face.uvIndex[i] - 1].u, uvs[face.uvIndex[i] - 1].v, },
	//		};
	//		meshData.vertices.push_back(vertex);
	//		meshData.indices.push_back(index);
	//		index++;
	//	}
	//}

	return meshData;
}