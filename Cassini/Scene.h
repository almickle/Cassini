#pragma once
#include "ElectricField.h"
#include "Entities.h"
#include "Entity.h"
#include "GDIPlusManager.h"
#include "Graphics.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>
#include <string>

using namespace std::chrono;
using namespace std;

class Scene
{
public:
	Scene(Graphics& gfx, ResourceManager& manager)
	{
		float lower_bound = -90.0f;   // Lower bound of the range
		float upper_bound = 90.0f; // Upper bound of the range
		mt19937 rng{ random_device{}() };
		uniform_real_distribution<float> distribution(lower_bound, upper_bound);
		for (int i = 0; i < 100; i++) {
			float charge = i % 2 == 0 ? 1.0f : -1.0f;
			float radius = 1.0f;
			Particle* ptcl = new Particle(gfx, manager, radius, charge, { distribution(rng), distribution(rng) + 100, distribution(rng) });
			particles.push_back(ptcl);
			entities.push_back(ptcl);
		}
		Particle* ptcla = new Particle(gfx, manager, 1.0f, 0.0f, { 20.0f, 10.0f, 0.0f }, { -10000.0f, 0.0f, 0.0f });
		Particle* ptclb = new Particle(gfx, manager, 1.0f, 0.0f, { -20.0f, 10.0f, 0.0f }, { 10000.0f, 0.0f, 0.0f });
		particles.push_back(ptcla);
		particles.push_back(ptclb);
		entities.push_back(ptcla);
		entities.push_back(ptclb);
		field = new ElectricField(gfx, manager, particles);
		camera = new Camera(gfx, manager);
		light = new PointLight(gfx, manager);
		grid = new Grid(gfx, manager);
		AddEntity(camera);
		AddEntity(light);
		AddEntity(grid);
	};
	void UpdateScene(Graphics& gfx, ImVec2 size, ResourceManager& manager)
	{
		auto start = steady_clock::now();
		gfx.SetProjection(XMMatrixPerspectiveLH(1.0f, size.y / size.x, 0.5f, 1000.0f));
		camera->SpawnControlWindow();
		light->SpawnControlWindow();
		manager.SpawnControlWindow();
		FindCentroid();
		camera->SetTarget(worldCenter);
		camera->UpdateCamera(gfx);
		light->UpdateLight(gfx);
		field->Bind(gfx, manager);
		field->Dispatch(gfx, manager, 100, dt * simulationSpeed / 100);
		for (auto entity : entities)
		{
			entity->Bind(gfx, manager);
			entity->Update(gfx, manager, dt);
			entity->Draw(gfx, manager);
		}
		auto end = steady_clock::now();
		dt = duration<float>(end - start).count();
		frameRate = 1 / dt;
		frameCount++;
		if (frameCount % 10 == 0) {
			displayFR = frameRate;
			displayFT = dt;
		};
		ImGui::Begin("Diagnostics");
		ImGui::Text("Framerate: %.2f fps", displayFR);
		ImGui::Text("Frametime: %.2f ms", displayFT * 1000);
		ImGui::SliderFloat("SimulationSpeed", &simulationSpeed, 0.0f, 1.0f);
		ImGui::End();
	};

	void AddEntity(Entity* entity)
	{
		entities.push_back(entity);
	};

	void FindCentroid() {
		centroid = { 0.0f, 0.0f, 0.0f };
		for (auto& atom : particles) {
			XMFLOAT3 pos = atom->GetPosition();
			XMStoreFloat3(&centroid, (XMLoadFloat3(&centroid) + XMLoadFloat3(&pos)));
		}
		XMStoreFloat3(&centroid, (XMLoadFloat3(&centroid) / particles.size()));
	}

private:
	vector<Entity*> entities;
	Camera* camera;
	PointLight* light;
	Grid* grid;
private:
	ElectricField* field;
	vector<Particle*> particles;
private:
	int frameCount = 0;
	float frameRate = 0;
	float displayFR = 0;
	float displayFT = 0;
	float dt = 0.0f;
private:
	float simulationSpeed = 0.0f;
	XMFLOAT3 worldCenter = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 simulationCenter = { 0.0f, 50.0f, 0.0f };
	XMFLOAT3 centroid;
};
