#include "Scene.h"

Scene::Scene(Graphics& gfx)
{

	Entity* entity = new Entity(gfx);
	entities.push_back(*entity);
	for (Entity& entity : entities)
	{
		entity.Bind(gfx);
	}
}

void
Scene::UpdateScene(Graphics& gfx, ImVec2 size)
{
	auto start = std::chrono::steady_clock::now();
	gfx.SetProjection(XMMatrixPerspectiveLH(1.0f, size.y / size.x, 0.5f, 100.0f));
	for (Entity& entity : entities)
	{
		entity.Update(gfx);
		entity.Draw(gfx);
	}
	auto end = std::chrono::steady_clock::now();
	dt = std::chrono::duration<float>(end - start).count();
	frameRate = 60 / dt;
	frameCount++;
}

void Scene::AddEntity(Entity& entity)
{
	entities.push_back(entity);
}