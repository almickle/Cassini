#include "Primitives.h"
#include "Scene.h"
#include <algorithm>

Scene::Scene(Graphics& gfx)
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
      switch (typedist(rng)) {
        case 0:
          return make_unique<Pyramid>(gfx, rng, adist, ddist, odist, rdist);
        case 1:
          return make_unique<Box>(gfx, rng, adist, ddist, odist, rdist, bdist);
        case 2:
          return make_unique<Box>(gfx, rng, adist, ddist, odist, rdist, bdist);
        default:
          assert(false && "bad drawable type in factory");
          return {};
      }
    }

  private:
    Graphics& gfx;
    mt19937 rng{ random_device{}() };
    uniform_real_distribution<float> adist{ 0.0f, PI * 2.0f };
    uniform_real_distribution<float> ddist{ 0.0f, PI * 0.5f };
    uniform_real_distribution<float> odist{ 0.0f, PI * 0.08f };
    uniform_real_distribution<float> rdist{ 6.0f, 20.0f };
    uniform_real_distribution<float> bdist{ 0.4f, 3.0f };
    uniform_int_distribution<int> latdist{ 5, 20 };
    uniform_int_distribution<int> longdist{ 10, 40 };
    uniform_int_distribution<int> typedist{ 0, 2 };
  };

  Factory f(gfx);
  drawables.reserve(nDrawables);
  generate_n(back_inserter(drawables), nDrawables, f);
}

void
Scene::UpdateScene(Graphics& gfx, ImVec2 size)
{
  gfx.SetProjection(XMMatrixPerspectiveLH(1.0f, size.y / size.x, 0.5f, 40.0f));

  for (auto& d : drawables) {
    d->Update(0.01f);
    d->Draw(gfx);
  }
  frameCount++;
}