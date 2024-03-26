#pragma once
#include "Graphics.h"
#include "Primitives.h"

class Scene
{
public:
  Scene(Graphics& gfx);
  void UpdateScene(Graphics& gfx, ImVec2 size);

private:
  std::vector<std::unique_ptr<class Drawable>> drawables;
  static constexpr size_t nDrawables = 180;

  float frameCount = 0;
};
