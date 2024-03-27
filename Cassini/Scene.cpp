#include "Cylinder.h"
#include "Primitives.h"
#include "Scene.h"
#include <algorithm>
#include <chrono>
#include <cmath>

Scene::Scene(Graphics& gfx)
  : light(gfx)
{
  class Factory
  {
  public:
    Factory(Graphics& gfx)
      : gfx(gfx)
    {
    }
    unique_ptr<Drawable> operator()()
    {
      const DirectX::XMFLOAT3 mat = { cdist(rng), cdist(rng), cdist(rng) };

      switch (sdist(rng)) {
        case 0:
          return std::make_unique<Box>(
            gfx, rng, adist, ddist, odist, rdist, bdist, mat);
        case 1:
          return std::make_unique<Cylinder>(
            gfx, rng, adist, ddist, odist, rdist, bdist, tdist);
        default:
          assert(false && "impossible drawable option in factory");
          return {};
      }
    }

  private:
    Graphics& gfx;
    mt19937 rng{ random_device{}() };
    std::uniform_int_distribution<int> sdist{ 0, 1 };
    uniform_real_distribution<float> adist{ 0.0f, PI * 2.0f };
    uniform_real_distribution<float> ddist{ 0.0f, PI * 0.5f };
    uniform_real_distribution<float> odist{ 0.0f, PI * 0.08f };
    uniform_real_distribution<float> rdist{ 6.0f, 20.0f };
    uniform_real_distribution<float> bdist{ 0.4f, 3.0f };
    std::uniform_real_distribution<float> cdist{ 0.0f, 1.0f };
    std::uniform_int_distribution<int> tdist{ 3, 30 };
  };

  drawables.reserve(nDrawables);
  std::generate_n(std::back_inserter(drawables), nDrawables, Factory(gfx));
}

void
Scene::UpdateScene(Graphics& gfx, ImVec2 size)
{
  auto start = std::chrono::steady_clock::now();
  gfx.SetProjection(XMMatrixPerspectiveLH(1.0f, size.y / size.x, 0.5f, 40.0f));
  gfx.SetCamera(cam.GetMatrix());
  light.Bind(gfx, cam.GetMatrix());

  for (auto& d : drawables) {
    d->Update(cbrt(dt * speedFactor / 100));
    d->Draw(gfx);
  }

  // imgui window to control simulation speed
  if (ImGui::Begin("Simulation Speed")) {
    ImGui::SliderFloat("Speed Factor", &speedFactor, 0.0f, 6.0f, "%.4f");
    ImGui::Text("%.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    ImGui::End();
  }

  // imgui windows to control camera and light
  cam.SpawnControlWindow();
  light.SpawnControlWindow();

  light.Draw(gfx);
  auto end = std::chrono::steady_clock::now();
  dt = std::chrono::duration<float>(end - start).count();
  frameRate = 60 / dt;
  frameCount++;
}