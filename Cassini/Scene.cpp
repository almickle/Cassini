#include "Scene.h"

Scene::Scene(Graphics& gfx) {}

void
Scene::UpdateScene(Graphics& gfx, ImVec2 size)
{
  auto start = std::chrono::steady_clock::now();
  gfx.SetProjection(XMMatrixPerspectiveLH(1.0f, size.y / size.x, 0.5f, 40.0f));

  auto end = std::chrono::steady_clock::now();
  dt = std::chrono::duration<float>(end - start).count();
  frameRate = 60 / dt;
  frameCount++;
}