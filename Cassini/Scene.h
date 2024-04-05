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
		float lower_bound = 30.0f;   // Lower bound of the range
		float upper_bound = 70.0f; // Upper bound of the range
		mt19937 rng{ random_device{}() };
		uniform_real_distribution<float> distribution(lower_bound, upper_bound);
		for (int i = 0; i < 6; i++) {
			XMFLOAT3 color = i % 2 == 0 ? XMFLOAT3{ 0.1f, 0.1f, 1.0f } : XMFLOAT3{ 1.0f, 0.1f, 0.1f };
			float charge = i % 2 == 0 ? 1.0f : -1.0f;
			float radius = 1.0f;
			Atom* ptcl = new Atom(gfx, manager, color, radius, charge, { distribution(rng), distribution(rng), distribution(rng) });
			atoms.push_back(ptcl);
			entities.push_back(ptcl);
			XMFLOAT3 pos = ptcl->GetPosition();
			XMStoreFloat3(&centroid, (XMLoadFloat3(&centroid) + XMLoadFloat3(&pos)) / (i + 1));
		}
		field = new ElectricField(gfx, manager, atoms);
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
		for (auto& atom : atoms) {
			XMFLOAT3 pos = atom->GetPosition();
			XMStoreFloat3(&centroid, (XMLoadFloat3(&centroid) + XMLoadFloat3(&pos)));
		}
		XMStoreFloat3(&centroid, (XMLoadFloat3(&centroid) / atoms.size()));
		camera->SetTarget(centroid);
		camera->UpdateCamera(gfx);
		light->UpdateLight(gfx);
		field->Bind(gfx, manager);
		field->Dispatch(gfx, manager, 10);
		for (auto entity : entities)
		{
			entity->Bind(gfx, manager);
			entity->Update(gfx, manager, dt);
			entity->Draw(gfx);
		}
		auto end = steady_clock::now();
		dt = duration<float>(end - start).count();
		frameRate = 60 / dt / 1000;
		frameCount++;
		if (frameCount % 10 == 0) {
			displayFR = frameRate;
		};
		ImGui::Begin("Diagnostics");
		ImGui::Text("Framerate: %.2f fps", displayFR);
		ImGui::End();
	};

	void AddEntity(Entity* entity)
	{
		entities.push_back(entity);
	};

private:
	vector<Entity*> entities;
	Camera* camera;
	PointLight* light;
	Grid* grid;
private:
	ElectricField* field;
	vector<Atom*> atoms;
private:
	int frameCount = 0;
	float frameRate = 0;
	float displayFR = 0;
	float speedFactor = 100;
	float dt = 0.0f;
	XMFLOAT3 worldCenter = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 centroid;
};
