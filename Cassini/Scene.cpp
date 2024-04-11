#include "ParticleSystem.h"
#include "Scene.h"

Scene::Scene(Graphics& gfx, ResourceManager& manager)
{
	manager.CreateGlobalDynamicConstantBuffer(gfx, ViewProjection{ gfx.GetCameraView(), gfx.GetProjection() }, VERTEX_SHADER_BUFFER, 0u);
	manager.CreateGlobalDynamicConstantBuffer(gfx, PhongLightingData{ gfx.GetLighting() }, PIXEL_SHADER_BUFFER, 0u);
	light = new PointLight(gfx, manager);
	camera = new Camera(gfx, manager);
	grid = new Grid(gfx, manager);
	system = new ParticleSystem(gfx, manager, 10000);
}

void Scene::UpdateScene(Graphics& gfx, ImVec2 size, ResourceManager& manager)
{
	auto start = steady_clock::now();
	gfx.SetProjection(XMMatrixPerspectiveLH(1.0f, size.y / size.x, 0.5f, 1000.0f));
	manager.SpawnControlWindow();
	system->SpawnControlWindow();
	camera->SpawnControlWindow();
	camera->SetTarget(worldCenter);
	camera->UpdateCamera(gfx);
	light->UpdateLight(gfx);
	light->SpawnControlWindow();
	manager.BindGlobalBuffers(gfx);
	for (auto& system : manager.GetSystemList())
	{
		manager.BindResources(gfx, system.first, SYSTEM);
		for (auto& entity : manager.GetSystemEntityList(system.first))
		{
			manager.BindResources(gfx, system.first, entity.first);
			manager.GetSystemInstances(system.first)[0]->Update(gfx, manager);
			manager.DrawSystemEntityInstances(gfx, system.first, entity.first);
		}
	}
	for (auto& entity : manager.GetEntityList())
	{
		manager.BindResources(gfx, entity.first, ENTITY);
		manager.UpdateGlobalBuffer(gfx, 0u, ViewProjection{ gfx.GetCameraView(), gfx.GetProjection() });
		manager.UpdateGlobalBuffer(gfx, 1u, PhongLightingData{ gfx.GetLighting() });
		for (auto& instance : manager.GetInstances(entity.first))
		{
			instance->Update(gfx, manager);
		}
		manager.DrawEntityInstances(gfx, entity.first);
	}
	auto end = steady_clock::now();
	dt = duration<float>(end - start).count();
	frameRate = 1 / dt;
	frameCount++;
	if (frameCount % 10 == 0) {
		displayFR = frameRate;
		displayFT = dt;
	};
	SpawnControlWindow();
}

//void Scene::FindCentroid() {
//	centroid = { 0.0f, 0.0f, 0.0f };
//	for (auto& atom : particles) {
//		XMFLOAT3 pos = atom->GetPosition();
//		XMStoreFloat3(&centroid, (XMLoadFloat3(&centroid) + XMLoadFloat3(&pos)));
//	}
//	XMStoreFloat3(&centroid, (XMLoadFloat3(&centroid) / particles.size()));
//}

void Scene::SpawnControlWindow()
{
	ImGui::Begin("Diagnostics");
	ImGui::Text("Framerate: %.2f fps", displayFR);
	ImGui::Text("Frametime: %.2f ms", displayFT * 1000);
	ImGui::SliderFloat("SimulationSpeed", &simulationSpeed, 0.0f, 1.0f);
	ImGui::End();
}