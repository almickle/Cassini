#pragma once
#include "DirectXMath.h"
#include <vector>

using namespace DirectX;

struct ModelViewProjection
{
	XMMATRIX model;
	XMMATRIX view;
	XMMATRIX projection;
};

struct ViewProjection
{
	XMMATRIX view;
	XMMATRIX projection;
};

struct ModelColor {
	alignas(16) XMFLOAT3 color;
};

struct PhongLightingData {
	alignas(16) XMFLOAT3 pos;
	alignas(16) XMFLOAT3 color;
	alignas(16) XMFLOAT3 ambient;
};



struct ParticleData {
	XMFLOAT3 s;
	XMFLOAT3 v;
};

struct IntrinsicParticleData {
	float mass;
	float charge;
	float radius;
};

struct BondData {
	float bondEnergy;
	float length;
	unsigned short ptcla;
	unsigned short ptclb;
};

struct alignas(16) StaticSimulationData {
	alignas(16) XMFLOAT3 minBoundary;
	alignas(16) XMFLOAT3 maxBoundary;
	unsigned int size;
};

struct alignas(16) DynamicSimulationData {
	float dt;
};
