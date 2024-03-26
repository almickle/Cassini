#include "Scene.h"

Scene::Scene(Graphics* pGfx)
{
  gfx = pGfx;
  mt19937 rng(random_device{}());
  uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
  uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
  uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
  uniform_real_distribution<float> rdist(6.0f, 20.0f);
  for (auto i = 0; i < 80; i++) {
    cubes.push_back(make_unique<Cube>(*gfx, rng, adist, ddist, odist, rdist));
  }
  gfx->SetProjection(XMMatrixPerspectiveLH(1.0f, 1.0f, 0.5f, 40.0f));
}

void
Scene::UpdateScene()
{
  for (auto& cube : cubes) {
    cube->Update(frameCount * 3.14 / 180);
  }

  frameCount++;
}

void
Scene::DrawScene()
{
  for (auto& cube : cubes) {
    cube->Draw(*gfx);
  }
}