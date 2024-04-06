#pragma once
#include "DirectXMath.h"
#include <vector>

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

struct alignas(16) PhongLightingData {
	alignas(16) XMFLOAT3 pos;
	alignas(16) XMFLOAT3 color;
	alignas(16) XMFLOAT3 ambient;
};

struct LightBuffer {
	alignas(16) PhongLightingData lighting;
	alignas(16) XMFLOAT3 modelColor;
};

struct ParticleData {
	XMFLOAT3 x;
	XMFLOAT3 v;
};
