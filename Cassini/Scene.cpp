#include "ParticleSystem.h"
#include "Scene.h"

Scene::Scene(Graphics& gfx, ResourceManager& manager, Resolution resolution)
{
	manager.CreateGlobalDynamicConstantBuffer(gfx, ViewProjection{ gfx.GetCameraView(), gfx.GetProjection() }, VERTEX_SHADER_BUFFER, 0u);
	manager.CreateGlobalDynamicConstantBuffer(gfx, PhongLightingData{ gfx.GetLighting() }, PIXEL_SHADER_BUFFER, 0u);
	light = new PointLight(gfx, manager);
	camera = new Camera(gfx, manager);
	//grid = new Grid(gfx, manager);
	system = new ParticleSystem(gfx, manager, 10);
	camera->SetTarget(system->GetParticle(1u)->GetPosition());
	animation = new Animation(gfx, manager, system, camera, light);
}

void Scene::UpdateScene(Graphics& gfx, ImVec2 size, ResourceManager& manager)
{
	ImGui::Begin("Simulation");
	if (!watch)
		if (ImGui::Button("Bake")) {
			animation->Bake(gfx, manager, Resolution{ (UINT)size.x, (UINT)size.y });
			watch = true;
			auto start = chrono::steady_clock::now();
			animation->SetPlaybackStart(start);
		}
	if (watch)
		animation->Watch(gfx, size);

	ImGui::End();
	if (!watch)
		return;
	auto start = steady_clock::now();
	gfx.SetProjection(XMMatrixPerspectiveLH(1.0f, size.y / size.x, 0.5f, 10000.0f));
	manager.SpawnControlWindow();
	system->SpawnControlWindow();
	camera->SpawnControlWindow(system);
	//camera->SetTarget(system->GetBond(0)->GetMove());
	//camera->SetTarget(simulationCenter);
	//camera->SetTarget(worldCenter);
	//camera->SetTarget(system->GetCentroid());
	camera->UpdateCamera(gfx);
	light->UpdateLight(gfx);
	light->SpawnControlWindow();
	manager.BindGlobalBuffers(gfx);
	for (auto& system : manager.GetSystemList())
	{
		manager.BindResources(gfx, system.first, SYSTEM);
		Scene::system->RenderSnapshot(gfx, manager, animation->GetSnapshot());
		for (auto& entity : manager.GetSystemEntityList(system.first))
		{
			manager.BindResources(gfx, system.first, entity.first);
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

void Scene::SpawnControlWindow()
{
	ImGui::Begin("Diagnostics");
	ImGui::Text("Framerate: %.2f fps", displayFR);
	ImGui::Text("Frametime: %.2f ms", displayFT * 1000);
	ImGui::SliderFloat("SimulationSpeed", &simulationSpeed, 0.0f, 1.0f);
	ImGui::End();
}