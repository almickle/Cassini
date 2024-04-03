#include "Entity.h"
#include "InstanceData.h"
#include "Utility.h"
#include <fstream>
#include <iostream>

using namespace utility;

Entity::Entity(Graphics& gfx, ResourceManager& manager, string meshPath, string VSPath, string PSPath, D3D11_PRIMITIVE_TOPOLOGY topology)
{
	LoadMesh(meshPath);
	entityID = GenerateUniqueID();
	instanceID = GenerateUniqueID();
	manager.RegisterEntity(entityID);
	manager.RegisterInstance(entityID, instanceID);
	manager.CreateStaticResources(gfx, entityID, vertices, indices, VSPath, PSPath, topology);
	manager.BindStaticResources(gfx, entityID);

	ModelViewProjection buffer = { XMMatrixTranspose(transformation), XMMatrixTranspose(gfx.GetCameraView()), XMMatrixTranspose(gfx.GetProjection()) };
	AddInstanceBuffer(gfx, manager, VERTEX_SHADER_BUFFER, buffer);
	LightBuffer lightBuffer = { gfx.GetLighting(), modelColor };
	/*TestLightBuffer lightBuffer = {
		{ 7.0f, 7.0f, 7.0f, 7.0f },
		{ 7.0f, 7.0f, 7.0f, 7.0f },
		{ 7.0f, 7.0f, 7.0f, 7.0f },
		{ 7.0f, 7.0f, 7.0f, 7.0f }
	};*/
	AddInstanceBuffer(gfx, manager, PIXEL_SHADER_BUFFER, lightBuffer);
}

void Entity::Draw(Graphics& gfx)
{
	gfx.GetContext()->DrawIndexed(indices.size(), 0, 0);
};

void Entity::Bind(Graphics& gfx, ResourceManager& manager) {
	manager.BindStaticResources(gfx, entityID);
	manager.BindConstantBuffer(gfx, entityID, instanceID, GetResourceID(0));
}

template<typename T>
string Entity::AddInstanceBuffer(Graphics& gfx, ResourceManager& manager, UINT type, const T& cbData) {
	string resourceID = GenerateUniqueID();
	resourceIDs.push_back(resourceID);
	manager.CreateConstantBuffer(gfx, entityID, instanceID, resourceID, type, cbData);
	manager.BindConstantBuffer(gfx, entityID, instanceID, resourceID);
	return resourceID;
}

void Entity::UpdateVSData(Graphics& gfx, ResourceManager& manager, const XMMATRIX& transform)
{
	string resourceID = GetResourceID(0);
	ModelViewProjection buffer = { XMMatrixTranspose(transform), XMMatrixTranspose(gfx.GetCameraView()), XMMatrixTranspose(gfx.GetProjection()) };
	manager.UpdateConstantData(gfx, entityID, instanceID, resourceID, buffer);
}

void Entity::UpdatePSData(Graphics& gfx, ResourceManager& manager, const PhongLightingData& lightData)
{
	string resourceID = GetResourceID(1);
	LightBuffer buffer = { lightData, modelColor };
	manager.UpdateConstantData(gfx, entityID, instanceID, resourceID, buffer);
}

void Entity::MoveX(float x) {
	orientation.x += x;
}

void Entity::MoveY(float y) {
	orientation.y += y;
}

void Entity::MoveZ(float z) {
	orientation.z += z;
}

void Entity::Rotate(float x, float y, float z) {

}

void Entity::LoadMesh(string path)
{
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
			vertices.push_back(vertex);
			indices.push_back(index);
			index++;
		}
	}
}