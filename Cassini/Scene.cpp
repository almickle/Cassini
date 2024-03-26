#include "Scene.h"

Scene::Scene(Graphics& gfx)
{
  mt19937 rng(random_device{}());
  uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
  uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
  uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
  uniform_real_distribution<float> rdist(6.0f, 20.0f);
  for (auto i = 0; i < 80; i++) {
    cubes.push_back(make_unique<Cube>(gfx, rng, adist, ddist, odist, rdist));
  }
}

void
Scene::UpdateScene(Graphics& gfx, ImVec2 size)
{
  gfx.SetProjection(XMMatrixPerspectiveLH(1.0f, size.y / size.x, 0.5f, 40.0f));

  for (auto& cube : cubes) {
    cube->Update(0.01f);
  }

  frameCount++;
}

void
Scene::DrawScene(Graphics& gfx)
{
  for (auto& cube : cubes) {
    cube->Draw(gfx);
  }
}