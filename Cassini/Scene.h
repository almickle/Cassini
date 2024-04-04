#pragma once
#include "Entities.h"
#include "Entity.h"
#include "GDIPlusManager.h"
#include "Graphics.h"
#include <algorithm>
#include <chrono>
#include <cmath>

using namespace std::chrono;

class Scene
{
public:
	Scene(Graphics& gfx, ResourceManager& manager)
	{
		camera = new Camera(gfx, manager);
		light = new PointLight(gfx, manager);
		model = new Model(gfx, manager);
		grid = new Grid(gfx, manager);
		//sphere = new Sphere(gfx, manager, { 0.0f, 1.0f, 0.0f });
		AddEntity(camera);
		AddEntity(light);
		AddEntity(model);
		AddEntity(grid);
		//AddEntity(sphere);
	};
	void UpdateScene(Graphics& gfx, ImVec2 size, ResourceManager& manager)
	{
		auto start = steady_clock::now();
		gfx.SetProjection(XMMatrixPerspectiveLH(1.0f, size.y / size.x, 0.5f, 100.0f));
		camera->SpawnControlWindow();
		light->SpawnControlWindow();
		camera->SetTarget(model->GetPosition());
		camera->UpdateCamera(gfx);
		light->UpdateLight(gfx);
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
	Camera* camera;
	Model* model;
	PointLight* light;
	Grid* grid;
	Sphere* sphere;
private:
	float frameCount = 0;
	float frameRate = 0;
	float speedFactor = 100;
	float dt = 0.0f;
	XMFLOAT3 worldCenter = { 0.0f, 0.0f, 0.0f };
};
