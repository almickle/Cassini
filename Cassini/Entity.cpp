#include "Entity.h"
#include <fstream>
#include <iostream>

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