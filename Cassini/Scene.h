#pragma once
#include "Entities.h"
#include "GDIPlusManager.h"
#include "Graphics.h"
#include "Molecule.h"
#include "ResourceManager.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>
#include <string>

class ParticleSystem;

using namespace std::chrono;
using namespace std;

class Scene
{
public:
	Scene(Graphics& gfx, ResourceManager& manager);
	void UpdateScene(Graphics& gfx, ImVec2 size, ResourceManager& manager);
	//void AddEntity(Entity* entity);
	//void FindCentroid();
	void SpawnControlWindow();
private:
	PointLight* light;
	Camera* camera;
	Grid* grid;
	ParticleSystem* system;
private:
	int frameCount = 0;
	float frameRate = 0.0f;
	float displayFR = 0.0f;
	float displayFT = 0.0f;
	float dt = 0.0f;
private:
	float simulationSpeed = 0.0f;
	XMFLOAT3 worldCenter = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 simulationCenter = { 0.0f, 50.0f, 0.0f };
	XMFLOAT3 centroid = { 0.0f, 50.0f, 0.0f };
};
