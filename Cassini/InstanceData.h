#pragma once
#include "DirectXMath.h"

using namespace DirectX;

enum ShaderDataTypes {
	VERTEX_SHADER_BUFFER,
	COMPUTE_SHADER_BUFFER,
	PIXEL_SHADER_BUFFER
};

enum ResourceTypes {
	VERTEX_BUFFER,
	INDEX_BUFFER,
	INPUT_LAYOUT,
	VERTEX_SHADER,
	PIXEL_SHADER,
	COMPUTE_SHADER,
	PRIMITIVE_TOPOLOGY,
	SAMPLER,
	CONSTANT_BUFFER,
	RASTERIZER,
	TEXTURE
};

struct ModelViewProjection
{
	XMMATRIX model;
	XMMATRIX view;
	XMMATRIX projection;
};

struct PhongLightingData {
	XMFLOAT3 pos;
	XMFLOAT3 color;
	XMFLOAT3 ambient;
	float intensity;
	float att;
};

struct LightBuffer {
	PhongLightingData lighting;
	XMFLOAT3 modelColor;
};

struct TestLightBuffer {
	XMVECTOR a;
	XMVECTOR b;
	XMVECTOR c;
	XMVECTOR d;
};

struct Compute {
	float a;
	float b;
	float c;
	float padding;
};
