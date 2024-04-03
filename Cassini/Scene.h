#pragma once
#include "Camera.h"
#include "Entity.h"
#include "GDIPlusManager.h"
#include "Graphics.h"
#include "Model.h"
#include "PointLight.h"
#include <algorithm>
#include <chrono>
#include <cmath>

using namespace std::chrono;

class Scene
{
public:
	Scene(Graphics& gfx, ResourceManager& manager)
	{
		AddEntity(new Camera(gfx, manager));
		AddEntity(new PointLight(gfx, manager));
		AddEntity(new Model(gfx, manager));
	};
	void UpdateScene(Graphics& gfx, ImVec2 size, ResourceManager& manager)
	{
		auto start = steady_clock::now();
		gfx.SetProjection(XMMatrixPerspectiveLH(1.0f, size.y / size.x, 0.5f, 100.0f));
		reinterpret_cast<Camera*>(entities[0])->UpdateCamera(gfx, frameCount);
		for (auto entity : entities)
		{
			entity->Bind(gfx, manager);
			entity->Update(gfx, manager);
			entity->Draw(gfx);
		}
		auto end = steady_clock::now();
		dt = duration<float>(end - start).count();
		frameRate = 60 / dt;
		frameCount++;
	};

	void AddEntity(Entity* entity)
	{
		entities.push_back(entity);
	};

private:
	vector<Entity*> entities;
private:
	float frameCount = 0;
	float frameRate = 0;
	float speedFactor = 100;
	float dt = 0.0f;
	XMFLOAT3 worldCenter = { 0.0f, 0.0f, 0.0f };
};
