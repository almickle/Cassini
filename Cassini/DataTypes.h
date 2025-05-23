#pragma once
#include "Windows.h"
#include <vector>

using namespace std;

enum ShaderDataTypes {
	VERTEX_SHADER_BUFFER = 0,
	COMPUTE_SHADER_BUFFER,
	PIXEL_SHADER_BUFFER
};

struct Vertex
{
	struct Position
	{
		float x;
		float y;
		float z;
	} position;
	struct Normal
	{
		float x;
		float y;
		float z;
	} normal;
	struct TextureCoordinate
	{
		float u;
		float v;
	} textureCoordinate;
};

struct MeshData {
	vector<Vertex> vertices;
	vector<unsigned short> indices;
};

struct Vec3 {
	float x;
	float y;
	float z;
};

struct Vec2 {
	float x;
	float y;
};

struct Resolution {
	UINT width;
	UINT height;
};

struct SimulationBoundary {
	float xMin;
	float xMax;
	float yMin;
	float yMax;
	float zMin;
	float zMax;
};